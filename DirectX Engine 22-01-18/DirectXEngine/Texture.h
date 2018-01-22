#ifndef TEXTURE_H_
#define TEXTURE_H_

#include <d3d11.h>

class Texture {

public:
	Texture();
	~Texture();

	ID3D11ShaderResourceView* const* GetTexture() { return &m_colorMap; }
	ID3D11SamplerState* const* GetSampler() { return &m_colorMapSampler; }
	bool LoadTexture(const char* fileLocation, ID3D11Device* device);
	void UnloadTexture();
protected:

private:
	ID3D11ShaderResourceView* m_colorMap;
	ID3D11SamplerState* m_colorMapSampler;
};

#endif