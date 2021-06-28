#pragma once
#include <d3d11.h>
#include "Prerequisites_Header.h"

class IndexBuffer
{
public:
	IndexBuffer(void* list_indices, UINT size_list, RenderSystem* rs);
	UINT getSizeIndexList();
	~IndexBuffer();

private:
	UINT m_size_list;

private:
	ID3D11Buffer* m_buffer;
	RenderSystem* render_system = nullptr;

private:
	friend class DeviceContext;
};