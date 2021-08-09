#include "DeviceContext.h"
#include "SwapChain.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ConstantBuffer.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "Texture.h"
#include <exception>

DeviceContext::DeviceContext(ID3D11DeviceContext* device_context, RenderSystem* rs) :m_device_context(device_context), render_system(rs)
{
}

void DeviceContext::clearRenderTargetColor(const SwapChainPtr& swap_chain, float red, float green, float blue, float alpha)
{
	FLOAT clear_color[] = { red,green,blue,alpha };
	m_device_context->ClearRenderTargetView(swap_chain->m_rtv, clear_color);
	m_device_context->ClearDepthStencilView(swap_chain->m_dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);
	m_device_context->OMSetRenderTargets(1, &swap_chain->m_rtv, swap_chain->m_dsv);
}

void DeviceContext::clearDepthStencil(const SwapChainPtr& swap_chain)
{
	m_device_context->ClearDepthStencilView(swap_chain->m_dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);
}

void DeviceContext::clearRenderTargetColor(const TexturePtr& render_target, float red, float green, float blue, float alpha)
{
	if (render_target->texture_type != Texture::TEXTURE_TYPE::TEXTURE_TYPE_RENDER_TARGET) return;

	FLOAT clear_color[] = { red,green,blue,alpha };
	m_device_context->ClearRenderTargetView(render_target->render_target_view, clear_color);
}

void DeviceContext::clearDepthStencil(const TexturePtr& depth_stencil)
{
	//if (depth_stencil->texture_type != Texture::TEXTURE_TYPE::TEXTURE_TYPE_DEPTH_STENCIL) return;

	m_device_context->ClearDepthStencilView(depth_stencil->depth_stencil_view, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void DeviceContext::SetRenderTarget(const SwapChainPtr& swap_chain)
{
	m_device_context->OMSetRenderTargets(1, &swap_chain->m_rtv, swap_chain->m_dsv);
}

void DeviceContext::SetRenderTarget(const TexturePtr& render_target, const TexturePtr& depth_stencil)
{
	//if (render_target->texture_type != Texture::TEXTURE_TYPE::TEXTURE_TYPE_RENDER_TARGET) return;
	//if (depth_stencil->texture_type != Texture::TEXTURE_TYPE::TEXTURE_TYPE_DEPTH_STENCIL) return;
	if (render_target == NULL && depth_stencil == NULL)
	{
		m_device_context->OMSetRenderTargets(1, NULL, NULL);
	}
	else m_device_context->OMSetRenderTargets(1, &render_target->render_target_view, depth_stencil->depth_stencil_view);
}

void DeviceContext::setVertexBuffer(const VertexBufferPtr& vertex_buffer)
{
	UINT stride = vertex_buffer->m_size_vertex;
	UINT offset = 0;
	m_device_context->IASetVertexBuffers(0, 1, &vertex_buffer->m_buffer, &stride, &offset);
	m_device_context->IASetInputLayout(vertex_buffer->m_layout);
}

void DeviceContext::setIndexBuffer(const IndexBufferPtr& index_buffer)
{
	m_device_context->IASetIndexBuffer(index_buffer->m_buffer, DXGI_FORMAT_R32_UINT, 0);
}

void DeviceContext::drawTriangleList(UINT vertex_count, UINT start_vertex_index)
{
	m_device_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_device_context->Draw(vertex_count, start_vertex_index);
}

void DeviceContext::drawIndexedTriangleList(UINT index_count, UINT start_vertex_index, UINT start_index_location)
{
	m_device_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_device_context->DrawIndexed(index_count,start_index_location,start_vertex_index);
}

void DeviceContext::drawTriangleStrip(UINT vertex_count, UINT start_vertex_index)
{
	m_device_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	m_device_context->Draw(vertex_count, start_vertex_index);
}

void DeviceContext::setViewportSize(UINT width, UINT height)
{
	D3D11_VIEWPORT vp = {};
	vp.Width = (FLOAT)width;
	vp.Height = (FLOAT)height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	m_device_context->RSSetViewports(1, &vp);
}

void DeviceContext::setVertexShader(const VertexShaderPtr& vertex_shader)
{
	m_device_context->VSSetShader(vertex_shader->m_vertex_shader, nullptr, 0);
}

void DeviceContext::setPixelShader(const PixelShaderPtr& pixel_shader)
{
	m_device_context->PSSetShader(pixel_shader->m_pixel_shader, nullptr, 0);
}

void DeviceContext::setConstantBuffer(const VertexShaderPtr& vertex_shader, const ConstantBufferPtr& buffer)
{
	m_device_context->VSSetConstantBuffers(0, 1, &buffer->m_buffer);
}

void DeviceContext::setConstantBuffer(const PixelShaderPtr& pixel_shader, const ConstantBufferPtr& buffer)
{
	m_device_context->PSSetConstantBuffers(0, 1, &buffer->m_buffer);
}

void DeviceContext::setTexture(const VertexShaderPtr& vertex_shader, const TexturePtr* textures, unsigned int num_tex)
{
	ID3D11ShaderResourceView* res_views[32];
	ID3D11SamplerState* samplers[32];
	for (unsigned int i = 0; i < num_tex; i++)
	{
		res_views[i] = textures[i]->shader_resource_view;
		samplers[i] = textures[i]->sampler_state;
	}

	m_device_context->VSSetShaderResources(0, num_tex, res_views);
	m_device_context->VSSetSamplers(0, num_tex, samplers);
}

void DeviceContext::setTexture(const PixelShaderPtr& pixel_shader, const TexturePtr* textures, unsigned int num_tex)
{
	ID3D11ShaderResourceView* res_views[32];
	ID3D11SamplerState* samplers[32];
	for (unsigned int i = 0; i < num_tex; i++)
	{
		res_views[i] = textures[i]->shader_resource_view;
		samplers[i] = textures[i]->sampler_state;
	}

	m_device_context->PSSetShaderResources(0, num_tex, res_views);
	m_device_context->PSSetSamplers(0, num_tex, samplers);
}

void DeviceContext::unbindSRV()
{
	ID3D11ShaderResourceView* nullSRV[1] = { nullptr };
	m_device_context->PSSetShaderResources(0, 1, nullSRV);
}

DeviceContext::~DeviceContext()
{
	m_device_context->Release();
}
