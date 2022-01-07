#pragma once

#include "CoreCommon.h"
#include "ObjectPool.h"

namespace GameCore
{
	enum class IOType
	{
		IO_NONE,
		IO_SEND,
		IO_RECV,
		IO_RECV_ZERO,
		IO_ACCEPT,
		IO_DISCONNECT,
		IO_CONNECT
	};

	enum class DisconnectReason
	{
		DR_NONE,
		DR_ACTIVE,
		DR_ONCONNECT_ERROR,
		DR_IO_REQUEST_ERROR,
		DR_COMPLETION_ERROR,
		DR_SENDFLUSH_ERROR,
		DR_CONNECT_ERROR,
	};



	struct OverlappedIOContext
	{
		OverlappedIOContext(const int32 _index, IOType _ioType);

		OVERLAPPED	overlapped;
		int32		sessionIndex = INVALID_VALUE;
		IOType		ioType;
		WSABUF		wsaBuf;
	};

	struct OverlappedSendContext : public OverlappedIOContext, public ObjectPool<OverlappedSendContext>
	{
		OverlappedSendContext(const int32 _index) : OverlappedIOContext(_index, IOType::IO_SEND)
		{
		}
	};

	struct OverlappedRecvContext : public OverlappedIOContext, public ObjectPool<OverlappedRecvContext>
	{
		OverlappedRecvContext(const int32 _index) : OverlappedIOContext(_index, IOType::IO_RECV)
		{
		}
	};

	struct OverlappedPreRecvContext : public OverlappedIOContext, public ObjectPool<OverlappedPreRecvContext>
	{
		OverlappedPreRecvContext(const int32 _index) : OverlappedIOContext(_index, IOType::IO_RECV_ZERO)
		{
		}
	};

	struct OverlappedDisconnectContext : public OverlappedIOContext, public ObjectPool<OverlappedDisconnectContext>
	{
		OverlappedDisconnectContext(const int32 _index, DisconnectReason dr)
			: OverlappedIOContext(_index, IOType::IO_DISCONNECT)
			, mDisconnectReason(dr)
		{
		}

		DisconnectReason mDisconnectReason;
	};

	struct OverlappedAcceptContext : public OverlappedIOContext, public ObjectPool<OverlappedAcceptContext, 1024>
	{
		OverlappedAcceptContext(const int32 _index) : OverlappedIOContext(_index, IOType::IO_ACCEPT)
		{
		}
	};

	struct OverlappedConnectContext : public OverlappedIOContext, public ObjectPool<OverlappedConnectContext>
	{
		OverlappedConnectContext(const int32 _index) : OverlappedIOContext(_index, IOType::IO_CONNECT)
		{ }
	};

	void DeleteIoContext(OverlappedIOContext* context);
}


