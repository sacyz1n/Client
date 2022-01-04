#pragma once

enum class InputKey : int
{
	KeyUp			= 1 << 0,
	KeyDown			= 1 << 1,
	KeyLeft			= 1 << 2,
	KeyRight		= 1 << 3,


};

class InputManager
{
	DECLARE_SINGLETON(InputManager)

public:
	void Update();
	void Release();

	bool IsKeyDown(InputKey _key);
	bool IsKeyPressing(InputKey _key);
	bool IsKeyUp(InputKey _key);

private:
	int mKeyDown = 0;
	int mKeyPress = 0;
	int mKeyUp = 0;
};


