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
}
GraphicsEngine::~GraphicsEngine()
{
	GraphicsEngine::graphics_engine = nullptr;
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
