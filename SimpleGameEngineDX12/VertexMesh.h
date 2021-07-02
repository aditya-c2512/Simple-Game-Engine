#pragma once
#include "Vector2D.h"
#include "Vector3D.h"

class VertexMesh
{
public :
	VertexMesh() : position(), texcoord()
	{}
	VertexMesh(Vector3D pos, Vector2D tc) : position(pos), texcoord(tc)
	{}
	VertexMesh(const VertexMesh& vm) : position(vm.position), texcoord(vm.texcoord)
	{}
	~VertexMesh()
	{}

public :
	Vector3D position;
	Vector2D texcoord;
};