#include "TexturePackage.h"

/*******************************************************************************************************************
	Constructor with initializer list to set all default values of variables
*******************************************************************************************************************/
TexturePackage::TexturePackage()
{
}


/*******************************************************************************************************************
	Shut down all necessary procedures, release resources and clean up memory
*******************************************************************************************************************/
TexturePackage::~TexturePackage()
{
}


/*******************************************************************************************************************
	Function that loads in 5 x texture files for terrain multitexturing and stores them in to packed textures array
*******************************************************************************************************************/
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


/*******************************************************************************************************************
	Function that returns a texture within the packed textures array at a given index
*******************************************************************************************************************/
Texture* TexturePackage::GetPackedTexture(int index) { return &m_packedTextures[index]; }