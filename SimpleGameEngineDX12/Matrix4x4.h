#pragma once
#include <memory>
#include "Vector3D.h"

class Matrix4x4
{
public:
	Matrix4x4() {}
	~Matrix4x4() {}

	void setIdentity()
	{
		memset(mat, 0, sizeof(float) * 4 * 4);
		mat[0][0] = 1.0f;
		mat[1][1] = 1.0f;
		mat[2][2] = 1.0f;
		mat[3][3] = 1.0f;
	}
	void setTranslation(const Vector3D& v)
	{
		setIdentity();
		mat[3][0] = v.x;
		mat[3][1] = v.y;
		mat[3][2] = v.z;
	}
	void setScale(Vector3D s)
	{
		memset(mat, 0, sizeof(float) * 4 * 4);
		mat[0][0] = s.x;
		mat[1][1] = s.y;
		mat[2][2] = s.z;
		mat[3][3] = 1.0f;
	}
	void setOrthoProj(float width, float height, float near_plane, float far_plane)
	{
		setIdentity();
		mat[0][0] = 2.0f / width;
		mat[1][1] = 2.0f / height;
		mat[2][2] = 1.0f / (far_plane - near_plane);
		mat[3][2] = (-near_plane) / (far_plane - near_plane);
	}

	void setRotationX(float angle)
	{
		setIdentity();
		mat[1][1] = cos(angle);
		mat[2][2] = cos(angle);
		mat[2][1] = -sin(angle);
		mat[1][2] = sin(angle);
	}

	void setRotationY(float angle)
	{
		setIdentity();
		mat[0][0] = cos(angle);
		mat[2][2] = cos(angle);
		mat[0][2] = -sin(angle);
		mat[2][0] = sin(angle);
	}

	void setRotationZ(float angle)
	{
		setIdentity();
		mat[0][0] = cos(angle);
		mat[1][1] = cos(angle);
		mat[1][0] = -sin(angle);
		mat[0][1] = sin(angle);
	}

	void operator*=(const Matrix4x4& matrix)
	{
		Matrix4x4 out;
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				out.mat[i][j] = mat[i][0] * matrix.mat[0][j] +
							mat[i][1] * matrix.mat[1][j] +
							mat[i][2] * matrix.mat[2][j] +
							mat[i][3] * matrix.mat[3][j];
			}
		}

		memcpy(mat, out.mat, sizeof(float) * 4 * 4);
	}

public:
	float mat[4][4] = {};
};

