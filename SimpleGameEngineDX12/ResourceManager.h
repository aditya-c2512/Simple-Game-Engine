#pragma once
#include <unordered_map>
#include <string>
#include "Prerequisites_Header.h"

class ResourceManager
{
public :
	ResourceManager();
	~ResourceManager();

	ResourcePtr createResourceFromFile(const wchar_t* filename);

protected :
	virtual Resource* createResourceFromFileConcrete(const wchar_t* filename) = 0;

private :
	std::unordered_map<std::wstring, ResourcePtr> resources_map;
};