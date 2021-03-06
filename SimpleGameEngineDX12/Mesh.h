#pragma once
#include "GraphicsEngine.h"
#include "DeviceContext.h"
#include "ConstantBuffer.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "Resource.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

class Mesh : public Resource
{
public :
	Mesh(const wchar_t* fp);
	~Mesh();

	const VertexBufferPtr& getVertexBuffer();
	const IndexBufferPtr& getIndexBuffer();

	void drawMesh(const MaterialPtr& material);

private :
	VertexBufferPtr vertex_buffer;
	IndexBufferPtr index_buffer;

	friend class DeviceContext;
};

