#include "pch.h"
#include "Transform.h"

Vector2 Transform::MoveDir[static_cast<int>(MoveDirection::End)] =
{
	{ 0.f, -1.f }, // Up 
	{ 0.f,  1.f }, // Down
	{ 1.f,  0.f }, // Right
	{ -1.f, 0.f }  // Left
};

void Transform::Move(MoveDirection _dir)
{
	Position += MoveDir[static_cast<int>(_dir)] * Speed * DELTA_TIME;
}
