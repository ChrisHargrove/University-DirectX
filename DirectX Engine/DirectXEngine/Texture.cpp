#include "Texture.h"
#include <d3dx11tex.h>
#include "GraphicsManager.h"
#include "Log.h"

Texture::Texture()	:	m_texture(nullptr),
						m_textureSampler(nullptr)
{

}


Texture::~Texture()
{
	if (m_textureSampler)	m_textureSampler->Release(); m_textureSampler = nullptr;
	if (m_texture)			m_texture->Release(); m_texture = nullptr;
}


bool Texture::LoadTexture(const char* fileLocation)
{
	HRESULT result = S_OK;

	result = D3DX11CreateShaderResourceViewFromFile(Graphics::Instance()->GetDevice(), fileLocation, 0, 0, &m_texture, 0);
	if (FAILED(result)) { DX_LOG("[TEXTURE] Failed to create texture: ", fileLocation, LOG_ERROR); return false; }

	D3D11_SAMPLER_DESC colorMapDescription = {};
	
	/*colorMapDescription.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	colorMapDescription.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	colorMapDescription.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	colorMapDescription.ComparisonFunc = D3D11_COMPARISON_NEVER;
	colorMapDescription.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	//colorMapDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	colorMapDescription.MaxLOD = D3D11_FLOAT32_MAX;

	result = Graphics::Instance()->GetDevice()->CreateSamplerState(&colorMapDescription, &m_textureSampler);

	*/
	
		// Create a texture sampler state description.
	colorMapDescription.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	colorMapDescription.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	colorMapDescription.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	colorMapDescription.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	colorMapDescription.MipLODBias = 0.0f;
	colorMapDescription.MaxAnisotropy = 1;
	colorMapDescription.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	colorMapDescription.BorderColor[0] = 0;
	colorMapDescription.BorderColor[1] = 0;
	colorMapDescription.BorderColor[2] = 0;
	colorMapDescription.BorderColor[3] = 0;
	colorMapDescription.MinLOD = 0;
	colorMapDescription.MaxLOD = D3D11_FLOAT32_MAX;
	result = Graphics::Instance()->GetDevice()->CreateSamplerState(&colorMapDescription, &m_textureSampler);


	if (FAILED(result)) {
		DX_LOG("[TEXTURE] Failed to create colour map sampler state", DX_LOG_EMPTY, LOG_ERROR);
		return false;
	}

	return true;
}
