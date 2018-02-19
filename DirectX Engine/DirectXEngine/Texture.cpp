#include "Texture.h"
#include <d3dx11tex.h>
#include <string>
#include "GraphicsManager.h"
#include "Log.h"

Texture::Texture()	:	m_texture(nullptr)
{

}


Texture::~Texture()
{
	if (m_defaultSampler)	m_defaultSampler->Release(); m_defaultSampler = nullptr;
	if (m_texture)			m_texture->Release(); m_texture = nullptr;
}

bool Texture::GenerateTexture(const std::string& fileLocation, ID3D11ShaderResourceView** texture)
{
	HRESULT result = S_OK;

	D3DX11_IMAGE_INFO info;
	D3DX11GetImageInfoFromFile(fileLocation.c_str(), nullptr, &info, nullptr);

	_Height = info.Height;
	_Width = info.Width;

	result = D3DX11CreateShaderResourceViewFromFile(Graphics::Instance()->GetDevice(), fileLocation.c_str(), nullptr, nullptr, texture, nullptr);
	if (FAILED(result)) { DX_LOG("[TEXTURE] Failed to create texture: ", fileLocation.c_str(), LOG_ERROR); return false; }

	return true;
}

bool Texture::GenerateSamplerFilters()
{
	HRESULT result = S_OK;

	D3D11_SAMPLER_DESC defaultSamplerDescription = {};

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
	defaultSamplerDescription.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	defaultSamplerDescription.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	defaultSamplerDescription.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	defaultSamplerDescription.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	defaultSamplerDescription.MipLODBias = 0.0f;
	defaultSamplerDescription.MaxAnisotropy = 1;
	defaultSamplerDescription.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	defaultSamplerDescription.BorderColor[0] = 0;
	defaultSamplerDescription.BorderColor[1] = 0;
	defaultSamplerDescription.BorderColor[2] = 0;
	defaultSamplerDescription.BorderColor[3] = 0;
	defaultSamplerDescription.MinLOD = 0;
	defaultSamplerDescription.MaxLOD = D3D11_FLOAT32_MAX;

	result = Graphics::Instance()->GetDevice()->CreateSamplerState(&defaultSamplerDescription, &m_defaultSampler);

	if (FAILED(result)) {
		DX_LOG("[TEXTURE] Failed to create default sampler state", DX_LOG_EMPTY, LOG_ERROR);
		return false;
	}

	return true;
}

bool Texture::LoadTexture(const std::string& texture)
{
	std::string fileLocation = "Assets\\Textures\\";

	if (!GenerateTexture(fileLocation + texture, &m_texture)) { return false; }
	
	return true;
}



ID3D11SamplerState* Texture::m_defaultSampler = nullptr;