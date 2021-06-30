#pragma once
#include <string>

class Resource
{
public :
	Resource(const wchar_t* fp);
	virtual ~Resource();

protected :
	std::wstring fullpath;
};

