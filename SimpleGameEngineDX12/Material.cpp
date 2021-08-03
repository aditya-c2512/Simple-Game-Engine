#include "Material.h"
#include "GraphicsEngine.h"

#include <stdexcept>

Material::Material(const wchar_t* vs_path, const wchar_t* ps_path)
{
	void* shader_byte_code = nullptr;
	size_t size_shader = 0;

	GraphicsEngine::get()->getRenderSystem()->compileVertexShader(vs_path, "vsmain", &shader_byte_code, &size_shader);
	vertex_shader = GraphicsEngine::get()->getRenderSystem()->createVertexShader(shader_byte_code, size_shader);
	GraphicsEngine::get()->getRenderSystem()->releaseCompiledShader();

	if (!vertex_shader) throw std::runtime_error("VERTEX SHADER NOT CREATED IN MATERIAL.");

	GraphicsEngine::get()->getRenderSystem()->compilePixelShader(ps_path, "psmain", &shader_byte_code, &size_shader);
	pixel_shader = GraphicsEngine::get()->getRenderSystem()->createPixelShader(shader_byte_code, size_shader);

	if (!pixel_shader) throw std::runtime_error("PIXEL SHADER NOT CREATED IN MATERIAL.");
}

Material::Material(const MaterialPtr& mat)
{
	vertex_shader = mat->vertex_shader;
	pixel_shader = mat->pixel_shader;
}

Material::~Material()
{
}

void Material::addTexture(const TexturePtr& texture)
{
	textures.push_back(texture);
}

void Material::removeTexture(unsigned int index)
{
	if (index >= this->textures.size()) return;

	textures.erase(textures.begin() + index);
}

void Material::setData(void* data, unsigned int size)
{
	if (!constant_buffer)
		constant_buffer = GraphicsEngine::get()->getRenderSystem()->createConstantBuffer(data, size);
	else
		constant_buffer->update(GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext(), data);
}

void Material::setCullMode(CULL_MODE mode)
{
	cull_mode = mode;
}

CULL_MODE Material::getCullMode()
{
	return cull_mode;
}
