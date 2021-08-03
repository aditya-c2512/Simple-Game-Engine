#pragma once
#include "VertexShader.h"
#include "PixelShader.h"
#include "ConstantBuffer.h"

#include <vector>

enum CULL_MODE
{
	CULL_MODE_FRONT=0,
	CULL_MODE_BACK
};

class GraphicsEngine;

class Material
{
public :
	Material(const wchar_t* vs_path, const wchar_t* ps_path);
	Material(const MaterialPtr& mat);
	~Material();

	void addTexture(const TexturePtr& texture);
	void removeTexture(unsigned int index);

	void setData(void* data, unsigned int size);

	void setCullMode(CULL_MODE mode);
	CULL_MODE getCullMode();

private :
	VertexShaderPtr vertex_shader;
	PixelShaderPtr pixel_shader;
	ConstantBufferPtr constant_buffer;
	std::vector<TexturePtr> textures;

	CULL_MODE cull_mode = CULL_MODE_BACK;

public :
	friend class GraphicsEngine;
};

