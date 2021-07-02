#pragma once
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

private :
	VertexBufferPtr vertex_buffer;
	IndexBufferPtr index_buffer;

	friend class DeviceContext;
};

