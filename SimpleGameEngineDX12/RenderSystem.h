#pragma once
#include <d3d11.h>
#pragma comment(lib,"d3d11.lib")
#include "Prerequisites_Header.h"

class RenderSystem
{
public:
	RenderSystem();
	~RenderSystem();

	SwapChainPtr createSwapChain(HWND hwnd, UINT width, UINT height);
	DeviceContextPtr getImmediateDeviceContext();
	VertexBufferPtr createVertexBuffer(void* list_vertices, UINT size_vertex, UINT size_list, void* shader_byte_code, size_t size_byte_shader);
	IndexBufferPtr createIndexBuffer(void* list_indices, UINT size_list);
	ConstantBufferPtr createConstantBuffer(void* buffer, UINT size_buffer);
	VertexShaderPtr createVertexShader(const void* shader_byte_code, size_t byte_code_size);
	PixelShaderPtr createPixelShader(const void* shader_byte_code, size_t byte_code_size);

	bool compileVertexShader(const wchar_t* filename, const char* entrypoint_name, void** shader_byte_code, size_t* byte_code_size);
	bool compilePixelShader(const wchar_t* filename, const char* entrypoint_name, void** shader_byte_code, size_t* byte_code_size);
	void releaseCompiledShader();

private:
	DeviceContextPtr m_imm_device_context;
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

