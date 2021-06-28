#pragma once

#include <d3d11.h>
#include "Prerequisites_Header.h"

class PixelShader
{
public:
	PixelShader(const void* shader_byte_code, size_t byte_code_size, RenderSystem* rs);
	~PixelShader();

private:
	ID3D11PixelShader* m_pixel_shader;
	RenderSystem* render_system = nullptr;

	friend class RenderSystem;
	friend class DeviceContext;
};