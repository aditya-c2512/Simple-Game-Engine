#pragma once
#include "ResourceManager.h"

class TextureManager : public ResourceManager
{
public :
	TextureManager();
	~TextureManager();

	TexturePtr createTextureFromFile(const wchar_t* filename);

protected:
	virtual Resource* createResourceFromFileConcrete(const wchar_t* filename);
};

