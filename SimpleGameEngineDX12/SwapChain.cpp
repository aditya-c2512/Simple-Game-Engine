#include "SwapChain.h"
#include "GraphicsEngine.h"

SwapChain::SwapChain()
{
}

bool SwapChain::init(HWND hwnd, UINT width, UINT height)
{
	ID3D11Device* d3d_device = GraphicsEngine::get()->d3d_device;

	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Width = width;
	swapChainDesc.BufferDesc.Height = height;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = hwnd;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.Windowed = TRUE;

	//CREATE SWAP CHAIN
	HRESULT result = GraphicsEngine::get()->dxgi_factory->CreateSwapChain(d3d_device, &swapChainDesc, &swapChain);
	if (FAILED(result)) return false;

	ID3D11Texture2D* buffer;
	result = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&buffer);
	if (FAILED(result)) return false;

	result = d3d_device->CreateRenderTargetView(buffer, NULL, &renderTargetView);
	buffer->Release();
	if (FAILED(result)) return false;

	return true;
}

bool SwapChain::release()
{
	swapChain->Release();
	delete this;
	return true;
}

SwapChain::~SwapChain()
{
}

bool SwapChain::present(bool vsync)
{
	swapChain->Present(vsync, NULL);
	return true;
}
