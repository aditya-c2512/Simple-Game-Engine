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

Texture::Texture(const Rect& s, Texture::TEXTURE_TYPE type) : Resource(L"")
{
	D3D11_TEXTURE2D_DESC tex_desc = {};
	tex_desc.Width = s.width;
	tex_desc.Height = s.height;

	if (type == TEXTURE_TYPE_NORMAL)
	{
		tex_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		tex_desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	}
	else if (type == TEXTURE_TYPE_RENDER_TARGET)
	{
		tex_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		tex_desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	}
	else if (type == TEXTURE_TYPE_DEPTH_STENCIL)
	{
		tex_desc.Format = DXGI_FORMAT_R24G8_TYPELESS;
		tex_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	}
	else if (type == TEXTURE_TYPE_SHADOW_MAP)
	{
		tex_desc.Format = DXGI_FORMAT_R24G8_TYPELESS;
		tex_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	}

	tex_desc.Usage = D3D11_USAGE_DEFAULT;
	//tex_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	tex_desc.MipLevels = 1;
	tex_desc.SampleDesc.Count = 1;
	tex_desc.SampleDesc.Quality = 0;
	tex_desc.MiscFlags = 0;
	tex_desc.ArraySize = 1;
	tex_desc.CPUAccessFlags = 0;

	HRESULT hr = GraphicsEngine::get()->getRenderSystem()->m_d3d_device->CreateTexture2D(&tex_desc, nullptr, (ID3D11Texture2D**)(&texture));
	if (FAILED(hr))
	{
		throw std::exception("FAILED TO CREATE FRAMEBUFFER TEXTURE");
	}

	if (type == TEXTURE_TYPE_NORMAL || type == TEXTURE_TYPE_RENDER_TARGET)
	{
		hr = GraphicsEngine::get()->getRenderSystem()->m_d3d_device->CreateShaderResourceView(texture, NULL, &shader_resource_view);
		if (FAILED(hr))
		{
			throw std::exception("FAILED TO CREATE DEPTH STENCIL VIEW");
		}
	}
	if (type == TEXTURE_TYPE_RENDER_TARGET)
	{
		hr = GraphicsEngine::get()->getRenderSystem()->m_d3d_device->CreateRenderTargetView(texture, NULL, &render_target_view);
		if (FAILED(hr))
		{
			throw std::exception("FAILED TO CREATE DEPTH STENCIL VIEW");
		}
	}
	else if (type == TEXTURE_TYPE_DEPTH_STENCIL)
	{
		D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
		depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		depthStencilViewDesc.Texture2D.MipSlice = 0;
		depthStencilViewDesc.Flags = 0;

		hr = GraphicsEngine::get()->getRenderSystem()->m_d3d_device->CreateDepthStencilView(texture, &depthStencilViewDesc, &depth_stencil_view);
		if (FAILED(hr))
		{
			throw std::exception("FAILED TO CREATE DEPTH STENCIL VIEW");
		}

		D3D11_SAMPLER_DESC sampler_desc = {};
		sampler_desc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
		sampler_desc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
		sampler_desc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
		sampler_desc.BorderColor[0] = 1.0f;
		sampler_desc.BorderColor[1] = 1.0f;
		sampler_desc.BorderColor[2] = 1.0f;
		sampler_desc.BorderColor[3] = 1.0f;
		sampler_desc.Filter = D3D11_FILTER_ANISOTROPIC;

		hr = GraphicsEngine::get()->getRenderSystem()->m_d3d_device->CreateSamplerState(&sampler_desc, &sampler_state);
		if (FAILED(hr)) throw std::exception("FAILED TO CREATE SAMPLER STATE");

		D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
		shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		shaderResourceViewDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
		shaderResourceViewDesc.Texture2D.MipLevels = 1;
		shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;

		hr = GraphicsEngine::get()->getRenderSystem()->m_d3d_device->CreateShaderResourceView(texture, &shaderResourceViewDesc, &shader_resource_view);
		if (FAILED(hr))
		{
			throw std::exception("FAILED TO CREATE DEPTH STENCIL VIEW");
		}
	}
	else if (type == TEXTURE_TYPE_SHADOW_MAP)
	{
		hr = GraphicsEngine::get()->getRenderSystem()->m_d3d_device->CreateDepthStencilView(texture, NULL, &depth_stencil_view);
		if (FAILED(hr))
		{
			throw std::exception("FAILED TO CREATE DEPTH STENCIL VIEW");
		}
	}

	texture_type = type;
	size = s;
}

Texture::~Texture()
{
	if(render_target_view) render_target_view->Release();
	if(depth_stencil_view) depth_stencil_view->Release();
	if(sampler_state) sampler_state->Release();
	if(shader_resource_view) shader_resource_view->Release();
	if(texture) texture->Release();
}

Rect Texture::getSize()
{
	return size;
}

Texture::TEXTURE_TYPE Texture::getTextureType()
{
	return texture_type;
}
