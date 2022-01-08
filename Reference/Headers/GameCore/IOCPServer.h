#pragma once

#include "CoreCommon.h"
#include "Performance.h"

namespace GameCore
{
	class Session;
	class SessionManager;
	class MessagePool;
	class IOThread;
	class Performance;
	class DLL_TYPE IOCPServer
	{
	public:
		IOCPServer();
		~IOCPServer() = default;

		// 시작 종료 함수
		NetResult Run(NetConfig _netConfig);

		void Stop();

		bool GetNetworkMessage(int8& _msgOperationType, int32& _sessonIndex, char* buffer, int16& copySize, const int32 _waitTimeMillSec);

	private:
		// Listen Socket 생성
		NetResult CreateListenSocket();

		// Message Pool 생성
		NetResult CreateMessagePool();

		// IOCP Handle 생성
		NetResult CreateIOCPHandle();

		// Work Thread 생성
		NetResult CreateWorkThread();

		// Listen Socket 과 IOCP 연결
		NetResult LinkListenSocketToIOCP();

		NetResult CreatePerformance();

		NetResult CreateUpdateTimer();

		void MakeNetMessageOnClose(Session* _session, const Message* _msg, int8& _msgOperationType, int32& _sessionIndex);
		void MakeNetMessageOnReceive(Session* _session, const Message* _msg, int8& _msgOperationType, int32& _sessionIndex, char* _buffer, int16& _copySize, const DWORD _ioSize);
		void MakeNetMessageOnConnection(Session* _session, const Message* _msg, int8& _msgOperationType, int32& _sessionIndex);
		void MakeNetMessageOnUpdate(const Message* _msg, int8& _msgOperationType);

	private:
		using WORKTHREADS = std::vector<std::shared_ptr<IOThread>>;

		// Network Configuration
		NetConfig	mNetConfig;

		// Listen Socket
		SOCKET		mListenSocket	= NULL;

		// IOCP Handle
		HANDLE		mWorkIOCP		= NULL;
		HANDLE		mLogicIOCP		= NULL;

		// Work Thread
		bool		mIsRunWorkThread = true;
		WORKTHREADS mWorkThreads;

		// Update Timer
		std::unique_ptr<std::thread> mUpdateThread;

		SessionManager& mSessionManager;
		std::shared_ptr<MessagePool> mMessagePool;

		std::unique_ptr<Performance> mPerformance;
	};
}

