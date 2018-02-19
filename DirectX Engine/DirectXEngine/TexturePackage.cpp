#include "TexturePackage.h"

TexturePackage::TexturePackage()
{
}

TexturePackage::~TexturePackage()
{
}

bool TexturePackage::LoadTexturePackage(const std::string& backgroundTexture, const std::string& rTexture, const std::string& gTexture, const std::string& bTexture, const std::string& blendMap)
{
	std::string fileLocation = "Assets\\Textures\\Terrain\\";
	
	if (!GenerateTexture(fileLocation + backgroundTexture, m_packedTextures[0].GetTexture()))	{ return false; }
	if (!GenerateTexture(fileLocation + rTexture, m_packedTextures[1].GetTexture()))			{ return false; }
	if (!GenerateTexture(fileLocation + gTexture, m_packedTextures[2].GetTexture()))			{ return false; }
	if (!GenerateTexture(fileLocation + bTexture, m_packedTextures[3].GetTexture()))			{ return false; }
	if (!GenerateTexture(fileLocation + blendMap, m_packedTextures[4].GetTexture()))			{ return false; }

	return true;
}

Texture* TexturePackage::GetPackedTexture(int index) { return &m_packedTextures[index]; }