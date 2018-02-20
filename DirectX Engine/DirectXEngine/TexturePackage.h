#pragma once
/*******************************************************************************************************************
	TexturePackage.h, TexturePackage.cpp
	Created by Kim Kane
	Last updated: 19/02/2018

	Creates a texture package (5 x textures), which can be used for terrain multi-texture rendering.

*******************************************************************************************************************/
#include <array>
#include <string>

#include "Texture.h"

class TexturePackage : public Texture {

public:
	TexturePackage();
	~TexturePackage();

public:
	bool LoadTexturePackage(const std::string& backgroundTexture, const std::string& rTexture, const std::string& gTexture, const std::string& bTexture, const std::string& blendMap);

public:
	Texture* GetPackedTexture(int index);

private:
	std::array<Texture, 5> m_packedTextures;
};