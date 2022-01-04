#pragma once
class ConsoleRendering
{
public:
	static bool Initialize(const SIZE& _size);
	static void Release();

	static bool ChangeBufferSize(const SIZE& _size);

	static void ClearBuffer();
	static void SwapBuffer();

	static void GotoXY(int _x, int _y);

	template <typename ... Args>
	static void Draw(int _x, int _y, const TCHAR* _ch, Args ..._args)
	{
		if (_x >= mSize.cx || _y >= mSize.cy)
			return;

		GotoXY(_x, _y);
		Draw(_ch, std::forward<Args>(_args)...);
	}

	template <typename ... Args>
	static void Draw(int _x, int _y, const TCHAR _ch)
	{
		if (_x >= mSize.cx || _y >= mSize.cy)
			return;

		GotoXY(_x, _y);
		Draw(_ch);
	}

	template <typename ... Args>
	static void Draw(const wchar_t _ch)
	{
		// 화면 위치를 넘는 경우
		if (mCursorPos.X + sizeof(_ch) >= mSize.cx)
			return;
		
		DWORD dw = 0;
		WriteConsole(mCurBuffer, &_ch, sizeof(_ch), &dw, NULL);

		CONSOLE_SCREEN_BUFFER_INFO cbsi;
		GetConsoleScreenBufferInfo(mCurBuffer, &cbsi);
		mCursorPos = cbsi.dwCursorPosition;
	}

	template <typename ... Args>
	static void Draw(const TCHAR* _ch, Args ..._args)
	{
		if (_ch == nullptr)
			assert("Draw Failed - ch is nullptr");

		TCHAR str[MAX_STR] = {};
		_stprintf_s(str, _ch, std::forward<Args>(_args)...);
		DWORD dw = 0;

		int strLength = lstrlen(str);
		 
		// 화면 위치를 넘는 경우
		//_tprintf_s(str);
		if (mCursorPos.X + strLength >= mSize.cx)
		{
			WriteConsole(mCurBuffer, str, (mSize.cx - mCursorPos.X), &dw, NULL);
		}
		else
			WriteConsole(mCurBuffer, str, strLength, &dw, NULL);

		CONSOLE_SCREEN_BUFFER_INFO cbsi;
		GetConsoleScreenBufferInfo(mCurBuffer, &cbsi);
		mCursorPos = cbsi.dwCursorPosition;
	}

private:
	static const int BackBufferCount = 2;
	static HANDLE mBuffer[BackBufferCount];
	static HANDLE mCurBuffer;
	static int mCurBufferIndex;
	static SIZE mSize;
	static SIZE mMaxConsoleSize;
	static COORD mCursorPos;
};

