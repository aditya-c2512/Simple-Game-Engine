#include "ResourceManager.h"
#include <filesystem>

ResourceManager::ResourceManager()
{
}

ResourceManager::~ResourceManager()
{
}

ResourcePtr ResourceManager::createResourceFromFile(const wchar_t* filename)
{
	std::wstring fullpath = std::filesystem::absolute(filename);

	auto it = resources_map.find(fullpath);
	if (it != resources_map.end())
		return it->second;

	Resource* raw_resource = this->createResourceFromFileConcrete(fullpath.c_str());

	if (raw_resource)
	{
		ResourcePtr resource(raw_resource);
		resources_map[fullpath] = resource;
		return resource;
	}

	return nullptr;
}
