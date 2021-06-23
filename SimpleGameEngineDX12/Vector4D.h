#pragma once

class Vector4D
{
public:
	Vector4D() : x(0), y(0), z(0), w(0) {}
	Vector4D(float xx, float yy, float zz, float ww) : x(xx), y(yy), z(zz), w(ww) {}
	Vector4D(const Vector4D& vec) : x(vec.x), y(vec.y), z(vec.z), w(vec.w) {}
	~Vector4D() {}

	static Vector4D lerp(const Vector4D& start, const Vector4D& end, float delta)
	{
		return Vector4D(
			(1.0f - delta) * start.x + delta * end.x,
			(1.0f - delta) * start.y + delta * end.y,
			(1.0f - delta) * start.z + delta * end.z,
			(1.0f - delta) * start.w + delta * end.w
		);
	}
	void cross(Vector4D& v1, Vector4D& v2, Vector4D& v3)
	{
		this->x = v1.y * (v2.z * v3.w - v3.z * v2.w) - v1.z * (v2.y * v3.w - v3.y * v2.w) + v1.w * (v2.y * v3.z - v2.z * v3.y);
		this->y = -(v1.x * (v2.z * v3.w - v3.z * v2.w) - v1.z * (v2.x * v3.w - v3.x * v2.w) + v1.w * (v2.x * v3.z - v3.x * v2.z));
		this->z = v1.x * (v2.y * v3.w - v3.y * v2.w) - v1.y * (v2.x * v3.w - v3.x * v2.w) + v1.w * (v2.x * v3.y - v3.x * v2.y);
		this->w = -(v1.x * (v2.y * v3.z - v3.y * v2.z) - v1.y * (v2.x * v3.z - v3.x * v2.z) + v1.z * (v2.x * v3.y - v3.x * v2.y));
	}

public:
	float x, y, z, w;
};