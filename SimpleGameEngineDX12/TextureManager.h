#pragma once
#include "ResourceManager.h"
#include "Texture.h"

class TextureManager : public ResourceManager
{
public :
	TextureManager();
	~TextureManager();

	TexturePtr createTextureFromFile(const wchar_t* filename);
	TexturePtr createTexture(const Rect& s, Texture::TEXTURE_TYPE type);

protected:
	virtual Resource* createResourceFromFileConcrete(const wchar_t* filename);
};

