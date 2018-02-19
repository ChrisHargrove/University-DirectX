#pragma once

#include "Texture.h"
#include <array>
#include <string>

class TexturePackage : public Texture {

public:
	TexturePackage();
	~TexturePackage();

public:
	Texture* GetPackedTexture(int index);

public:
	bool LoadTexturePackage(const std::string& backgroundTexture, const std::string& rTexture, const std::string& gTexture, const std::string& bTexture, const std::string& blendMap);

private:
	std::array<Texture, 5> m_packedTextures;
};