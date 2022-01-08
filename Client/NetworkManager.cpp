#include "pch.h"
#include "NetworkManager.h"

DEFINE_SINGLETON(NetworkManager)

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

	mConnectIP = _ip;
	mConnectPort = _port;

	SetNetStateConnecting(TRUE);
	PostQueuedMessage(&mConnectOverlapped);
}

void NetworkManager::TryDisconnect()
{
	// 이미 닫혀있는 경우
	if (mIsConnect == FALSE)
		return;

	if (mIsClosing == TRUE)
	{
		// 모든 I/O 처리가 끝난 경우
		if (mRecvIORef == 0 && mSendIORef == 0)
		{
			SetNetStateDisconnect();
			SetNetStateClosing(FALSE);
		}
		return;
	}

	SetNetStateClosing(TRUE);
	PostQueuedMessage(&mDisconnectOverlapped);
}

bool NetworkManager::TryRecv()
{
	mRecvWSABuf.buf = mRecvBuffer + mRecvBytes;
	mRecvWSABuf.len = sizeof(mRecvBuffer) - mRecvBytes;

	IncrementRecvIORef();
	DWORD flag = 0;
	DWORD recvByte = 0;
	auto result = WSARecv(mSocket, &mRecvWSABuf, 1, &recvByte, &flag, reinterpret_cast<OVERLAPPED*>(&mRecvOverlapped), nullptr);

	if (result == SOCKET_ERROR)
	{
		int error = WSAGetLastError();

		if (error != WSA_IO_PENDING)
		{
			DecrementRecvIORef();
			return false;
		}
	}

	return true;
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
	SOCKADDR_IN sockAddr;
	ZeroMemory(&sockAddr, sizeof(sockAddr));
	sockAddr.sin_family = AF_INET;
	sockAddr.sin_port = htons(mConnectPort);
	inet_pton(AF_INET, mConnectIP, &sockAddr.sin_addr);

	BindIOCP();

	if (connect(mSocket, reinterpret_cast<SOCKADDR*>(&sockAddr), sizeof(sockAddr)) == SOCKET_ERROR)
	{
		SetNetStateConnecting(FALSE);
		return;
	}

	SetNetStateConnecting(FALSE);

	// 연결 상태로 변경
	SetNetStateConnect();

	if (!TryRecv())
		TryDisconnect();
}

void NetworkManager::OnDisconnect()
{
	if (mSocket != INVALID_SOCKET)
	{
		closesocket(mSocket);
		mSocket = INVALID_SOCKET;
	}

	TryDisconnect();
}

void NetworkManager::OnRecv(DWORD _transfer)
{
	DecrementRecvIORef();

	mRecvBytes += _transfer;

	if (!TryRecv())
	{
		
	}
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
	}

	TryDisconnect();
}

void NetworkManager::PostQueuedMessage(OVERLAPPED_EX* _overlapped)
{
	auto result = PostQueuedCompletionStatus(mWorkIOCP, 0, reinterpret_cast<ULONG_PTR>(this), reinterpret_cast<LPOVERLAPPED>(_overlapped));

	if (!result)
	{
		char logmsg[256] = { 0, };
		sprintf_s(logmsg, "NetworkManager::PostQueuedMessage - PostQueuedCompletionStatus(). error:%d, IOType:%d", WSAGetLastError(), _overlapped->ioType);
		assert(logmsg);
		return;
	}
}

void NetworkManager::WorkerThread()
{
	while (mIsRun)
	{
		DWORD transfer = 0;
		LPOVERLAPPED overlapped = nullptr;
		ULONG_PTR key = 0;
		int result = GetQueuedCompletionStatus(mWorkIOCP, &transfer, reinterpret_cast<PULONG_PTR>(&key), &overlapped, INFINITE);

		OVERLAPPED_EX* context = reinterpret_cast<OVERLAPPED_EX*>(overlapped);
		if (result == FALSE)
		{
			if (context != nullptr && (context->ioType == IOType::Recv || context->ioType == IOType::Send))
				HandleExceptionWorkThread(context);

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
