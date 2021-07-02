#include "MeshManager.h"
#include "Mesh.h"

MeshManager::MeshManager()
{
}

MeshManager::~MeshManager()
{
}

MeshPtr MeshManager::createMeshFromFile(const wchar_t* filename)
{
    return std::static_pointer_cast<Mesh>(createResourceFromFile(filename));;
}

Resource* MeshManager::createResourceFromFileConcrete(const wchar_t* filename)
{
	Mesh* mesh = nullptr;
	try
	{
		mesh = new Mesh(filename);
	}
	catch (...)
	{
	}
	return mesh;
}
