#pragma once
class Timer
{
public:
	Timer();
	~Timer() = default;

public:
	void Update();
	float GetDeltaTime() const;
	float GetPureTime() const;
	void SetTimeScale(const float _timeScale);

	// Timer
	void Start(float _time);
	void Stop();
	float GetTime();

private:
	using ms = std::chrono::duration<float, std::milli>;

	std::chrono::high_resolution_clock mTimer;
	std::chrono::steady_clock::time_point mBefore;
	std::chrono::steady_clock::time_point mNow;
	float mDeltaTime = 0.f;
	float mPureTime = 0.f;
	float mTimeScale = 1.f;


	// Timer
	std::chrono::steady_clock::time_point mTimeCheckStart;
	std::chrono::steady_clock::time_point mTimeCheckEnd;
	float mReserveTime = 0.f;
	bool mIsStart = false;
	bool mIsTimeOver = false;
};

