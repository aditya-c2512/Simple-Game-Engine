#pragma once

class Vector2D
{
public:
	Vector2D() : x(0), y(0){}
	Vector2D(float xx, float yy) : x(xx), y(yy){}
	Vector2D(const Vector2D& vec) : x(vec.x), y(vec.y) {}
	~Vector2D() {}

	static Vector2D lerp(const Vector2D& start, const Vector2D& end, float delta)
	{
		return Vector2D(
			(1.0f - delta) * start.x + delta * end.x,
			(1.0f - delta) * start.y + delta * end.y
		);
	}

	Vector2D operator +(Vector2D vec)
	{
		return Vector2D(x + vec.x, y + vec.y);
	}
	Vector2D operator *(float num)
	{
		return Vector2D(x * num, y * num);
	}
public:
	float x, y;
};