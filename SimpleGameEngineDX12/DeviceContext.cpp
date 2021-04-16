#include "DeviceContext.h"
#include "SwapChain.h"

DeviceContext::DeviceContext(ID3D11DeviceContext* d3d_deviceContext)
{
	deviceContext = d3d_deviceContext;
}

bool DeviceContext::clearRenderTarget(SwapChain* swapChain, float r, float g, float b, float a)
{
	FLOAT clearColor[] = { r,g,b,a };
	deviceContext->ClearRenderTargetView(swapChain->renderTargetView, clearColor);
	return true;
}

bool DeviceContext::release()
{
	deviceContext->Release();
	delete this;
	return true;
}

DeviceContext::~DeviceContext()
{
}
