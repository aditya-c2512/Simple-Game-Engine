#include "GraphicsEngine.h"
#include "RenderSystem.h"
#include "DeviceContext.h"

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

MaterialPtr GraphicsEngine::createMaterial(const wchar_t* vs_path, const wchar_t* ps_path)
{
	MaterialPtr mat = nullptr;
	try
	{
		mat = std::make_shared<Material>(vs_path, ps_path);
	}
	catch (...)
	{
	}
	return mat;
}

MaterialPtr GraphicsEngine::createMaterial(const MaterialPtr& mat)
{
	MaterialPtr material = nullptr;
	try
	{
		MaterialPtr material = std::make_shared<Material>(mat);
	}
	catch (...)
	{
	}
	return material;
}

void GraphicsEngine::setMaterial(const MaterialPtr& mat)
{
	GraphicsEngine::get()->getRenderSystem()->setRasterizerState((mat->cull_mode == CULL_MODE_FRONT));

	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setConstantBuffer(mat->vertex_shader, mat->constant_buffer);
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setConstantBuffer(mat->pixel_shader, mat->constant_buffer);

	//SET DEFAULT SHADER IN THE GRAPHICS PIPELINE TO BE ABLE TO DRAW
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setVertexShader(mat->vertex_shader);
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setPixelShader(mat->pixel_shader);

	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setTexture(mat->pixel_shader, &mat->textures[0], mat->textures.size());
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
