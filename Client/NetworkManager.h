#pragma once

enum class IOType : int
{
	Connect,
	Disconnect,
	Recv,
	Send
};

struct OVERLAPPED_EX
{
	OVERLAPPED overlapped;
	IOType ioType;

	OVERLAPPED_EX(IOType _ioType)
		: ioType(_ioType)
	{
		ZeroMemory(&overlapped, sizeof(overlapped));
	}
};

class NetworkManager
{
	DECLARE_SINGLETON(NetworkManager)

public:
	bool Initialize();
	void Update();
	void Release();

	void TryConnect(const char* _ip, const unsigned short _port);
	void TryDisconnect();
	bool TryRecv();
	bool IsConnect() const { return mIsConnect; }
	void BindIOCP();

	// CallBack
	void OnConnect();
	void OnDisconnect();
	void OnRecv(DWORD _transfer);
	void OnSend(DWORD _transfer);

	void SetNetStateConnect();
	void SetNetStateDisconnect();

	void IncrementRecvIORef() { ++mRecvIORef; }
	void IncrementSendIORef() { ++mSendIORef; }
	void DecrementRecvIORef() { --mRecvIORef; }
	void DecrementSendIORef() { --mSendIORef; }

	void SetNetStateClosing(bool _flag) { mIsClosing = _flag; }
	void SetNetStateConnecting(bool _flag) { mIsConnecting = _flag; }

	bool IsConnecting() { return mIsConnecting; }

private:
	void HandleExceptionWorkThread(OVERLAPPED_EX* _overlapped);
	void PostQueuedMessage(OVERLAPPED_EX* _overlapped);
	void WorkerThread();

private:
	bool   mIsRun = false;
	HANDLE mWorkIOCP = NULL;
	SOCKET mSocket = NULL;

	int mIsConnect = false;
	int mIsClosing = false;
	int mIsConnecting = false;

	// Connect
	OVERLAPPED_EX mConnectOverlapped	= OVERLAPPED_EX(IOType::Connect);
	const char* mConnectIP = nullptr;
	unsigned short mConnectPort = 0;

	OVERLAPPED_EX mDisconnectOverlapped = OVERLAPPED_EX(IOType::Disconnect);

	// Recv
	OVERLAPPED_EX mRecvOverlapped		= OVERLAPPED_EX(IOType::Recv);
	int mRecvBytes = 0;
	WSABUF mRecvWSABuf;
	char mRecvBuffer[1024] = {};

	// Send
	OVERLAPPED_EX mSendOverlapped		= OVERLAPPED_EX(IOType::Send);


	std::atomic<int> mRecvIORef = 0;
	std::atomic<int> mSendIORef = 0;
	std::mutex mDisconnectLock;


	std::unique_ptr<std::thread> mWorkThread;
};

