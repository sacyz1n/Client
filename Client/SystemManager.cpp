#include "pch.h"
#include "SystemManager.h"

DEFINE_SINGLETON(SystemManager)

SystemManager::~SystemManager()
{

}

bool SystemManager::IsTrue(SYSTEM_OPTION _option)
{
    return (mOption & static_cast<int>(_option)) > 0;
}

void SystemManager::On(SYSTEM_OPTION _option)
{
}

void SystemManager::Off(SYSTEM_OPTION _option)
{
}

void SystemManager::Reset()
{
    mOption = static_cast<int>(SYSTEM_OPTION::SO_NONE);
}

void SystemManager::SetFrameRate(const float _frame)
{
	mLimitFPS = _frame;
	mFrameTime = 1.f / mLimitFPS;
}

const char* SystemManager::GetFPS()
{
	return mStringFPS;
}

void SystemManager::FPSTimeAdd(const float _time)
{
	mFPSTime += _time;

	if (mFPSTime > 1.f)
	{
		mFPSTime = 0.f;
		sprintf_s(mStringFPS, "FPS : %d", mFPS);
		mFPS = 0;
	}
}

void SystemManager::FPSCounter()
{
	++mFPS;
}

void SystemManager::FPSReset()
{
	mFPS = 0;
}

const float SystemManager::LimitFPS()
{
	return mLimitFPS;
}

float SystemManager::GetFrameTime()
{
	return mFrameTime;
}
