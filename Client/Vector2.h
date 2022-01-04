#pragma once

struct Vector2
{
	float X, Y;

	Vector2()
		: X(0.f)
		, Y(0.f)
	{ }

	Vector2(const float _x, const float _y)
		: X(_x)
		, Y(_y)
	{ }

	Vector2(const float _value)
		: X(_value)
		, Y(_value)
	{ }

	Vector2 operator+(const Vector2& _rhs)
	{
		return Vector2(X + _rhs.X, Y + _rhs.Y);
	}

	Vector2 operator-(const Vector2& _rhs)
	{
		return Vector2(X + _rhs.X, Y + _rhs.Y);
	}

	Vector2& operator+=(const Vector2& _rhs)
	{
		X += _rhs.X;
		Y += _rhs.Y;
		return (*this);
	}

	Vector2 operator*(const float _value)
	{
		return Vector2(X * _value, Y * _value);
	}

	Vector2& operator=(const Vector2& _rhs)
	{
		X = _rhs.X;
		Y = _rhs.Y;
		return (*this);
	}


};

