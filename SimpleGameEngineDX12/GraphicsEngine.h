#pragma once
#include <d3d11.h>
#include "Prerequisites_Header.h"
#include "RenderSystem.h"
#include "TextureManager.h"

class GraphicsEngine
{
private:
	GraphicsEngine();
	~GraphicsEngine();

public:
	RenderSystem* getRenderSystem();
	TextureManager* getTextureManager();

	static GraphicsEngine* get();
	static void create();
	static void release();

private :
	RenderSystem* render_system = nullptr;
	TextureManager* texture_manager = nullptr;
	static GraphicsEngine* graphics_engine;
};

