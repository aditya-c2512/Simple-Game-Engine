#pragma once
#include <d3d11.h>
#include "Prerequisites_Header.h"

class VertexBuffer
{
public:
	VertexBuffer(void* list_vertices, UINT size_vertex, UINT size_list, void* shader_byte_code, size_t size_byte_shader, RenderSystem* rs);

	UINT getSizeVertexList();

	~VertexBuffer();
private:
	UINT m_size_vertex;
	UINT m_size_list;
private:
	ID3D11Buffer* m_buffer;
	ID3D11InputLayout* m_layout;
	RenderSystem* render_system = nullptr;

private:
	friend class DeviceContext;
};