#pragma once
#include <memory>
#include "Vector3D.h"
#include "Vector4D.h"

class Matrix4x4
{
public:
	Matrix4x4() {}
	~Matrix4x4() {}

	void setMatrix(const Matrix4x4& matrix)
	{
		::memcpy(mat, matrix.mat, sizeof(float) * 16);
	}

	float getDeterminant()
	{
		Vector4D minor, v1, v2, v3;
		float det;

		v1 = Vector4D(this->mat[0][0], this->mat[1][0], this->mat[2][0], this->mat[3][0]);
		v2 = Vector4D(this->mat[0][1], this->mat[1][1], this->mat[2][1], this->mat[3][1]);
		v3 = Vector4D(this->mat[0][2], this->mat[1][2], this->mat[2][2], this->mat[3][2]);


		minor.cross(v1, v2, v3);
		det = -(this->mat[0][3] * minor.x + this->mat[1][3] * minor.y + this->mat[2][3] * minor.z +
			this->mat[3][3] * minor.w);
		return det;
	}

	void inverse()
	{
		int a, i, j;
		Matrix4x4 out;
		Vector4D v, vec[3];
		float det = 0.0f;

		det = this->getDeterminant();
		if (!det) return;
		for (i = 0; i < 4; i++)
		{
			for (j = 0; j < 4; j++)
			{
				if (j != i)
				{
					a = j;
					if (j > i) a = a - 1;
					vec[a].x = (this->mat[j][0]);
					vec[a].y = (this->mat[j][1]);
					vec[a].z = (this->mat[j][2]);
					vec[a].w = (this->mat[j][3]);
				}
			}
			v.cross(vec[0], vec[1], vec[2]);

			out.mat[0][i] = pow(-1.0f, i) * v.x / det;
			out.mat[1][i] = pow(-1.0f, i) * v.y / det;
			out.mat[2][i] = pow(-1.0f, i) * v.z / det;
			out.mat[3][i] = pow(-1.0f, i) * v.w / det;
		}

		this->setMatrix(out);
	}


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
	void setPerspectiveProj(float fov, float aspectRatio, float near_plane, float far_plane)
	{
		float yscale = 1.0f / tan(fov / 2.0f);
		float xscale = yscale / aspectRatio;
		mat[0][0] = xscale;
		mat[1][1] = yscale;
		mat[2][2] = far_plane / (far_plane - near_plane);
		mat[2][3] = 1.0f;
		mat[3][2] = (-near_plane * far_plane) / (far_plane - near_plane);
	}

	void setRotationX(float angle)
	{
		mat[1][1] = cos(angle);
		mat[2][2] = cos(angle);
		mat[2][1] = -sin(angle);
		mat[1][2] = sin(angle);
	}

	void setRotationY(float angle)
	{
		mat[0][0] = cos(angle);
		mat[2][2] = cos(angle);
		mat[0][2] = -sin(angle);
		mat[2][0] = sin(angle);
	}

	void setRotationZ(float angle)
	{
		mat[0][0] = cos(angle);
		mat[1][1] = cos(angle);
		mat[1][0] = -sin(angle);
		mat[0][1] = sin(angle);
	}

	Vector3D getXDirection()
	{
		return Vector3D(mat[0][0], mat[0][1], mat[0][2]);
	}
	Vector3D getYDirection()
	{
		return Vector3D(mat[1][0], mat[1][1], mat[1][2]);
	}
	Vector3D getZDirection()
	{
		return Vector3D(mat[2][0], mat[2][1], mat[2][2]);
	}
	Vector3D getTranslation()
	{
		return Vector3D(mat[3][0], mat[3][1], mat[3][2]);
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

