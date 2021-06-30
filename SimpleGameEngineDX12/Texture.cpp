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
		

		D3D11_SHADER_RESOURCE_VIEW_DESC rsv_desc;
		rsv_desc.Format = img_data.GetMetadata().format;
		rsv_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		rsv_desc.Texture2D.MipLevels = img_data.GetMetadata().mipLevels;
		rsv_desc.Texture2D.MostDetailedMip = 0;

		GraphicsEngine::get()->getRenderSystem()->m_d3d_device->CreateShaderResourceView(texture, &rsv_desc, &shader_resource_view);
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
