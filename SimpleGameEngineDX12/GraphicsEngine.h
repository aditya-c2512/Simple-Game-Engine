#pragma once

#include <d3d11.h>

class SwapChain;
class DeviceContext;

class GraphicsEngine
{
public :
	GraphicsEngine();
	bool init();
	bool release();
	~GraphicsEngine();

	static GraphicsEngine* get();

	SwapChain* createSwapChain();
	DeviceContext* getImmediateDeviceContext();

private :
	ID3D11Device* d3d_device;
	D3D_FEATURE_LEVEL featureLevel;
	IDXGIDevice* dxgi_device;
	IDXGIAdapter* dxgi_adapter;
	IDXGIFactory* dxgi_factory;

	DeviceContext* immediateDeviceContext;

	friend class SwapChain;
};

