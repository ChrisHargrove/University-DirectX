#include "Texture.h"
#include <d3dx11tex.h>
#include "GraphicsManager.h"
#include "Log.h"

Texture::Texture()	:	m_colorMap(nullptr),
						m_colorMapSampler(nullptr)
{

}


Texture::~Texture()
{
	if (m_colorMapSampler)	m_colorMapSampler->Release(); m_colorMapSampler = nullptr;
	if (m_colorMap)			m_colorMap->Release(); m_colorMap = nullptr;
}


bool Texture::LoadTexture(const char* fileLocation)
{
	HRESULT result = S_OK;

	result = D3DX11CreateShaderResourceViewFromFile(Graphics::Instance()->GetDevice(), fileLocation, 0, 0, &m_colorMap, 0);
	if (FAILED(result)) { DX_LOG("[TEXTURE] Failed to create texture: ", fileLocation, LOG_ERROR); return false; }

	D3D11_SAMPLER_DESC colorMapDescription = {};
	colorMapDescription.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	colorMapDescription.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	colorMapDescription.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	colorMapDescription.ComparisonFunc = D3D11_COMPARISON_NEVER;
	colorMapDescription.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	//colorMapDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	colorMapDescription.MaxLOD = D3D11_FLOAT32_MAX;

	result = Graphics::Instance()->GetDevice()->CreateSamplerState(&colorMapDescription, &m_colorMapSampler);

	if (FAILED(result)) {
		DX_LOG("[TEXTURE] Failed to create colour map sampler state", DX_LOG_EMPTY, LOG_ERROR);
		return false;
	}

	return true;
}
