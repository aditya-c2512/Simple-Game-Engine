#include "AppWindow.h"
#include "InputSystem.h"
#include "Vector2D.h"
#include "Vector3D.h"
#include "Vector4D.h"
#include "Matrix4x4.h"
#include "Mesh.h"

#include <Windows.h>

struct vertex
{
	Vector3D position;
	Vector2D texcoord;
};

__declspec(align(16))
struct constant
{
	Matrix4x4 worldMatrix;
	Matrix4x4 viewMatrix;
	Matrix4x4 projectionMatrix;
	Vector4D light_direction;
	Vector4D camera_position;
	Vector4D light_position = Vector4D(0, 1, 0, 1);
	float light_radius = 2.0f;
	float time = 0.0f;
};

AppWindow::AppWindow()
{
}

AppWindow::~AppWindow()
{
}

void AppWindow::onCreate()
{
	Window::onCreate();

	float aspectRatio = (float)(this->getClientWindowRect().right - this->getClientWindowRect().left) / (float)(this->getClientWindowRect().bottom - this->getClientWindowRect().top);
	camera = FPSCamera(Vector3D(0, 1, -2), 0.07f, 1.57f, aspectRatio);

	play_state = true;
	InputSystem::get()->addListener(this);
	InputSystem::get()->showCursor(false);

	/*
	TEX_earth = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"assets\\Textures\\earth_color.jpg");
	TEX_earth_night = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"assets\\Textures\\earth_night.jpg");
	TEX_earth_spec = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"assets\\Textures\\earth_spec.jpg");
	TEX_cloud = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"assets\\Textures\\clouds.jpg");
	*/
	TEX_scene = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"assets\\Textures\\factory_brick.jpg");
	SM_mesh = GraphicsEngine::get()->getMeshManager()->createMeshFromFile(L"assets\\Meshes\\scene.obj");

	TEX_sky = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"assets\\Textures\\hdri_sky.jpg");
	SM_sky_mesh = GraphicsEngine::get()->getMeshManager()->createMeshFromFile(L"assets\\Meshes\\sphere.obj");

	RECT rc = this->getClientWindowRect();
	m_swap_chain = GraphicsEngine::get()->getRenderSystem()->createSwapChain(this->m_hwnd, rc.right - rc.left, rc.bottom - rc.top);

	void* shader_byte_code = nullptr;
	size_t size_shader = 0;

	GraphicsEngine::get()->getRenderSystem()->compileVertexShader(L"PointLightVertexShader.hlsl", "vsmain", &shader_byte_code, &size_shader);
	m_vertex_shader = GraphicsEngine::get()->getRenderSystem()->createVertexShader(shader_byte_code, size_shader);
	GraphicsEngine::get()->getRenderSystem()->releaseCompiledShader();

	GraphicsEngine::get()->getRenderSystem()->compilePixelShader(L"PointLightPixelShader.hlsl", "psmain", &shader_byte_code, &size_shader);
	m_pixel_shader = GraphicsEngine::get()->getRenderSystem()->createPixelShader(shader_byte_code, size_shader);

	GraphicsEngine::get()->getRenderSystem()->compilePixelShader(L"SkyPixelShader.hlsl", "psmain", &shader_byte_code, &size_shader);
	m_sky_pixel_shader = GraphicsEngine::get()->getRenderSystem()->createPixelShader(shader_byte_code, size_shader);

	GraphicsEngine::get()->getRenderSystem()->releaseCompiledShader();

	constant cc;
	m_cb = GraphicsEngine::get()->getRenderSystem()->createConstantBuffer(&cc, sizeof(constant));
	m_sky_cb = GraphicsEngine::get()->getRenderSystem()->createConstantBuffer(&cc, sizeof(constant));
}

void AppWindow::onUpdate()
{
	Window::onUpdate();

	InputSystem::get()->update();

	this->render();
}

void AppWindow::onDestroy()
{
	Window::onDestroy();
	m_swap_chain->setFullscreen(false, 1, 1);
}

void AppWindow::onFocus()
{
	InputSystem::get()->addListener(this);
}

void AppWindow::onKillFocus()
{
	InputSystem::get()->removeListener(this);
}

void AppWindow::onSize()
{
	RECT rc = this->getClientWindowRect();
	m_swap_chain->resize(rc.right, rc.bottom);
	this->render();
}

void AppWindow::render()
{
	//CLEAR THE RENDER TARGET 
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->clearRenderTargetColor(this->m_swap_chain, 0.3f, 0.4f, 0.5f, 1);
	//SET VIEWPORT OF RENDER TARGET IN WHICH WE HAVE TO DRAW
	RECT rc = this->getClientWindowRect();
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setViewportSize(rc.right - rc.left, rc.bottom - rc.top);

	update();

	TexturePtr textures[1];
	textures[0] = TEX_scene;
	/*textures[0] = TEX_earth;
	textures[1] = TEX_earth_spec;
	textures[2] = TEX_cloud;
	textures[3] = TEX_earth_night;*/

	GraphicsEngine::get()->getRenderSystem()->setRasterizerState(false);
	SM_mesh->drawMesh(m_vertex_shader, m_pixel_shader, m_cb, textures, 1);

	textures[0] = TEX_sky;
	GraphicsEngine::get()->getRenderSystem()->setRasterizerState(true);
	SM_sky_mesh->drawMesh(m_vertex_shader, m_sky_pixel_shader, m_sky_cb, textures, 1);

	m_swap_chain->present(true);

	oldDelta = newDelta;
	newDelta = ::GetTickCount();
	deltaTime = (oldDelta) ? ((newDelta - oldDelta) / 1000.0f) : 0.0f;
	time += deltaTime;
}

void AppWindow::update()
{
	camera.updateCamera(rotate_x, rotate_y, move_forward, move_right);
	updateModel();
	updateSkyBox();
}

void AppWindow::updateModel()
{
	constant cc;
	Matrix4x4 light_rot;
	light_rot.setIdentity();
	light_rot.setRotationY(rotate_light_y);
	rotate_light_y += 0.707f * deltaTime;

	cc.worldMatrix.setIdentity();
	cc.worldMatrix.setTranslation(Vector3D(0, 0, 0));
	cc.viewMatrix = camera.view_camera;
	cc.projectionMatrix = camera.projection_camera;
	cc.camera_position = camera.world_camera.getTranslation();
	cc.light_direction = light_rot.getZDirection();
	float dist_from_orig = 1.0f;
	cc.light_position = Vector4D(cos(rotate_light_y) * dist_from_orig, 1.0f, sin(rotate_light_y) * dist_from_orig,1.0f);
	cc.time = time;

	m_cb->update(GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext(), &cc);
}

void AppWindow::updateSkyBox()
{
	constant cc;

	cc.worldMatrix.setIdentity();
	cc.worldMatrix.setScale(Vector3D(100, 100, 100));
	cc.worldMatrix.setTranslation(camera.world_camera.getTranslation());
	cc.viewMatrix = camera.view_camera;
	cc.projectionMatrix = camera.projection_camera;

	m_sky_cb->update(GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext(), &cc);
}

void AppWindow::onKeyDown(int key)
{
	if (key == 'W')
	{
		move_forward = 1.0f;
	}
	else if (key == 'S')
	{
		move_forward = -1.0f;
	}
	else if (key == 'A')
	{
		move_right = -1.0f;
	}
	else if (key == 'D')
	{
		move_right = 1.0f;
	}
}
void AppWindow::onKeyUp(int key)
{
	move_forward = move_right = 0.0f;

	if (key == 'G' || key == 27)
	{
		play_state = (play_state) ? false : true;
		InputSystem::get()->showCursor(!play_state);
	}
	if (key == 'F')
	{
		fullscreen_state = (fullscreen_state) ? false : true;
		RECT rc = this->getScreenSize();
		m_swap_chain->setFullscreen(fullscreen_state, rc.right, rc.bottom);
	}
}
void AppWindow::onMouseMove(const Point& mouse_pos)
{
	if(!play_state) return;
	int width = this->getClientWindowRect().right - this->getClientWindowRect().left;
	int height = this->getClientWindowRect().bottom - this->getClientWindowRect().top;
	rotate_x += 0.3 * (mouse_pos.y - height / 2.0f) * deltaTime;
	rotate_y += 0.3 * (mouse_pos.x - width / 2.0f) * deltaTime;

	InputSystem::get()->setCursorPosition(Point(width / 2, height / 2));
}
void AppWindow::onLeftMouseDown(const Point& mouse_pos)
{
}
void AppWindow::onLeftMouseUp(const Point& mouse_pos)
{
}
void AppWindow::onRightMouseDown(const Point& mouse_pos)
{
}
void AppWindow::onRightMouseUp(const Point& mouse_pos)
{
}
