#pragma once
#include <d3d11.h>
#include "Prerequisites_Header.h"
#include "RenderSystem.h"

class GraphicsEngine
{
private:
	GraphicsEngine();
	~GraphicsEngine();

public:
	RenderSystem* getRenderSystem();
	static GraphicsEngine* get();
	static void create();
	static void release();

private :
	RenderSystem* render_system = nullptr;
	static GraphicsEngine* graphics_engine;
};

