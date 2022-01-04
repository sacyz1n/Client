#pragma once


//#undef _UNICODE
//#undef UNICODE
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
#include "Vector2.h"

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
