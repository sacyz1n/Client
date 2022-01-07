#pragma once

#include "CoreCommon.h"

class FastSpinlock
{
public:
	FastSpinlock() = default;
	~FastSpinlock() = default;

	void EnterLock()
	{
		for (int nloops = 0; ; nloops++)
		{
			if (InterlockedExchange(&mLockFlag, 1) == 0) {
				return;
			}

			UINT uTimerRes = 1;
			timeBeginPeriod(uTimerRes);
			Sleep((DWORD)min(10, nloops));
			timeEndPeriod(uTimerRes);
		}
	}

	void LeaveLock()
	{
		InterlockedExchange(&mLockFlag, 0);
	}

private:
	FastSpinlock(const FastSpinlock& rhs) = delete;
	FastSpinlock& operator=(const FastSpinlock& rhs) = delete;

	volatile long mLockFlag = 0;
};

class FastSpinlockGuard
{
public:
	FastSpinlockGuard(FastSpinlock& lock) : mLock(lock)
	{
		mLock.EnterLock();
	}

	~FastSpinlockGuard()
	{
		mLock.LeaveLock();
	}

private:
	FastSpinlock& mLock;
};

template <class TargetClass>
class ClassTypeLock
{
public:
	struct LockGuard
	{
		LockGuard()
		{
			TargetClass::mLock.EnterLock();
		}

		~LockGuard()
		{
			TargetClass::mLock.LeaveLock();
		}

	};

private:
	static FastSpinlock mLock;

	//friend struct LockGuard;
};

template <class TargetClass>
FastSpinlock ClassTypeLock<TargetClass>::mLock;