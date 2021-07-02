#pragma once
#include <d3d11.h>
#include "Prerequisites_Header.h"
#include "RenderSystem.h"
#include "TextureManager.h"
#include "MeshManager.h"

class GraphicsEngine
{
private:
	GraphicsEngine();
	~GraphicsEngine();

public:
	RenderSystem* getRenderSystem();
	TextureManager* getTextureManager();
	MeshManager* getMeshManager();

	void getVMLShaderByteCodeAndSize(void** byte_code, size_t* size);

	static GraphicsEngine* get();
	static void create();
	static void release();

private :
	RenderSystem* render_system = nullptr;
	TextureManager* texture_manager = nullptr;
	MeshManager* mesh_manager = nullptr;
	static GraphicsEngine* graphics_engine;

	unsigned char mesh_layout_byte_code[1024];
	size_t mesh_layout_size = 0;
};

