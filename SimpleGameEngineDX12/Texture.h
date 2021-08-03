#pragma once
#include "Resource.h"
#include "Rect.h"

#include <d3d11.h>

class Texture : public Resource
{
public :
	enum TEXTURE_TYPE
	{
		TEXTURE_TYPE_NORMAL = 0,
		TEXTURE_TYPE_RENDER_TARGET,
		TEXTURE_TYPE_DEPTH_STENCIL,
		TEXTURE_TYPE_SHADOW_MAP
	};

public :
	Texture(const wchar_t* fp);
	Texture(const Rect& s, Texture::TEXTURE_TYPE type);
	~Texture();

	Rect getSize();
	Texture::TEXTURE_TYPE getTextureType();

private :
	ID3D11Resource* texture = nullptr;
	ID3D11ShaderResourceView* shader_resource_view = nullptr;
	ID3D11RenderTargetView* render_target_view = nullptr;
	ID3D11DepthStencilView* depth_stencil_view = nullptr;
	ID3D11SamplerState* sampler_state = nullptr;

	Texture::TEXTURE_TYPE texture_type = Texture::TEXTURE_TYPE_NORMAL;
	Rect size;

	friend class DeviceContext;
};

