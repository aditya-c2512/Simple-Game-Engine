#pragma once
#include <d3d11.h>
#include "Prerequisites_Header.h"
#include "RenderSystem.h"

class GraphicsEngine
{
public:
	GraphicsEngine();
	//Initialize the GraphicsEngine and DirectX 11 Device
	bool init();
	//Release all the resources loaded
	bool release();
	~GraphicsEngine();

	RenderSystem* getRenderSystem();
	static GraphicsEngine* get();

private :
	RenderSystem* render_system = nullptr;
};

