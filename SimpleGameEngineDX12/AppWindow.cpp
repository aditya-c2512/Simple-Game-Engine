#include "AppWindow.h"
#include "Vector3D.h"
#include "Matrix4x4.h"
#include <Windows.h>

struct vertex
{
	Vector3D position;
	Vector3D position1;
	Vector3D color;
	Vector3D color1;
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
	GraphicsEngine::get()->init();
	m_swap_chain = GraphicsEngine::get()->createSwapChain();

	RECT rc = this->getClientWindowRect();
	m_swap_chain->init(this->m_hwnd, rc.right - rc.left, rc.bottom - rc.top);

	vertex list[] =
	{
		//X1 - Y1 - Z1 - X2 - Y2 - Z2 - R1 - G1 - B1 - R2 - G2 - B2
		{Vector3D(-0.5f, -0.5f, 0.0f),	Vector3D(-0.32f, -0.11f, 0.0f),	Vector3D(0.5f, 0.5f, 0.0f),	Vector3D(0.5f, 1.0f, 0.7f)}, // POS1
		{Vector3D(-0.5f, 0.5f, 0.0f),	Vector3D(-0.11f, 0.78f, 0.0f),	Vector3D(0.0f, 1.0f, 1.0f),	Vector3D(0.5f, 0.3f, 0.0f)}, // POS2
		{Vector3D(0.5f, -0.5f, 0.0f),	Vector3D(0.75f, -0.73f, 0.0f),	Vector3D(0.5f, 0.0f, 0.5f),	Vector3D(0.0f, 0.5f, 1.0f)}, // POS3
		{Vector3D(0.5f, 0.5f, 0.0f),	Vector3D(0.88f, 0.77f, 0.0f),	Vector3D(1.0f, 0.5f, 0.0f),	Vector3D(0.5f, 0.7f, 0.3f)}, // POS4
	};

	m_vb = GraphicsEngine::get()->createVertexBuffer();
	UINT size_list = ARRAYSIZE(list);

	void* shader_byte_code = nullptr;
	size_t size_shader = 0;

	GraphicsEngine::get()->compileVertexShader(L"VertexShader.hlsl", "vsmain", &shader_byte_code, &size_shader);
	m_vertex_shader = GraphicsEngine::get()->createVertexShader(shader_byte_code, size_shader);
	m_vb->load(list, sizeof(vertex), size_list, shader_byte_code, size_shader);
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

	// FINALLY DRAW THE TRIANGLE
	GraphicsEngine::get()->getImmediateDeviceContext()->drawTriangleStrip(m_vb->getSizeVertexList(), 0);
	m_swap_chain->present(true);

	oldDelta = newDelta;
	newDelta = ::GetTickCount();
	deltaTime = (oldDelta)?((newDelta - oldDelta)/1000.0f):0.0f;
}

void AppWindow::onDestroy()
{
	Window::onDestroy();
	m_vb->release();
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

	deltaScale += deltaTime * 3.0f;
	cc.worldMatrix.setScale(Vector3D::lerp(startScale, endScale, (sin(deltaScale)+1.0f)/2.0f));

	temp.setTranslation(Vector3D::lerp(startPos, endPos, deltaPos));
	cc.worldMatrix *= temp;

	cc.viewMatrix.setIdentity();
	cc.projectionMatrix.setOrthoProj(
		(this->getClientWindowRect().right - this->getClientWindowRect().left) / 400.0f,
		(this->getClientWindowRect().bottom - this->getClientWindowRect().top) / 400.0f,
		-4.0f, 4.0f
	);
	m_cb->update(GraphicsEngine::get()->getImmediateDeviceContext(), &cc);
}
