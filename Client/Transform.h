#pragma once
#include "IComponent.h"
enum class MoveDirection
{
	Up, Down, Right, Left, End
};

class Transform : public IComponent
{
public:
	void Move(MoveDirection _dir);

	Vector2 Position;
	float   Speed = 1.f;

private:
	static Vector2 MoveDir[static_cast<int>(MoveDirection::End)];
};

