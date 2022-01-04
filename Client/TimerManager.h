#pragma once
class Timer;
class TimerManager
{
	DECLARE_SINGLETON(TimerManager)

public:
	bool Initialize();
	void Update();
	void Release();

	const float GetAccTime() const;
	const float GetSystemTime() const;
	void ResetAccTime();

private:
	float mSystemTime = 0.f;
	float mAccTime = 0.f;

	std::unique_ptr<Timer> mSystemTimer;
	std::unordered_map<const TCHAR*, std::unique_ptr<Timer>> mTimers;
};

#define DELTA_TIME (TimerManager::GetInstance()->GetAccTime())
#define SYSTEM_TIME (TimerManager::GetInstance()->GetSystemTime())