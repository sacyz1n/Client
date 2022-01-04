#include "pch.h"
#include "ConsoleRendering.h"

HANDLE ConsoleRendering::mBuffer[2] = {};
HANDLE ConsoleRendering::mCurBuffer = NULL;
int ConsoleRendering::mCurBufferIndex = 0;
SIZE ConsoleRendering::mSize = {};
SIZE ConsoleRendering::mMaxConsoleSize = {};
COORD ConsoleRendering::mCursorPos = {};

bool ConsoleRendering::Initialize(const SIZE& _size)
{
	CONSOLE_CURSOR_INFO cci;
	cci.bVisible = TRUE;
	cci.dwSize = 1;

	for (int i = 0; i < BackBufferCount; ++i)
		mBuffer[i] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	
	mCurBuffer = mBuffer[mCurBufferIndex];

	auto size = GetLargestConsoleWindowSize(mCurBuffer);
	mMaxConsoleSize.cx = size.X;
	mMaxConsoleSize.cy = size.Y;

	ChangeBufferSize(_size);

	return true;
}

void ConsoleRendering::Release()
{
	for (int i = 0; i < BackBufferCount; ++i)
		CloseHandle(mBuffer[i]);
}

bool ConsoleRendering::ChangeBufferSize(const SIZE& _size)
{
	mSize = SIZE(min(mMaxConsoleSize.cx, _size.cx), min(mMaxConsoleSize.cy, _size.cy));
	_COORD coord{ mSize.cx, mSize.cy };
	_SMALL_RECT rect{ 0, 0, mSize.cx - 1, _size.cy - 1 };

	for (int i = 0; i < BackBufferCount; ++i)
	{
		HANDLE handle = mBuffer[i];
		SetConsoleScreenBufferSize(handle, coord);
		SetConsoleWindowInfo(handle, TRUE, &rect);

		//std::ostream stream(mCurBuffer);
	}

	return true;
}

void ConsoleRendering::ClearBuffer()
{
	COORD Coor = { 0, 0 };
	DWORD dw;
	FillConsoleOutputCharacter(mCurBuffer, ' ', mSize.cx * mSize.cy, Coor, &dw);
	GotoXY(0, 0);
}

void ConsoleRendering::SwapBuffer()
{
	SetConsoleActiveScreenBuffer(mBuffer[mCurBufferIndex]);
	mCurBufferIndex = (mCurBufferIndex + 1) % BackBufferCount;
	mCurBuffer = mBuffer[mCurBufferIndex];
}

void ConsoleRendering::GotoXY(int _x, int _y)
{
	mCursorPos = { static_cast<short>(_x), static_cast<short>(_y) };
	SetConsoleCursorPosition(mCurBuffer, mCursorPos);
}
