#include "Texture.h"
#include "D3DX11tex.h"
#include "Memory.h"

Texture::Texture()
{
}

Texture::~Texture()
{
}

bool Texture::LoadTexture(const char* fileLocation, ID3D11Device* device)
{
	HRESULT result = S_OK;

	result = D3DX11CreateShaderResourceViewFromFile(device, fileLocation, 0, 0, &m_colorMap, 0);
	if (FAILED(result))
	{
		//DXTRACE_MSG("Failed to load the texture image!");
		return false;
	}
	
	D3D11_SAMPLER_DESC colorMapDesc;
	ZeroMemory(&colorMapDesc, sizeof(colorMapDesc));
	colorMapDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	colorMapDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	colorMapDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	colorMapDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	colorMapDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	//colorMapDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	colorMapDesc.MaxLOD = D3D11_FLOAT32_MAX;

	result = device->CreateSamplerState(&colorMapDesc, &m_colorMapSampler);

	if (FAILED(result)){
		//DXTRACE_MSG("Failed to create color map sampler state!");
		return false;
	}
	return true;
}

void Texture::UnloadTexture()
{
	if (m_colorMapSampler) SafeRelease(m_colorMapSampler);
	if (m_colorMap) SafeRelease(m_colorMap);
}
