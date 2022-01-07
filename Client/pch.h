#pragma once


//#undef _UNICODE
//#undef UNICODE
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <MSWSock.h>
#include <Windows.h>
#include <iostream>
#include <unordered_map>
#include <map>
#include <list>
#include <vector>
#include <queue>
#include <typeinfo>
#include <assert.h>
#include <chrono>
#include <stdarg.h>
#include <tchar.h>
#include <iostream>
#include <thread>
#include <mutex>
#include "Vector2.h"

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "mswsock.lib")

#define MAX_STR 256
#define DECLARE_SINGLETON(CLASSNAME)	\
private:								\
	CLASSNAME() = default;				\
	~CLASSNAME();						\
	static CLASSNAME* mInstance;		\
public:									\
	static CLASSNAME* GetInstance();	\
	static void DestroyInstance();

#define DEFINE_SINGLETON(CLASSNAME)			\
CLASSNAME* CLASSNAME::mInstance = nullptr;	\
CLASSNAME* CLASSNAME::GetInstance()			\
{											\
	if (mInstance == nullptr)				\
		mInstance = new CLASSNAME();		\
	return mInstance;						\
}											\
void CLASSNAME::DestroyInstance()			\
{											\
	if (mInstance)							\
	{										\
		delete mInstance;					\
		mInstance = nullptr;				\
	}										\
}

#include "SystemManager.h"
#include "TimerManager.h"
#include "ConsoleRendering.h"

enum class ObjectType
{
	DYNAMIC_NUMBERING,
	STATIC_NUMBERING,
	DYNAMIC_NORMAL,
	STATIC_NORMAL
};
