#pragma once

#ifndef __CORE_COMMON__
#define __CORE_COMMON__

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <MSWSock.h>
#include <Windows.h>
#include <iostream>
#include <vector>
#include <map>
#include <list>
#include <stack>
#include <queue>
#include <concurrent_vector.h>
#include <concurrent_queue.h>
#include <concurrent_priority_queue.h>
#include <memory>
#include <thread>
#include <string>
#include <atomic>
#include <mutex>
#include <tchar.h>

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib,"mswsock.lib")
#pragma comment(lib, "winmm.lib")

#ifdef EXPORT_DLL
#define DLL_TYPE __declspec(dllexport)
#else
#define DLL_TYPE __declspec(dllimport)
#endif


namespace GameCore
{
	DLL_TYPE void __cdecl AssertMessage(const TCHAR* _message, const TCHAR* _file, const unsigned int _line);
};


// Assert macro define
#ifdef _DEBUG

#define __BREAK() (__nop( ), __debugbreak( ))
#define _ASSERT_PRINT(expression) (GameCore::AssertMessage(_CRT_WIDE(#expression), _CRT_WIDE(__FILE__), (unsigned)(__LINE__)), 0)
#define ASSERT(expression) (void)( (!!(expression)) || _ASSERT_PRINT(expression) ); __BREAK( )

#else // NDEBUG

#define __BREAK() exit(IDABORT)
#define _ASSERT_PRINT(expression) (void)(expression)
#define ASSERT(expression) __assume(expression) // not recoverable error

#endif // _DEBUG end


#pragma warning(disable : 4251)

using int8		= __int8;
using int16		= __int16;
using int32		= __int32;
using uint8		= unsigned __int8;
using uint16	= unsigned __int16;
using uint32	= unsigned __int32;

#include "PacketBase.h"

namespace GameCore
{
	const int32 INVALID_VALUE = -1;

	enum class LogLevel : int32
	{
		Trace = 0,
		Debug,
		Info,
		Warn,
		Error,
		Fetal
	};

	// Log Function
	static void (*LogFunction)(const LogLevel, const char* _logMsg);

	enum class NetResult : __int16
	{
		Success = 0,

		fail_log_function_is_nullptr = 29,
		fail_make_directories_log = 30,
		fail_make_directories_dump = 31,
		fail_workThread_info = 32,
		fail_server_info_port = 33,
		fail_server_info_max_recv_ovelapped_buffer_size = 34,
		fail_server_info_max_send_ovelapped_buffer_size = 35,
		fail_server_info_max_recv_connection_buffer_size = 36,
		fail_server_info_max_send_connection_buffer_size = 37,
		fail_server_info_max_packet_size = 38,
		fail_server_info_max_connection_count = 39,
		fail_server_info_max_message_pool_count = 40,
		fail_server_info_extra_message_pool_count = 41,
		fail_server_info_performance_packet_milliseconds_time = 42,
		fail_server_info_post_messages_threads_count = 43,
		fail_create_listensocket_startup = 45,
		fail_create_listensocket_socket = 46,
		fail_create_listensocket_bind = 47,
		fail_create_listensocket_listen = 48,
		fail_handleiocp_work = 49,
		fail_handleiocp_logic = 50,
		fail_create_message_manager = 51,
		fail_link_iocp = 52,
		fail_create_connection = 53,
		fail_create_performance = 54,
		fail_create_workthread = 55,

		BindAcceptExSocket_fail_WSASocket = 56,
		BindAcceptExSocket_fail_AcceptEx = 57,

		fail_message_null = 58,
		fail_pqcs = 59,

		PostRecv_Null_Obj = 61,
		PostRecv_Null_WSABUF = 62,
		PostRecv_Null_Socket_Error = 63,
		PostRecv_Disconnect = 64,

		ReservedSendPacketBuffer_Not_Connected = 90,
		ReservedSendPacketBuffer_Empty_Buffer = 91,

		FUNCTION_RESULT_END
	};

	struct NetConfig
	{
		uint16 PortNumber = 0;
		int32 WorkThreadCount = INVALID_VALUE;
		int32 ConnectionMaxRecvBufferSize = INVALID_VALUE; // 받기용 버퍼의 최대 크기(데이터를 받으면 여기에 저장되고, 데이터의 위치가 애플리케이션에 전달되므로 넉넉하게 커야 한다)
		int32 ConnectionMaxSendBufferSize = INVALID_VALUE; // 보내기용 버퍼의 최대 크기
		int32 MaxPacketSize = INVALID_VALUE;
		int32 MaxConnectionCount = INVALID_VALUE;
		int32 MaxMessagePoolCount = INVALID_VALUE;
		int32 ExtraMessagePoolCount = INVALID_VALUE;
		int32 PerformancePacketMillisecondsTime = INVALID_VALUE;
	};

	struct ConnectionNetConfig
	{
		int32 MaxRecvBufferSize;
		int32 MaxSendBufferSize;
		int32 MaxRecvOverlappedBufferSize;
		int32 MaxSendOverlappedBufferSize;
	};

	enum class MessageType : int8
	{
		None = 0,
		OnConnect,
		OnClose,
		OnRecv
	};

	struct Message
	{
		MessageType Type = MessageType::None;
		PACKET_HEADER* Contents = nullptr;

		void Clear()
		{
			Type = MessageType::None;
			Contents = nullptr;
		}

		void SetMessage(MessageType _msgType, PACKET_HEADER* _contents)
		{
			Type = _msgType;
			Contents = _contents;
		}
	};

}

#define NET_RESULT_CHECK(Result)			\
if (Result != GameCore::NetResult::Success)	\
{											\
	return Result;							\
}



#define MAX_IP_LENGTH 20
#define MAX_ADDR_LENGTH 64


#endif __CORE_COMMON__