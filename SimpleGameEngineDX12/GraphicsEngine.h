#pragma once

#include <d3d11.h>

class GraphicsEngine
{
public :
	GraphicsEngine();
	bool init();
	bool release();
	~GraphicsEngine();

	static GraphicsEngine* get();

private :
	ID3D11Device* d3d_device;
	D3D_FEATURE_LEVEL featureLevel;
	ID3D11DeviceContext* d3d_deviceContext;
};

