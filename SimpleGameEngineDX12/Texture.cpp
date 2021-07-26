#include "Texture.h"
#include "GraphicsEngine.h"
#include <DirectXTex.h>

Texture::Texture(const wchar_t* fp) : Resource(fp)
{
	DirectX::ScratchImage img_data;
	HRESULT hr = DirectX::LoadFromWICFile(fp, DirectX::WIC_FLAGS_NONE, nullptr, img_data);

	if(SUCCEEDED(hr))
	{
		hr = DirectX::CreateTexture(GraphicsEngine::get()->getRenderSystem()->m_d3d_device, img_data.GetImages(), img_data.GetImageCount(), img_data.GetMetadata(), &texture);
		if(FAILED(hr)) throw std::exception("FAILED TO LOAD IMAGE");

		D3D11_SHADER_RESOURCE_VIEW_DESC rsv_desc = {};
		rsv_desc.Format = img_data.GetMetadata().format;
		rsv_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		rsv_desc.Texture2D.MipLevels = img_data.GetMetadata().mipLevels;
		rsv_desc.Texture2D.MostDetailedMip = 0;

		D3D11_SAMPLER_DESC sampler_desc = {};
		sampler_desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		sampler_desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		sampler_desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		sampler_desc.Filter = D3D11_FILTER_ANISOTROPIC;
		sampler_desc.MinLOD = 0.0f;
		sampler_desc.MaxLOD = (FLOAT)img_data.GetMetadata().mipLevels;

		hr = GraphicsEngine::get()->getRenderSystem()->m_d3d_device->CreateSamplerState(&sampler_desc, &sampler_state);
		if(FAILED(hr)) throw std::exception("FAILED TO CREATE SAMPLER STATE");

		hr = GraphicsEngine::get()->getRenderSystem()->m_d3d_device->CreateShaderResourceView(texture, &rsv_desc, &shader_resource_view);
		if (FAILED(hr)) throw std::exception("FAILED TO CREATE SHADER RESOURCE VIEW");
	}
	else
	{
		throw std::exception("FAILED TO LOAD TEXTURE RESOURCE");
	}
}

Texture::~Texture()
{
	shader_resource_view->Release();
	texture->Release();
}
