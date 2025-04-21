#pragma once
#pragma once
#include "pch.h"


class Vector2
{
public:
	float x;
	float y;

	Vector2& operator = (Vector2 _pt)
	{
		x = (float)_pt.x;
		y = (float)_pt.y;

		return *this;
	}

	/*float Length()
	{
		return sqrt(x * x + y * y);
	}
	Vector2& Normalize()
	{
		float fLength = Length();

		if (fLength == 0.f)
		{
			return *this;
		}

		x /= fLength;
		y /= fLength;

		return *this;
	}*/
	Vector2 operator + (Vector2 other)
	{
		return Vector2(x + other.x, y + other.y);
	}

	Vector2 operator * (int _i)
	{
		return Vector2(x * (float)_i, y * (float)_i);
	}

	Vector2() : x(0.f), y(0.f)
	{
	}

	Vector2(float _x, float _y) : x(_x), y(_y)
	{
	}
};
