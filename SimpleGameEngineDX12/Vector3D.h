#pragma once

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

public:
	float x, y, z;
};

