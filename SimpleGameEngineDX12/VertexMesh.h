#pragma once
#include "Vector2D.h"
#include "Vector3D.h"

class VertexMesh
{
public :
	VertexMesh() : position(), texcoord(), normal()
	{}
	VertexMesh(Vector3D pos, Vector2D tc, Vector3D n) : position(pos), texcoord(tc), normal(n)
	{}
	VertexMesh(const VertexMesh& vm) : position(vm.position), texcoord(vm.texcoord), normal(vm.normal)
	{}
	~VertexMesh()
	{}

public :
	Vector3D position;
	Vector2D texcoord;
	Vector3D normal;
};