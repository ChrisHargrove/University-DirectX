#pragma once

#include <d3d11.h>

class Texture {

public:
	Texture();
	~Texture();

public:
	ID3D11ShaderResourceView* const* GetTexture()	{ return &m_texture; }
	ID3D11SamplerState* const* GetSampler()			{ return &m_textureSampler; }

public:
	bool LoadTexture(const char* fileLocation);
    float GetWidth() { return _Width; }
    float GetHeight() { return _Height; }

private:
    float _Width, _Height;

	ID3D11ShaderResourceView*	m_texture;
	ID3D11SamplerState*			m_textureSampler;
};