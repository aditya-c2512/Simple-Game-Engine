#include "GraphicsEngine.h"
#include "SwapChain.h"
#include "DeviceContext.h"

GraphicsEngine::GraphicsEngine()
{
}

bool GraphicsEngine::init()
{
	D3D_DRIVER_TYPE driverTypes[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE
	};
	UINT dtSize = ARRAYSIZE(driverTypes);

	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_0
	};
	UINT flSize = ARRAYSIZE(featureLevels);

	HRESULT result = 0;

	ID3D11DeviceContext* d3d_deviceContext;
	for(UINT dtIndex = 0; dtIndex < dtSize; dtSize++)
	{
		result = D3D11CreateDevice(NULL, driverTypes[dtIndex], NULL, NULL, featureLevels, flSize, D3D11_SDK_VERSION, &d3d_device, &featureLevel, &d3d_deviceContext);
		if (SUCCEEDED(result))
		{
			break;
		}
	}

	if (FAILED(result)) return false;

	immediateDeviceContext = new DeviceContext(d3d_deviceContext);

	//SWAP CHAIN FACTORY CREATION
	d3d_device->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgi_device);
	dxgi_device->GetParent(__uuidof(IDXGIAdapter), (void**)&dxgi_adapter);
	dxgi_adapter->GetParent(__uuidof(IDXGIFactory), (void**)&dxgi_factory);

	return true;
}

bool GraphicsEngine::release()
{
	dxgi_device->Release();
	dxgi_adapter->Release();
	dxgi_factory->Release();

	immediateDeviceContext->release();
	d3d_device->Release();
	return true;
}

GraphicsEngine::~GraphicsEngine()
{
}

GraphicsEngine* GraphicsEngine::get()
{
	static GraphicsEngine graphicsEngine;
	return &graphicsEngine;
}

SwapChain* GraphicsEngine::createSwapChain()
{
	return new SwapChain();
}

DeviceContext* GraphicsEngine::getImmediateDeviceContext()
{
	return this->immediateDeviceContext;
}
