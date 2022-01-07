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
	bool IsConnect() const { return mIsConnect; }
	void BindIOCP();

	// CallBack
	void OnConnect();
	void OnDisconnect();
	void OnRecv(DWORD _transfer);
	void OnSend(DWORD _transfer);

	void SetNetStateConnect();
	void SetNetStateDisconnect();

	void IncrementDisconnectIORef() { mDisconnectIORef; }
	void IncrementConnectIORef() { ++mConnectIORef; }
	void IncrementRecvIORef() { ++mRecvIORef; }
	void IncrementSendIORef() { ++mSendIORef; }
	void DecrementDisconnectIORef() { --mDisconnectIORef; }
	void DecrementConnectIORef() { --mConnectIORef; }
	void DecrementRecvIORef() { --mRecvIORef; }
	void DecrementSendIORef() { --mSendIORef; }

	void SetNetStateClosing(bool _flag) { mIsClosing = _flag; }
	void SetNetStateConnecting(bool _flag) { mIsConnecting = _flag; }

	bool IsConnecting() { return mIsConnecting; }
	void HandleExceptionWorkThread(OVERLAPPED_EX* _context);

private:
	void WorkerThread();
	static LPFN_CONNECTEX ConnectEx;
	static LPFN_DISCONNECTEX DisconnectEx;

private:
	bool   mIsRun = false;
	HANDLE mWorkIOCP = NULL;
	SOCKET mSocket = NULL;

	int mIsConnect = false;
	int mIsClosing = false;
	int mIsConnecting = false;

	char mConnectBuf[64] = {};

	OVERLAPPED_EX mConnectOverlapped	= OVERLAPPED_EX(IOType::Connect);
	OVERLAPPED_EX mDisconnectOverlapped = OVERLAPPED_EX(IOType::Disconnect);
	OVERLAPPED_EX mRecvOverlapped		= OVERLAPPED_EX(IOType::Recv);
	OVERLAPPED_EX mSendOverlapped		= OVERLAPPED_EX(IOType::Send);

	std::atomic<int> mDisconnectIORef = 0;
	std::atomic<int> mConnectIORef = 0;
	std::atomic<int> mRecvIORef = 0;
	std::atomic<int> mSendIORef = 0;
	std::mutex mDisconnectLock;

	std::unique_ptr<std::thread> mWorkThread;
};

