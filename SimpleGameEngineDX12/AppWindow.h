#pragma once
#include "Window.h"
#include "GraphicsEngine.h"
#include "SwapChain.h"
#include "DeviceContext.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ConstantBuffer.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "InputListener.h"
#include "Matrix4x4.h"

class AppWindow : public Window, public InputListener
{
public:
	AppWindow();
	~AppWindow();

	// Inherited via Window
	virtual void onCreate() override;
	virtual void onUpdate() override;
	virtual void onDestroy() override;
	virtual void onFocus() override;
	virtual void onKillFocus() override;

	// Inherited via InputListener
	virtual void onKeyDown(int key) override;
	virtual void onKeyUp(int key) override;

	virtual void onMouseMove(const Point& mouse_pos) override;
	virtual void onLeftMouseDown(const Point& mouse_pos) override;
	virtual void onLeftMouseUp(const Point& mouse_pos) override;
	virtual void onRightMouseDown(const Point& mouse_pos) override;
	virtual void onRightMouseUp(const Point& mouse_pos) override;

	void update();
	void updateCamera();
	void updateModel();
	void updateSkyBox();
	void drawMesh(const MeshPtr& mesh, const VertexShaderPtr& vs, const PixelShaderPtr& ps, const ConstantBufferPtr& cb, const TexturePtr& texture);

private:
	SwapChainPtr m_swap_chain;
	VertexBufferPtr m_vb;
	IndexBufferPtr m_ib;
	ConstantBufferPtr m_cb;
	ConstantBufferPtr m_sky_cb;
	VertexShaderPtr m_vertex_shader;
	PixelShaderPtr m_pixel_shader;
	PixelShaderPtr m_sky_pixel_shader;

	TexturePtr TEX_wood;
	TexturePtr TEX_sky;
	MeshPtr SM_mesh;
	MeshPtr SM_sky_mesh;

	unsigned int oldDelta;
	unsigned int newDelta;
	float deltaTime;

	float rotate_x = 0.0f;
	float rotate_y = 0.0f;
	float scale_cube = 1.0f;
	float move_forward = 0.0f;
	float move_right = 0.0f;
	float rotate_light_y = 0.0f;

	Matrix4x4 world_camera;
	Matrix4x4 view_camera;
	Matrix4x4 projection_camera;
};

