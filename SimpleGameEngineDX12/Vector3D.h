#pragma once
#include <math.h>
class Vector3D
{
public:
	Vector3D() : x(0), y(0), z(0) {}
	Vector3D(float xx, float yy, float zz) : x(xx), y(yy), z(zz) {}
	Vector3D(const Vector3D& vec) : x(vec.x), y(vec.y), z(vec.z) {}
	~Vector3D() {}

	static Vector3D lerp(const Vector3D& start, const Vector3D& end, float delta)
	{
		return Vector3D(
			(1.0f - delta) * start.x + delta * end.x,
			(1.0f - delta) * start.y + delta * end.y,
			(1.0f - delta) * start.z + delta * end.z
		);
	}

	Vector3D operator +(Vector3D vec)
	{
		return Vector3D(x + vec.x, y + vec.y, z + vec.z);
	}
	Vector3D operator *(float num)
	{
		return Vector3D(x * num, y * num, z * num);
	}
	Vector3D normalize()
	{
		float length = sqrtf(x * x + y * y + z * z);
		return Vector3D(x / length, y / length, z / length);
	}
	Vector3D crossProduct(Vector3D v)
	{
		float x1 = y * v.z - z * v.y;// Cx
		float y1 = z * v.x - x * v.z;// Cy
		float z1 = x * v.y - y * v.x;// Cz

		return Vector3D(x1, y1, z1);
	}
public:
	float x, y, z;
};

