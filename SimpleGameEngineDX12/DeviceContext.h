#pragma once

#include <d3d11.h>

class SwapChain;

class DeviceContext
{
public :
	DeviceContext(ID3D11DeviceContext* d3d_deviceContext);
	bool clearRenderTarget(SwapChain* swapChain, float r, float g, float b, float a);
	bool release();
	~DeviceContext();

private :
	ID3D11DeviceContext* deviceContext;
};

