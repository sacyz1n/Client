#include "pch.h"
#include "InputManager.h"

DEFINE_SINGLETON(InputManager)

InputManager::~InputManager()
{
	Release();
}

void InputManager::Update()
{
	mKeyPress = 0;

	if (GetAsyncKeyState(VK_UP) & 0x8000)
		mKeyPress |= static_cast<int>(InputKey::KeyUp);

	if (GetAsyncKeyState(VK_DOWN) & 0x8000)
		mKeyPress |= static_cast<int>(InputKey::KeyDown);

	if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
		mKeyPress |= static_cast<int>(InputKey::KeyRight);

	if (GetAsyncKeyState(VK_LEFT) & 0x8000)
		mKeyPress |= static_cast<int>(InputKey::KeyLeft);
}

void InputManager::Release()
{
}

bool InputManager::IsKeyDown(InputKey _key)
{
	int key = static_cast<int>(_key);

	// ������ ���� ���� �����°Ŷ��
	if (!(mKeyDown & key) && (mKeyPress & key))
	{
		mKeyDown |= key;
		return true;
	}

	// ������ �ְ� ������ ���� �ƴ϶�� => �� ����
	if ((mKeyDown & key) && !(mKeyPress & key))
	{
		mKeyDown ^= key;
		return false;
	}

	return false;
}

bool InputManager::IsKeyPressing(InputKey _key)
{
	return (mKeyPress & static_cast<int>(_key));
}

bool InputManager::IsKeyUp(InputKey _key)
{
	int key = static_cast<int>(_key);

	// KeyUp�� �ƴϰ� KeyPress�� �ƴ϶�� => Ű�� �� ������.
	if (!(mKeyUp & key) && !(mKeyPress & key))
	{
		mKeyUp ^= key;
		return true;
	}

	if (!(mKeyUp & key) && (mKeyPress & key))
	{
		mKeyUp |= key;
		return false;
	}

	return false;
}
