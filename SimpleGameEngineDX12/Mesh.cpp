#define _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS
#include "Mesh.h"
#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>
#include <locale>
#include <codecvt>

//#include "GraphicsEngine.h"
#include "VertexMesh.h"

Mesh::Mesh(const wchar_t* fp) : Resource(fp)
{
	tinyobj::attrib_t attribs;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string warn; std::string err;
	std::string input_fp = std::wstring_convert<std::codecvt_utf8<wchar_t>>().to_bytes(fp);

	bool res = tinyobj::LoadObj(&attribs, &shapes, &materials, &warn, &err, input_fp.c_str());

	if (!err.empty())
	{
		throw std::exception("FAILED TO LOAD MODEL");
	}
	if (!res)
	{
		throw std::exception("FAILED TO LOAD MODEL");
	}
	if (shapes.size() > 1)
	{
		throw std::exception("FAILED TO LOAD MODEL. NUMBER OF SHAPES > 1");
	}

	std::vector<VertexMesh> list_vertices;
	std::vector<unsigned int> list_indices;

	for (size_t s = 0; s < shapes.size(); s++)
	{
		size_t index_offset = 0;
		list_vertices.reserve(shapes[s].mesh.indices.size());
		list_indices.reserve(shapes[s].mesh.indices.size());

		for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++)
		{
			unsigned char num_face_vertices = shapes[s].mesh.num_face_vertices[f];
			for (unsigned char v = 0; v < num_face_vertices; v++)
			{
				tinyobj::index_t index = shapes[s].mesh.indices[index_offset + v];
				tinyobj::real_t vx = attribs.vertices[size_t(index.vertex_index) * 3 + 0];
				tinyobj::real_t vy = attribs.vertices[size_t(index.vertex_index) * 3 + 1];
				tinyobj::real_t vz = attribs.vertices[size_t(index.vertex_index) * 3 + 2];

				tinyobj::real_t tx, ty, nx, ny, nz;
				tx = ty = nx = ny = nz = 0;
				if (index.texcoord_index >= 0)
				{
					tx = attribs.texcoords[size_t(index.texcoord_index) * 2 + 0];
					ty = attribs.texcoords[size_t(index.texcoord_index) * 2 + 1];
				}

				if (index.normal_index >= 0)
				{
					nx = attribs.normals[size_t(index.normal_index) * 3 + 0];
					ny = attribs.normals[size_t(index.normal_index) * 3 + 1];
					nz = attribs.normals[size_t(index.normal_index) * 3 + 2];
				}
				VertexMesh vertex(Vector3D(vx, vy, vz), Vector2D(tx, ty), Vector3D(nx, ny, nz));

				list_vertices.push_back(vertex);
				list_indices.push_back((unsigned int)(index_offset + v));
			}

			index_offset += num_face_vertices;
		}
	}

	void* shader_byte_code = nullptr;
	size_t size_shader = 0;
	GraphicsEngine::get()->getVMLShaderByteCodeAndSize(&shader_byte_code, &size_shader);
	vertex_buffer = GraphicsEngine::get()->getRenderSystem()->createVertexBuffer(&list_vertices[0], sizeof(VertexMesh), (UINT)list_vertices.size(), shader_byte_code, size_shader);
	index_buffer = GraphicsEngine::get()->getRenderSystem()->createIndexBuffer(&list_indices[0], (UINT)list_indices.size());
}

Mesh::~Mesh()
{
}

const VertexBufferPtr& Mesh::getVertexBuffer()
{
	return vertex_buffer;
}

const IndexBufferPtr& Mesh::getIndexBuffer()
{
	return index_buffer;
}

void Mesh::drawMesh(const VertexShaderPtr& vs, const PixelShaderPtr& ps, const ConstantBufferPtr& cb, const TexturePtr* textures, unsigned int num_tex)
{
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setConstantBuffer(vs, cb);
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setConstantBuffer(ps, cb);

	//SET DEFAULT SHADER IN THE GRAPHICS PIPELINE TO BE ABLE TO DRAW
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setVertexShader(vs);
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setPixelShader(ps);

	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setTexture(vs, textures, num_tex);
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setTexture(ps, textures, num_tex);

	//SET THE VERTICES OF THE TRIANGLE TO DRAW
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setVertexBuffer(getVertexBuffer());
	//SET THE INDICES OF THE TRIANGLE TO DRAW
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setIndexBuffer(getIndexBuffer());

	// FINALLY DRAW THE TRIANGLE
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->drawIndexedTriangleList(getIndexBuffer()->getSizeIndexList(), 0, 0);
}
