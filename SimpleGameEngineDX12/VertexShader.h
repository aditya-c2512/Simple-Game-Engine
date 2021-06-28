#pragma once

#include <d3d11.h>
#include "Prerequisites_Header.h"

class VertexShader
{
public :
	VertexShader(const void* shader_byte_code, size_t byte_code_size, RenderSystem* rs);
	~VertexShader();

private :
	ID3D11VertexShader* m_vertex_shader;
	RenderSystem* render_system = nullptr;

	friend class RenderSystem;
	friend class DeviceContext;
};

