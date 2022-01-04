#include "pch.h"
#include "TimerManager.h"
#include "Timer.h"

DEFINE_SINGLETON(TimerManager)

TimerManager::~TimerManager()
{
	Release();
}

bool TimerManager::Initialize()
{
	if (mSystemTimer != nullptr)
		return false;

	mSystemTimer = std::make_unique<Timer>();

	return true;
}


void TimerManager::Update()
{
	// Update System Timer 
	mSystemTimer->Update();
	float pureTime = mSystemTimer->GetPureTime();
	mAccTime += pureTime;
	mSystemTime += pureTime;

	// Custom Timer Update
	for (auto& pair : mTimers)
		pair.second->Update();
}

void TimerManager::Release()
{
	mTimers.clear();
}

const float TimerManager::GetAccTime() const
{
	return mAccTime;
}

const float TimerManager::GetSystemTime() const
{
	return mSystemTime;
}

void TimerManager::ResetAccTime()
{
	mAccTime = 0.f;
}
