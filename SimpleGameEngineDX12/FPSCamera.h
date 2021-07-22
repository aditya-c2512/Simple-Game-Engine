#pragma once
#include "Vector3D.h"
#include "Matrix4x4.h"

class FPSCamera
{
public :
	FPSCamera();
	FPSCamera(Vector3D pos, float s, float f, float a);
	~FPSCamera();

	void updateCamera(float rotate_x, float rotate_y, float move_forward, float move_right);

public :
	float speed;
	float fov;
	float aspectRatio;
	Matrix4x4 world_camera;
	Matrix4x4 view_camera;
	Matrix4x4 projection_camera;
};

