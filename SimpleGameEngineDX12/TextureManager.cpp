#include "TextureManager.h"
#include "Texture.h"

TextureManager::TextureManager() : ResourceManager()
{
}

TextureManager::~TextureManager()
{
}

TexturePtr TextureManager::createTextureFromFile(const wchar_t* filename)
{
	return std::static_pointer_cast<Texture>(createResourceFromFile(filename));
}

TexturePtr TextureManager::createTexture(const Rect& s, Texture::TEXTURE_TYPE type)
{
	Texture* tex = nullptr;
	try
	{
		tex = new Texture(s, type);
	}
	catch (...)
	{
	}
	TexturePtr res(tex);
	return res;
}

Resource* TextureManager::createResourceFromFileConcrete(const wchar_t* filename)
{
	Texture* tex = nullptr;
	try
	{
		tex = new Texture(filename);
	}
	catch (...)
	{
	}
	return tex;
}
