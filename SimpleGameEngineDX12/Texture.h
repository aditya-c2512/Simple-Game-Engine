#pragma once
#include "Resource.h"
#include <d3d11.h>

class Texture : public Resource
{
public :
	Texture(const wchar_t* fp);
	~Texture();

private :
	ID3D11Resource* texture = nullptr;
	ID3D11ShaderResourceView* shader_resource_view = nullptr;
	ID3D11SamplerState* sampler_state = nullptr;

	friend class DeviceContext;
};

