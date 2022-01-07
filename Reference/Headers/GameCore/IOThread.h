#pragma once

#include "IOContext.h"
#include "Session.h"

namespace GameCore
{
	class MessagePool;
	class DLL_TYPE IOThread
	{
	public:
		IOThread(MessagePool& _messagePool, HANDLE _workIOCP, HANDLE _logicIOCP);
		IOThread(const IOThread&) = delete;
		IOThread& operator=(const IOThread&) = delete;
		~IOThread() = default;

		static NetResult InitStatic();

		void Stop();

		void Join()
		{
			if (mWorkThread->joinable())
				mWorkThread->join();
		}

	private:
		void WorkThread();
		void OnAcceptCompletion(const OverlappedIOContext* _context);
		void OnSendCompletion(const OverlappedIOContext* _context, const DWORD _transfer);
		void OnRecvCompletion(const OverlappedIOContext* _context, const DWORD _recvBytes);
		void OnRecvZeroCompletion(const OverlappedIOContext* _context);
		void OnConnectCompletion(const OverlappedIOContext* _context);
		void OnDisconnectCompletion(const OverlappedIOContext* _context);

		// 남아 있는 데이터들을 패킷으로 조립
		std::optional<int> AssemblePacketLoop(Session* _session, int _remainByte, char* _buffer);
		void HandleExceptionCloseConnection(Session* _session);
		void HandleExceptionWorkThread(Session* _session, const OverlappedIOContext* _context);
		NetResult PostNetMessage(Session* _session, Message* _msg, const DWORD _packetSize = 0);

	private:
		SessionManager& mSessionManager;

		using WORKTHREAD = std::unique_ptr<std::thread>;
		WORKTHREAD mWorkThread;

		HANDLE mWorkIOCP = NULL;
		HANDLE mLogicIOCP = NULL;
		bool  mIsRun = true;

		MessagePool& mMessagePool;

		using  NtStatusToError = ULONG(NTAPI*)(NTSTATUS);
		static NtStatusToError GetOverlappedError;
	};
}