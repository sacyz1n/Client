#pragma once

enum class SYSTEM_OPTION : int
{
	SO_NONE = 0,
	SO_PACKET_LOCK			= 1 << 0,
	SO_QUIT_APPLICATION		= 1 << 1,
};

class SystemManager
{
	DECLARE_SINGLETON(SystemManager)

private:

	int mOption = static_cast<int>(SYSTEM_OPTION::SO_NONE);

	float mLimitFPS = 60.f;
	float mFrameTime = 1.f / mLimitFPS;

	float mFPSTime = 0.f;
	int   mFPS = 0;
	char  mStringFPS[MAX_STR] = {};

	SIZE  mConsoleSize = { 800, 600 };

public:
	bool IsTrue(SYSTEM_OPTION _option);
	void On(SYSTEM_OPTION _option);
	void Off(SYSTEM_OPTION _option);
	void Reset();

	const float LimitFPS();
	void SetFrameRate(const float _frame);
	float GetFrameTime();

	const char* GetFPS();
	void FPSCounter();
	void FPSReset();
	void FPSTimeAdd(const float _time);
};

