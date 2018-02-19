#pragma once

#include <d3d11.h>
#include <array>
#include <string>

class Texture {

public:
	Texture();
	~Texture();

public:
	ID3D11ShaderResourceView** GetTexture()			{ return &m_texture; }
	static ID3D11SamplerState* const* GetSampler()	{ return &m_defaultSampler; }

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