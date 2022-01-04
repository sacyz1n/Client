#include "pch.h"
#include "Timer.h"

Timer::Timer()
{
	mTimeCheckEnd = mTimeCheckStart = mBefore = mNow = mTimer.now();
}

void Timer::Update()
{
	mNow = mTimer.now();
	mPureTime = std::chrono::duration_cast<ms>(mNow - mBefore).count();
	mDeltaTime = mPureTime * mTimeScale;
	mBefore = mNow;

	if (mIsStart)
	{
		mTimeCheckEnd = mNow;

		if (GetTime() >= mReserveTime)
		{
			mIsTimeOver = true;
			Stop();
		}
	}

}

float Timer::GetDeltaTime() const
{
	return mDeltaTime;
}

float Timer::GetPureTime() const
{
	return mPureTime;
}

void Timer::SetTimeScale(const float _timeScale)
{
	mTimeScale = _timeScale;
	mDeltaTime = mPureTime * mTimeScale;
}

void Timer::Start(float _time)
{
	mIsStart = true;
	mTimeCheckStart = mTimer.now();
	mReserveTime = _time;
	mIsTimeOver = false;
}

void Timer::Stop()
{
	mIsStart = false;
	mTimeCheckEnd = mTimer.now();
}

float Timer::GetTime()
{

	return std::chrono::duration_cast<ms>(mTimeCheckEnd - mTimeCheckStart).count();
}
