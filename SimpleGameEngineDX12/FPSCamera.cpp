#include "FPSCamera.h"

FPSCamera::FPSCamera() : speed(0),fov(0),aspectRatio(0)
{
}

FPSCamera::FPSCamera(Vector3D pos, float s, float f, float a)
{
	world_camera.setTranslation(pos);
	fov = f;
	aspectRatio = a;
	speed = s;
}

FPSCamera::~FPSCamera()
{
}

void FPSCamera::updateCamera(float rotate_x, float rotate_y, float move_forward, float move_right)
{
	Matrix4x4 world_cam, temp;
	world_cam.setIdentity();

	temp.setIdentity();
	temp.setRotationX(rotate_x);
	world_cam *= temp;

	temp.setIdentity();
	temp.setRotationY(rotate_y);
	world_cam *= temp;

	Vector3D newPos = world_camera.getTranslation() + world_cam.getZDirection() * move_forward * speed + world_cam.getXDirection() * move_right * speed;

	world_cam.setTranslation(newPos);

	world_camera = world_cam;
	world_cam.inverse();

	view_camera = world_cam;
	
	projection_camera.setPerspectiveProj(fov, aspectRatio, 0.1f, 100.0f);
}
