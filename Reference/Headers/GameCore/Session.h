#pragma once

#include "RecvRingBuffer.h"
#include "SendRingBuffer.h"

namespace GameCore
{
	class Session
	{
	public:
		Session() {}
		~Session() {}

		void Initialize(const SOCKET _listenSocket, const int32 _index, const ConnectionNetConfig& _config);

	public:
		void Init();
		bool BindIOCP(const HANDLE _iocp);
		NetResult BindAcceptExSocket();
		bool	  PostSend();
		NetResult PostRecv();
		void ReadRecvBuffer(int32 _size)
		{
			mRecvRingBuffer.ForwardReadPos(_size);
		}
		bool TryClose();
		void DisconnectSession();
		NetResult ResetSession();
		bool IsConnect() const 
		{ 
			return mIsConnect; 
		}

		void IncrementRecvIORefCount() { InterlockedIncrement(reinterpret_cast<LPLONG>(&mRecvIORefCount)); }
		void IncrementSendIORefCount() { InterlockedIncrement(reinterpret_cast<LPLONG>(&mSendIORefCount)); }
		void IncrementAcceptIORefCount() { ++mAcceptIORefCount; }
		void DecrementRecvIORefCount() { InterlockedDecrement(reinterpret_cast<LPLONG>(&mRecvIORefCount)); }
		void DecrementSendIORefCount() { InterlockedDecrement(reinterpret_cast<LPLONG>(&mSendIORefCount)); }
		void DecrementAcceptIORefCount() { --mAcceptIORefCount; }

		void SetConnectionIP(const char* szIP) 
		{ 
			CopyMemory(mStrIP, szIP, MAX_IP_LENGTH); 
		}
		void SetNetStateConnection()
		{
			InterlockedExchange(reinterpret_cast<LPLONG>(&mIsConnect), TRUE);
		}
		void SetNetStateDisConnection()
		{
			InterlockedExchange(reinterpret_cast<LPLONG>(&mIsConnect), FALSE);
		}
		bool SetNetAddressInfo()
		{
			SOCKADDR* pLocalSockAddr = nullptr;
			SOCKADDR* pRemoteSockAddr = nullptr;

			int	localSockaddrLen = 0;
			int	remoteSockaddrLen = 0;

			GetAcceptExSockaddrs(mAddrBuf, 0,
				sizeof(SOCKADDR_IN) + 16, sizeof(SOCKADDR_IN) + 16,
				&pLocalSockAddr, &localSockaddrLen,
				&pRemoteSockAddr, &remoteSockaddrLen);

			if (remoteSockaddrLen != 0)
			{
				SOCKADDR_IN* pRemoteSockAddrIn = reinterpret_cast<SOCKADDR_IN*>(pRemoteSockAddr);
				if (pRemoteSockAddrIn != nullptr)
				{
					char szIP[MAX_IP_LENGTH] = { 0, };
					inet_ntop(AF_INET, &pRemoteSockAddrIn->sin_addr, szIP, sizeof(szIP));

					SetConnectionIP(szIP);
				}

				return true;
			}

			return false;
		}
		void SetEnableSend()
		{
			InterlockedExchange(reinterpret_cast<LPLONG>(&mIsSendable), TRUE);
		}
		void AddSendCompleted(const int32 _sendSize);

		Message* GetConnectionMsg() { return &mConnectionMsg; }
		Message* GetCloseMsg() { return &mCloseMsg; }
		int32 GetIndex() const;
		std::tuple<int32, char*> GetReceiveData(int32 _recvSize)
		{
			return mRecvRingBuffer.GetReceiveData(_recvSize);
		}
		int32 RecvBufferSize() 
		{ 
			return mRecvRingBuffer.GetBufferSize(); 
		}

	private:
		int32 mIndex = INVALID_VALUE;

		SOCKET mClientSocket = NULL;
		SOCKET mListenSocket = NULL;

		BOOL mIsClosed = FALSE;
		BOOL mIsConnect = FALSE;
		BOOL mIsSendable = TRUE;

		char mAddrBuf[MAX_ADDR_LENGTH] = {};
		char mStrIP[MAX_IP_LENGTH] = { 0, };

		std::mutex mObjLock;

		RecvRingBuffer  mRecvRingBuffer;
		SendRingBuffer  mSendRingBuffer;
		int32			mRecvBufSize = INVALID_VALUE;
		int32			mSendBufSize = INVALID_VALUE;
		DWORD			mSendIORefCount = 0;
		DWORD			mRecvIORefCount = 0;
		std::atomic<short> mAcceptIORefCount = 0;

		Message mConnectionMsg;
		Message mCloseMsg;
	};

	class SessionManager
	{
	private:
		SessionManager() = default;
		SessionManager(const SessionManager&) = delete;
		SessionManager operator=(const SessionManager&) = delete;

	public:
		static SessionManager& GetInstance()
		{
			static SessionManager instance;
			return instance;
		}

		NetResult	CreateSessions(const NetConfig& _netConfig, const SOCKET _listenSocket)
		{
			ConnectionNetConfig config;
			config.MaxSendBufferSize = _netConfig.ConnectionMaxSendBufferSize;
			config.MaxRecvBufferSize = _netConfig.ConnectionMaxRecvBufferSize;
			config.MaxRecvOverlappedBufferSize = _netConfig.MaxPacketSize;
			config.MaxSendOverlappedBufferSize = _netConfig.MaxPacketSize;
			mMaxSessionCount = _netConfig.MaxConnectionCount;

			mSessions.reserve(mMaxSessionCount);
			for (int32 i = 0; i < mMaxSessionCount; ++i)
			{
				auto session = new Session();
				session->Initialize(_listenSocket, i, config);
				mSessions.push_back(session);
			}

			return NetResult::Success;
		}

		void		DestroySessions()
		{
			for (int32 i = 0; i < mMaxSessionCount; ++i)
			{
				delete mSessions[i];
			}

			mSessions.clear();
			mSessions.shrink_to_fit();
		}

		Session*	GetSession(const int32 _index)
		{
			if (_index < 0 || _index >= mMaxSessionCount)
				return nullptr;

			return mSessions[_index];
		}

	private:
		int32				   mMaxSessionCount = 0;
		std::vector<Session*>  mSessions;
	};
}