#pragma once
/*******************************************************************************************************************
	Texture.h, Texture.cpp
	Created by Kim Kane & Chris Hargrove
	Last updated: 19/02/2018

	Creates a texture object using the DirectX API and loads in a texture from a file.

*******************************************************************************************************************/
#include <d3d11.h>
#include <array>
#include <string>

class Texture {

public:
	Texture();
	~Texture();

public:
	ID3D11ShaderResourceView** GetTexture();
	static ID3D11SamplerState* const* GetSampler();

	float GetHeight() { return _Height; }
	float GetWidth() { return _Width; }

public:
	bool LoadTexture(const std::string& texture);

public:
	static bool GenerateSamplerFilters();

protected:
	bool GenerateTexture(const std::string& fileLocation, ID3D11ShaderResourceView** texture);

private:
	ID3D11ShaderResourceView*	m_texture;

	float _Height, _Width;

	static ID3D11SamplerState*	m_defaultSampler;
};