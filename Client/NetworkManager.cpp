#include "pch.h"
#include "NetworkManager.h"

DEFINE_SINGLETON(NetworkManager)

LPFN_CONNECTEX NetworkManager::ConnectEx = nullptr;
LPFN_DISCONNECTEX NetworkManager::DisconnectEx = nullptr;


bool NetworkManager::Initialize()
{
	mIsRun = true;

	{
		WSADATA wsaData;
		auto result = WSAStartup(MAKEWORD(2, 2), &wsaData);
		if (result != 0)
			return false;
	}

	mWorkIOCP = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, 1);
	if (mWorkIOCP == INVALID_HANDLE_VALUE)
		return false;

	mSocket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, NULL, WSA_FLAG_OVERLAPPED);
	if (mSocket == INVALID_SOCKET)
		return false;

	mWorkThread = std::make_unique<std::thread>([&]() { WorkerThread(); });

	{
		DWORD dwBytes;
		int result = 0;

		auto sock = socket(AF_INET, SOCK_STREAM, 0);
		if (sock == INVALID_SOCKET)
			return FALSE;
		// ConnectEx
		{
			GUID guid = WSAID_CONNECTEX;
			result = WSAIoctl(sock, SIO_GET_EXTENSION_FUNCTION_POINTER,
				&guid, sizeof(guid),
				&ConnectEx, sizeof(ConnectEx),
				&dwBytes, NULL, NULL);
			if (result != 0)
				return false;
		}

		// DisconnnectEx
		{
			GUID guid = WSAID_DISCONNECTEX;
			result = WSAIoctl(sock, SIO_GET_EXTENSION_FUNCTION_POINTER,
				&guid, sizeof(guid),
				&DisconnectEx, sizeof(DisconnectEx),
				&dwBytes, NULL, NULL);
			if (result != 0)
				return false;
		}
	}


	return true;
}

NetworkManager::~NetworkManager()
{
	Release();
}

void NetworkManager::Update()
{

}

void NetworkManager::Release()
{
	mIsRun = false;
	TryDisconnect();
	WSACleanup();
}

void NetworkManager::TryConnect(const char* _ip, const unsigned short _port)
{
	if (mIsConnecting == TRUE)
		return;

	if (mIsConnect == TRUE)
		return;

	//SOCKADDR_IN localAddr;
	//ZeroMemory(&localAddr, sizeof(localAddr));
	//localAddr.sin_family = AF_INET;
	//localAddr.sin_port = htons(_port);
	//inet_pton(AF_INET, "127.0.0.1", &localAddr);

	//if (::bind(mSocket, (struct sockaddr*)&localAddr, sizeof(localAddr) == SOCKET_ERROR))
	//	return;

	BindIOCP();

	SOCKADDR_IN sockAddr;
	ZeroMemory(&sockAddr, sizeof(sockAddr));
	sockAddr.sin_family = AF_INET;
	sockAddr.sin_port = htons(_port);
	inet_pton(AF_INET, _ip, &sockAddr.sin_addr);

	SetNetStateConnecting(TRUE);
	IncrementConnectIORef();
	DWORD byteSent = 0;
	BOOL ret = NetworkManager::ConnectEx(mSocket, reinterpret_cast<SOCKADDR*>(&sockAddr), sizeof(sockAddr), 0, 0, 0, reinterpret_cast<OVERLAPPED*>(&mConnectOverlapped));

	if (ret == FALSE || ret == SOCKET_ERROR)
	{
		int error = WSAGetLastError();

		if (error != WSA_IO_PENDING)
		{
			SetNetStateConnecting(FALSE);
			DecrementConnectIORef();
			TryDisconnect();
			return;
		}
	}
}

void NetworkManager::TryDisconnect()
{
	// 이미 닫혀있는 경우
	if (mIsConnect == FALSE)
		return;

	if (mIsClosing == TRUE)
	{
		// 모든 I/O 처리가 끝난 경우
		if (mConnectIORef == 0 && mDisconnectIORef == 0 && mRecvIORef == 0 && mSendIORef == 0)
		{
			SetNetStateDisconnect();
			SetNetStateClosing(FALSE);
		}
		return;
	}

	SetNetStateClosing(TRUE);
	IncrementDisconnectIORef();
	int result = DisconnectEx(mSocket, reinterpret_cast<LPOVERLAPPED>(&mDisconnectOverlapped), TF_REUSE_SOCKET, 0);

	if (result == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSA_IO_PENDING)
		{
			std::cout << "DisconnectEx Failed" << std::endl;
			return;
		}
	}
}

void NetworkManager::BindIOCP()
{
	// 즉시 접속종료 소켓 옵션 추가
	linger li = { 0, 0 };
	li.l_onoff = 1;
	setsockopt(mSocket, SOL_SOCKET, SO_LINGER, reinterpret_cast<const char*>(&li), sizeof(li));

	auto iocpHandle = CreateIoCompletionPort(
		reinterpret_cast<HANDLE>(mSocket),
		mWorkIOCP,
		reinterpret_cast<ULONG_PTR>(this),
		0);

	if (iocpHandle == INVALID_HANDLE_VALUE || iocpHandle != mWorkIOCP)
		return;
}

void NetworkManager::OnConnect()
{
	SetNetStateConnecting(FALSE);

	DecrementConnectIORef();

	// 연결 상태로 변경
	SetNetStateConnect();

}

void NetworkManager::OnDisconnect()
{
	DecrementDisconnectIORef();

	// disconnect 마무리 체크
	TryDisconnect();

	if (mSocket != INVALID_SOCKET)
	{
		closesocket(mSocket);
		mSocket = INVALID_SOCKET;
	}
}

void NetworkManager::OnRecv(DWORD _transfer)
{
	DecrementRecvIORef();
}

void NetworkManager::OnSend(DWORD _transfer)
{
	DecrementSendIORef();
}

void NetworkManager::SetNetStateConnect()
{
	InterlockedExchange(reinterpret_cast<PLONG>(&mIsConnect), TRUE);
}

void NetworkManager::SetNetStateDisconnect()
{
	InterlockedExchange(reinterpret_cast<PLONG>(&mIsConnect), FALSE);
}


void NetworkManager::HandleExceptionWorkThread(OVERLAPPED_EX* _context)
{
	if (_context == nullptr)
		return;

	//Connection 접속 종료 시 남은 IO 처리
	switch (_context->ioType)
	{
	case IOType::Recv:
		DecrementRecvIORef();
		break;
	case IOType::Send:
		DecrementSendIORef();
		break;
	case IOType::Connect:
		DecrementConnectIORef();
		break;
	}

	TryDisconnect();
}

void NetworkManager::WorkerThread()
{
	while (mIsRun)
	{
		DWORD transfer = 0;
		LPOVERLAPPED overlapped = nullptr;
		ULONG_PTR key = 0;
		int result = GetQueuedCompletionStatus(mWorkIOCP, &transfer, reinterpret_cast<PULONG_PTR>(&key), &overlapped, INFINITE);

		OVERLAPPED_EX* context = reinterpret_cast<OVERLAPPED_EX*>(&overlapped);
		if (result == FALSE || transfer == 0)
		{
			if (context != nullptr && context->ioType != IOType::Disconnect)
				TryDisconnect();

			continue;
		}


		switch (context->ioType)
		{
		case IOType::Connect:
			OnConnect();
			break;
		case IOType::Disconnect:
			OnDisconnect();
			break;
		case IOType::Send:
			OnSend(transfer);
			break;
		case IOType::Recv:
			OnRecv(transfer);
			break;
		}
	}
}
