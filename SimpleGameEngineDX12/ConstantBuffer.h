#pragma once
#include <d3d11.h>
#include "Prerequisites_Header.h"

class ConstantBuffer
{
public :
	ConstantBuffer(void* buffer, UINT size_buffer, RenderSystem* rs);
	void update(DeviceContext* context, void* buffer);
	~ConstantBuffer();
private:
	ID3D11Buffer* m_buffer;
	RenderSystem* render_system = nullptr;

	friend class DeviceContext;
};

