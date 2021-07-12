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

	world_camera.setTranslation(Vector3D(0, 0, -2));

	InputSystem::get()->addListener(this);
	InputSystem::get()->showCursor(false);

	TEX_wood = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"assets\\Textures\\sand.jpg");
	SM_mesh = GraphicsEngine::get()->getMeshManager()->createMeshFromFile(L"assets\\Meshes\\suzanne.obj");

	TEX_sky = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"assets\\Textures\\hdri_skybox.png");
	SM_sky_mesh = GraphicsEngine::get()->getMeshManager()->createMeshFromFile(L"assets\\Meshes\\sphere.obj");

	RECT rc = this->getClientWindowRect();
	m_swap_chain = GraphicsEngine::get()->getRenderSystem()->createSwapChain(this->m_hwnd, rc.right - rc.left, rc.bottom - rc.top);

	void* shader_byte_code = nullptr;
	size_t size_shader = 0;

	GraphicsEngine::get()->getRenderSystem()->compileVertexShader(L"VertexShader.hlsl", "vsmain", &shader_byte_code, &size_shader);
	m_vertex_shader = GraphicsEngine::get()->getRenderSystem()->createVertexShader(shader_byte_code, size_shader);
	GraphicsEngine::get()->getRenderSystem()->releaseCompiledShader();

	GraphicsEngine::get()->getRenderSystem()->compilePixelShader(L"PixelShader.hlsl", "psmain", &shader_byte_code, &size_shader);
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

	//CLEAR THE RENDER TARGET 
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->clearRenderTargetColor(this->m_swap_chain, 0.3f, 0.4f, 0.5f, 1);
	//SET VIEWPORT OF RENDER TARGET IN WHICH WE HAVE TO DRAW
	RECT rc = this->getClientWindowRect();
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setViewportSize(rc.right - rc.left, rc.bottom - rc.top);

	update();

	GraphicsEngine::get()->getRenderSystem()->setRasterizerState(false);
	drawMesh(SM_mesh, m_vertex_shader, m_pixel_shader, m_cb, TEX_wood);

	GraphicsEngine::get()->getRenderSystem()->setRasterizerState(true);
	drawMesh(SM_sky_mesh, m_vertex_shader, m_sky_pixel_shader, m_sky_cb, TEX_sky);

	m_swap_chain->present(true);

	oldDelta = newDelta;
	newDelta = ::GetTickCount();
	deltaTime = (oldDelta)?((newDelta - oldDelta)/1000.0f):0.0f;
}

void AppWindow::onDestroy()
{
	Window::onDestroy();
}

void AppWindow::onFocus()
{
	InputSystem::get()->addListener(this);
}

void AppWindow::onKillFocus()
{
	InputSystem::get()->removeListener(this);
}

void AppWindow::update()
{
	updateCamera();
	updateModel();
	updateSkyBox();
}

void AppWindow::updateCamera()
{
	Matrix4x4 world_cam, temp;
	world_cam.setIdentity();

	temp.setIdentity();
	temp.setRotationX(rotate_x);
	world_cam *= temp;

	temp.setIdentity();
	temp.setRotationY(rotate_y);
	world_cam *= temp;

	Vector3D newPos = world_camera.getTranslation() + world_cam.getZDirection() * move_forward * 0.05f + world_cam.getXDirection() * move_right * 0.05f;

	world_cam.setTranslation(newPos);

	world_camera = world_cam;
	world_cam.inverse();

	view_camera = world_cam;
	float aspectRatio = (float)(this->getClientWindowRect().right - this->getClientWindowRect().left) / (float)(this->getClientWindowRect().bottom - this->getClientWindowRect().top);
	projection_camera.setPerspectiveProj(1.57f, aspectRatio, 0.1f, 100.0f);
}

void AppWindow::updateModel()
{
	constant cc;
	Matrix4x4 light_rot;
	light_rot.setIdentity();
	light_rot.setRotationY(rotate_light_y);
	rotate_light_y += 0.707f * deltaTime;

	cc.worldMatrix.setIdentity();
	cc.viewMatrix = view_camera;
	cc.projectionMatrix = projection_camera;
	cc.camera_position = world_camera.getTranslation();
	cc.light_direction = light_rot.getZDirection();

	m_cb->update(GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext(), &cc);
}

void AppWindow::updateSkyBox()
{
	constant cc;

	cc.worldMatrix.setIdentity();
	cc.worldMatrix.setScale(Vector3D(100, 100, 100));
	cc.worldMatrix.setTranslation(world_camera.getTranslation());
	cc.viewMatrix = view_camera;
	cc.projectionMatrix = projection_camera;

	m_sky_cb->update(GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext(), &cc);
}

void AppWindow::drawMesh(const MeshPtr& mesh, const VertexShaderPtr& vs, const PixelShaderPtr& ps, const ConstantBufferPtr& cb, const TexturePtr& texture)
{
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setConstantBuffer(vs, cb);
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setConstantBuffer(ps, cb);

	//SET DEFAULT SHADER IN THE GRAPHICS PIPELINE TO BE ABLE TO DRAW
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setVertexShader(vs);
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setPixelShader(ps);

	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setTexture(vs, texture);
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setTexture(ps, texture);

	//SET THE VERTICES OF THE TRIANGLE TO DRAW
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setVertexBuffer(mesh->getVertexBuffer());
	//SET THE INDICES OF THE TRIANGLE TO DRAW
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setIndexBuffer(mesh->getIndexBuffer());

	// FINALLY DRAW THE TRIANGLE
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->drawIndexedTriangleList(mesh->getIndexBuffer()->getSizeIndexList(), 0, 0);
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
}

void AppWindow::onMouseMove(const Point& mouse_pos)
{
	int width = this->getClientWindowRect().right - this->getClientWindowRect().left;
	int height = this->getClientWindowRect().bottom - this->getClientWindowRect().top;
	rotate_x += 0.3 * (mouse_pos.y - height / 2.0f) * deltaTime;
	rotate_y += 0.3 * (mouse_pos.x - width / 2.0f) * deltaTime;

	InputSystem::get()->setCursorPosition(Point(width / 2, height / 2));
}

void AppWindow::onLeftMouseDown(const Point& mouse_pos)
{
	scale_cube = 0.5f;
}

void AppWindow::onLeftMouseUp(const Point& mouse_pos)
{
	scale_cube = 1.0f;
}

void AppWindow::onRightMouseDown(const Point& mouse_pos)
{
	scale_cube = 2.0f;
}

void AppWindow::onRightMouseUp(const Point& mouse_pos)
{
	scale_cube = 1.0f;
}
