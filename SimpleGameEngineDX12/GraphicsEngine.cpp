#include "GraphicsEngine.h"
#include "RenderSystem.h"

GraphicsEngine::GraphicsEngine()
{
}

bool GraphicsEngine::init()
{
	render_system = new RenderSystem();
	render_system->init();
	return true;
}


bool GraphicsEngine::release()
{
	render_system->release();
	delete render_system;
	return true;
}

GraphicsEngine::~GraphicsEngine()
{
}

RenderSystem* GraphicsEngine::getRenderSystem()
{
	return render_system;
}

GraphicsEngine* GraphicsEngine::get()
{
	static GraphicsEngine engine;
	return &engine;
}
