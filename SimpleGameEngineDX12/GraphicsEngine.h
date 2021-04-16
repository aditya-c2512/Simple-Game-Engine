#pragma once
#include <d3d11.h>

class SwapChain;
class DeviceContext;
class VertexBuffer;

class GraphicsEngine
{
public:
	GraphicsEngine();
	//Initialize the GraphicsEngine and DirectX 11 Device
	bool init();
	//Release all the resources loaded
	bool release();
	~GraphicsEngine();

	SwapChain* createSwapChain();
	DeviceContext* getImmediateDeviceContext();
	VertexBuffer* createVertexBuffer();

	bool createShaders();
	bool setShaders();
	void getShaderBufferAndSize(void** bytecode, UINT* size);

	static GraphicsEngine* get();

private:
	DeviceContext* m_imm_device_context;
	ID3D11Device* m_d3d_device;
	D3D_FEATURE_LEVEL m_feature_level;

	IDXGIDevice* m_dxgi_device;
	IDXGIAdapter* m_dxgi_adapter;
	IDXGIFactory* m_dxgi_factory;
	ID3D11DeviceContext* m_imm_context;

	ID3DBlob* m_vsblob = nullptr;
	ID3DBlob* m_psblob = nullptr;
	ID3D11VertexShader* m_vs = nullptr;
	ID3D11PixelShader* m_ps = nullptr;

	friend class SwapChain;
	friend class VertexBuffer;
};

