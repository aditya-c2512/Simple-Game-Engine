#pragma once
#include <d3d11.h>
#pragma comment(lib,"d3d11.lib")
#include "Prerequisites_Header.h"

class RenderSystem
{
public:
	RenderSystem();
	//Initialize the GraphicsEngine and DirectX 11 Device
	bool init();
	//Release all the resources loaded
	bool release();
	~RenderSystem();

	SwapChain* createSwapChain(HWND hwnd, UINT width, UINT height);
	DeviceContext* getImmediateDeviceContext();
	VertexBuffer* createVertexBuffer(void* list_vertices, UINT size_vertex, UINT size_list, void* shader_byte_code, size_t size_byte_shader);
	IndexBuffer* createIndexBuffer(void* list_indices, UINT size_list);
	ConstantBuffer* createConstantBuffer(void* buffer, UINT size_buffer);
	VertexShader* createVertexShader(const void* shader_byte_code, size_t byte_code_size);
	PixelShader* createPixelShader(const void* shader_byte_code, size_t byte_code_size);

	bool compileVertexShader(const wchar_t* filename, const char* entrypoint_name, void** shader_byte_code, size_t* byte_code_size);
	bool compilePixelShader(const wchar_t* filename, const char* entrypoint_name, void** shader_byte_code, size_t* byte_code_size);
	void releaseCompiledShader();

private:
	DeviceContext* m_imm_device_context;
	ID3D11Device* m_d3d_device;
	D3D_FEATURE_LEVEL m_feature_level;

	IDXGIDevice* m_dxgi_device;
	IDXGIAdapter* m_dxgi_adapter;
	IDXGIFactory* m_dxgi_factory;
	ID3D11DeviceContext* m_imm_context;

	ID3DBlob* m_blob = nullptr;

	friend class SwapChain;
	friend class VertexBuffer;
	friend class IndexBuffer;
	friend class ConstantBuffer;
	friend class VertexShader;
	friend class PixelShader;
};

