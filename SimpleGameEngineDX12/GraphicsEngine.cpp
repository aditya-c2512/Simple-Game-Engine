#include "GraphicsEngine.h"
#include "RenderSystem.h"

GraphicsEngine* GraphicsEngine::graphics_engine = nullptr;

GraphicsEngine::GraphicsEngine()
{
	try
	{
		render_system = new RenderSystem();
	}
	catch (...)
	{
		throw std::exception("FAILED TO INITIALIZE GRAPHICS ENGINE");
	}

	try
	{
		texture_manager = new TextureManager();
	}
	catch (...)
	{
		throw std::exception("FAILED TO INITIALIZE TEXTURE MANAGER");
	}

	try
	{
		mesh_manager = new MeshManager();
	}
	catch (...)
	{
		throw std::exception("FAILED TO INITIALIZE MESH MANAGER");
	}

	void* shader_byte_code = nullptr;
	size_t size_shader = 0;

	render_system->compileVertexShader(L"VertexMeshLayout.hlsl", "vsmain", &shader_byte_code, &size_shader);
	::memcpy(mesh_layout_byte_code, shader_byte_code, size_shader);
	mesh_layout_size = size_shader;
	render_system->releaseCompiledShader();
}
GraphicsEngine::~GraphicsEngine()
{
	GraphicsEngine::graphics_engine = nullptr;
	delete mesh_manager;
	delete texture_manager;
	delete render_system;
}

RenderSystem* GraphicsEngine::getRenderSystem()
{
	return render_system;
}

TextureManager* GraphicsEngine::getTextureManager()
{
	return texture_manager;
}

MeshManager* GraphicsEngine::getMeshManager()
{
	return mesh_manager;
}

void GraphicsEngine::getVMLShaderByteCodeAndSize(void** byte_code, size_t* size)
{
	*byte_code = mesh_layout_byte_code;
	*size = mesh_layout_size;
}

GraphicsEngine* GraphicsEngine::get()
{
	return graphics_engine;
}

void GraphicsEngine::create()
{
	if (GraphicsEngine::graphics_engine) throw std::exception("ONE INSTANCE OF GRAPHICS ENGINE ALREADY EXISTS");
	GraphicsEngine::graphics_engine = new GraphicsEngine();
}

void GraphicsEngine::release()
{
	if (!GraphicsEngine::graphics_engine) throw std::exception("GRAPHICS ENGINE ALREADY RELEASED");
	delete GraphicsEngine::graphics_engine;
}
