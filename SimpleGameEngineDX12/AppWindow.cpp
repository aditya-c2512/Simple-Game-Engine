#include "AppWindow.h"
#include "Vector3D.h"
#include "Matrix4x4.h"
#include <Windows.h>

struct vertex
{
	Vector3D position;
	Vector3D color;
	Vector3D color1;
};

__declspec(align(16))
struct constant
{
	Matrix4x4 worldMatrix;
	Matrix4x4 viewMatrix;
	Matrix4x4 projectionMatrix;
	DWORD time;
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
	GraphicsEngine::get()->init();
	m_swap_chain = GraphicsEngine::get()->createSwapChain();

	RECT rc = this->getClientWindowRect();
	m_swap_chain->init(this->m_hwnd, rc.right - rc.left, rc.bottom - rc.top);

	vertex vertex_list[] =
	{
		//X1 - Y1 - Z1 - R1 - G1 - B1 - R2 - G2 - B2
		//FRONT FACE
		{Vector3D(-0.5f, -0.5f, -0.5f),	Vector3D(1, 0, 0),	Vector3D(0.2f, 0, 0)}, // POS1
		{Vector3D(-0.5f, 0.5f, -0.5f),	Vector3D(1, 1, 0),	Vector3D(0.2f, 0.2f, 0)}, // POS2
		{Vector3D(0.5f, 0.5f, -0.5f),	Vector3D(1, 1, 0),	Vector3D(0.2f, 0.2f, 0)}, // POS3
		{Vector3D(0.5f, -0.5f, -0.5f),	Vector3D(1, 0, 0),	Vector3D(0.2f, 0, 0)}, // POS4
		//BACK FACE
		{Vector3D(0.5f, -0.5f, 0.5f),	Vector3D(0, 1, 0),	Vector3D(0, 0.2f, 0)}, // POS5
		{Vector3D(0.5f, 0.5f, 0.5f),	Vector3D(0, 1, 1),	Vector3D(0, 0.2f, 0.2f)},	// POS6
		{Vector3D(-0.5f, 0.5f, 0.5f),	Vector3D(0, 1, 1),	Vector3D(1, 0.2f, 0.2f)},	// POS7
		{Vector3D(-0.5f, -0.5f, 0.5f),	Vector3D(0, 1, 0),	Vector3D(0, 0.2f, 0.2f)},	// POS8
	};

	m_vb = GraphicsEngine::get()->createVertexBuffer();
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
		1,6,5,	//TRI 5
		5,2,1,	//TRI 6
		//BOTTOM FACE
		7,0,3,	//TRI 7
		3,4,7,	//TRI 8
		//RIGHT FACE
		3,2,5,	//TRI 9
		5,4,3,	//TRI 10
		//LEFT FACE
		7,6,1,	//TRI 11
		1,0,7 	//TRI 12
	};

	m_ib = GraphicsEngine::get()->createIndexBuffer();
	UINT size_index_list = ARRAYSIZE(index_list);

	m_ib->load(index_list, size_index_list);

	void* shader_byte_code = nullptr;
	size_t size_shader = 0;

	GraphicsEngine::get()->compileVertexShader(L"VertexShader.hlsl", "vsmain", &shader_byte_code, &size_shader);
	m_vertex_shader = GraphicsEngine::get()->createVertexShader(shader_byte_code, size_shader);
	m_vb->load(vertex_list, sizeof(vertex), size_vertex_list, shader_byte_code, size_shader);
	GraphicsEngine::get()->releaseCompiledShader();

	GraphicsEngine::get()->compilePixelShader(L"PixelShader.hlsl", "psmain", &shader_byte_code, &size_shader);
	m_pixel_shader = GraphicsEngine::get()->createPixelShader(shader_byte_code, size_shader);
	GraphicsEngine::get()->releaseCompiledShader();

	constant cc;
	cc.time = 0;
	m_cb = GraphicsEngine::get()->createConstantBuffer();
	m_cb->load(&cc, sizeof(constant));
}

void AppWindow::onUpdate()
{
	Window::onUpdate();
	//CLEAR THE RENDER TARGET 
	GraphicsEngine::get()->getImmediateDeviceContext()->clearRenderTargetColor(this->m_swap_chain, 0.3f, 0.4f, 0.5f, 1);
	//SET VIEWPORT OF RENDER TARGET IN WHICH WE HAVE TO DRAW
	RECT rc = this->getClientWindowRect();
	GraphicsEngine::get()->getImmediateDeviceContext()->setViewportSize(rc.right - rc.left, rc.bottom - rc.top);

	updateQuadPosition();
	GraphicsEngine::get()->getImmediateDeviceContext()->setConstantBuffer(m_vertex_shader, m_cb);
	GraphicsEngine::get()->getImmediateDeviceContext()->setConstantBuffer(m_pixel_shader, m_cb);

	//SET DEFAULT SHADER IN THE GRAPHICS PIPELINE TO BE ABLE TO DRAW
	GraphicsEngine::get()->getImmediateDeviceContext()->setVertexShader(m_vertex_shader);
	GraphicsEngine::get()->getImmediateDeviceContext()->setPixelShader(m_pixel_shader);
	//SET THE VERTICES OF THE TRIANGLE TO DRAW
	GraphicsEngine::get()->getImmediateDeviceContext()->setVertexBuffer(m_vb);
	//SET THE INDICES OF THE TRIANGLE TO DRAW
	GraphicsEngine::get()->getImmediateDeviceContext()->setIndexBuffer(m_ib);

	// FINALLY DRAW THE TRIANGLE
	GraphicsEngine::get()->getImmediateDeviceContext()->drawIndexedTriangleList(m_ib->getSizeIndexList(), 0, 0);
	m_swap_chain->present(true);

	oldDelta = newDelta;
	newDelta = ::GetTickCount();
	deltaTime = (oldDelta)?((newDelta - oldDelta)/1000.0f):0.0f;
}

void AppWindow::onDestroy()
{
	Window::onDestroy();
	m_vb->release();
	m_ib->release();
	m_cb->release();
	m_swap_chain->release();
	m_vertex_shader->release();
	m_pixel_shader->release();
	GraphicsEngine::get()->release();
}

void AppWindow::updateQuadPosition()
{
	constant cc;
	cc.time = newDelta;
	Vector3D startPos(-1.5, -1.5, 0);
	Vector3D endPos(1.5, 1.5, 0);
	Vector3D startScale(0.5, 0.5, 0);
	Vector3D endScale(1, 1, 0);

	deltaPos += deltaTime * 0.3f;
	if (deltaPos > 1.0f) deltaPos = 0.0f;

	Matrix4x4 temp;

	deltaScale += deltaTime / 0.50f;
	//cc.worldMatrix.setScale(Vector3D::lerp(startScale, endScale, (sin(deltaScale)+1.0f)/2.0f));
	
	//temp.setTranslation(Vector3D::lerp(startPos, endPos, deltaPos));
	//cc.worldMatrix *= temp;

	cc.worldMatrix.setScale(Vector3D(1, 1, 1));
	temp.setRotationZ(deltaScale);
	cc.worldMatrix *= temp;
	temp.setRotationY(deltaScale);
	cc.worldMatrix *= temp;
	temp.setRotationX(deltaScale);
	cc.worldMatrix *= temp;

	cc.viewMatrix.setIdentity();
	cc.projectionMatrix.setOrthoProj(
		(this->getClientWindowRect().right - this->getClientWindowRect().left) / 400.0f,
		(this->getClientWindowRect().bottom - this->getClientWindowRect().top) / 400.0f,
		-4.0f, 4.0f
	);
	m_cb->update(GraphicsEngine::get()->getImmediateDeviceContext(), &cc);
}
