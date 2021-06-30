#include "AppWindow.h"
#include "InputSystem.h"
#include "Vector2D.h"
#include "Vector3D.h"
#include "Matrix4x4.h"
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
	unsigned int time;
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

	TEX_wood = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"assets\\Textures\\wall.jpg");

	RECT rc = this->getClientWindowRect();
	m_swap_chain = GraphicsEngine::get()->getRenderSystem()->createSwapChain(this->m_hwnd, rc.right - rc.left, rc.bottom - rc.top);

	Vector3D position_list[] =
	{
		//X1 - Y1 - Z1 
		//FRONT FACE
		Vector3D(-0.5f, -0.5f, -0.5f),	
		Vector3D(-0.5f, 0.5f, -0.5f),	
		Vector3D(0.5f, 0.5f, -0.5f),	
		Vector3D(0.5f, -0.5f, -0.5f),	
		//BACK FACE
		Vector3D(0.5f, -0.5f, 0.5f),	
		Vector3D(0.5f, 0.5f, 0.5f),	
		Vector3D(-0.5f, 0.5f, 0.5f),	
		Vector3D(-0.5f, -0.5f, 0.5f),	
	};
	Vector2D texcoord_list[] =
	{
		//X1 - Y1 - Z1 
		//FRONT FACE
		Vector2D(0,0),
		Vector2D(0,1),
		Vector2D(1,0),
		Vector2D(1,1),
	};

	vertex vertex_list[] =
	{
		{position_list[0],texcoord_list[1]},
		{position_list[1],texcoord_list[0]},
		{position_list[2],texcoord_list[2]},
		{position_list[3],texcoord_list[3]},

		{position_list[4],texcoord_list[1]},
		{position_list[5],texcoord_list[0]},
		{position_list[6],texcoord_list[2]},
		{position_list[7],texcoord_list[3]},

		{position_list[1],texcoord_list[1]},
		{position_list[6],texcoord_list[0]},
		{position_list[5],texcoord_list[2]},
		{position_list[2],texcoord_list[3]},

		{position_list[7],texcoord_list[1]},
		{position_list[0],texcoord_list[0]},
		{position_list[3],texcoord_list[2]},
		{position_list[4],texcoord_list[3]},

		{position_list[3],texcoord_list[1]},
		{position_list[2],texcoord_list[0]},
		{position_list[5],texcoord_list[2]},
		{position_list[4],texcoord_list[3]},

		{position_list[7],texcoord_list[1]},
		{position_list[6],texcoord_list[0]},
		{position_list[1],texcoord_list[2]},
		{position_list[0],texcoord_list[3]},
	};
	UINT size_vertex_list = ARRAYSIZE(vertex_list);

	unsigned int index_list[] =
	{
		//FRONT FACE
		0,1,2,	//TRI 1
		2,3,0,	//TRI 2
		//BACK FACE
		4,5,6,	//TRI 3
		6,7,4,	//TRI 4
		//TOP FACE
		8,9,10,	//TRI 5
		10,11,8,	//TRI 6
		//BOTTOM FACE
		12,13,14,	//TRI 7
		14,15,12,	//TRI 8
		//RIGHT FACE
		16,17,18,	//TRI 9
		18,19,16,	//TRI 10
		//LEFT FACE
		20,21,22,	//TRI 11
		22,23,20 	//TRI 12
	};
	UINT size_index_list = ARRAYSIZE(index_list);
	m_ib = GraphicsEngine::get()->getRenderSystem()->createIndexBuffer(index_list, size_index_list);

	void* shader_byte_code = nullptr;
	size_t size_shader = 0;

	GraphicsEngine::get()->getRenderSystem()->compileVertexShader(L"VertexShader.hlsl", "vsmain", &shader_byte_code, &size_shader);
	m_vertex_shader = GraphicsEngine::get()->getRenderSystem()->createVertexShader(shader_byte_code, size_shader);
	m_vb = GraphicsEngine::get()->getRenderSystem()->createVertexBuffer(vertex_list, sizeof(vertex), size_vertex_list, shader_byte_code, size_shader);
	GraphicsEngine::get()->getRenderSystem()->releaseCompiledShader();

	GraphicsEngine::get()->getRenderSystem()->compilePixelShader(L"PixelShader.hlsl", "psmain", &shader_byte_code, &size_shader);
	m_pixel_shader = GraphicsEngine::get()->getRenderSystem()->createPixelShader(shader_byte_code, size_shader);
	GraphicsEngine::get()->getRenderSystem()->releaseCompiledShader();

	constant cc;
	cc.time = 0;
	m_cb = GraphicsEngine::get()->getRenderSystem()->createConstantBuffer(&cc, sizeof(constant));
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
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setConstantBuffer(m_vertex_shader, m_cb);
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setConstantBuffer(m_pixel_shader, m_cb);

	//SET DEFAULT SHADER IN THE GRAPHICS PIPELINE TO BE ABLE TO DRAW
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setVertexShader(m_vertex_shader);
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setPixelShader(m_pixel_shader);

	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setTexture(m_vertex_shader, TEX_wood);
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setTexture(m_pixel_shader, TEX_wood);

	//SET THE VERTICES OF THE TRIANGLE TO DRAW
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setVertexBuffer(m_vb);
	//SET THE INDICES OF THE TRIANGLE TO DRAW
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setIndexBuffer(m_ib);

	// FINALLY DRAW THE TRIANGLE
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->drawIndexedTriangleList(m_ib->getSizeIndexList(), 0, 0);
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
	constant cc;
	cc.time = newDelta;

	Matrix4x4 temp;

	cc.worldMatrix.setIdentity();
	Matrix4x4 world_cam;
	world_cam.setIdentity();

	temp.setIdentity();
	temp.setRotationX(rotate_x);
	world_cam *= temp;

	temp.setIdentity();
	temp.setRotationY(rotate_y);
	world_cam *= temp;

	Vector3D newPos = world_camera.getTranslation() + world_cam.getZDirection()*move_forward*0.1f + world_cam.getXDirection() * move_right * 0.1f;

	world_cam.setTranslation(newPos);
	world_camera = world_cam;
	world_cam.inverse();

	cc.viewMatrix = world_cam;
	float aspectRatio = (this->getClientWindowRect().right - this->getClientWindowRect().left) / (this->getClientWindowRect().bottom - this->getClientWindowRect().top);
	cc.projectionMatrix.setPerspectiveProj(1.57f, aspectRatio, 0.001f, 1000.0f);

	m_cb->update(GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext(), &cc);
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
