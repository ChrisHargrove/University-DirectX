#pragma once

#include <d3d11.h>

class Texture {

public:
	Texture();
	~Texture();

public:
<<<<<<< HEAD
	ID3D11ShaderResourceView* const* GetTexture()	{ return &m_colorMap; }
	ID3D11SamplerState* const* GetSampler()			{ return &m_colorMapSampler; }
=======
	ID3D11ShaderResourceView* const* GetTexture()	{ return &m_texture; }
	ID3D11SamplerState* const* GetSampler()			{ return &m_textureSampler; }
>>>>>>> master

public:
	bool LoadTexture(const char* fileLocation);

private:
<<<<<<< HEAD
	ID3D11ShaderResourceView*	m_colorMap;
	ID3D11SamplerState*			m_colorMapSampler;
=======
	ID3D11ShaderResourceView*	m_texture;
	ID3D11SamplerState*			m_textureSampler;
>>>>>>> master
};