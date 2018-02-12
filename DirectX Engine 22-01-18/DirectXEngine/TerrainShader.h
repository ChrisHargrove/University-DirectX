#pragma once
/*******************************************************************************************************************
	TerrainShader.h, TerrainShader.cpp
	Created by Kim Kane
	Last updated: 09/02/2018

	Loads in a terrain vertex and pixel shader.
	Attributes available: world, view, projection matrices, position and texture of terrain

*******************************************************************************************************************/
#include <d3d11.h>
#include <xnamath.h>
#include <d3dx11async.h>

class Texture;
class Camera;

class TerrainShader {

public:
	TerrainShader();
	~TerrainShader();

	bool LoadShader(WCHAR* vertexFileLocation, WCHAR* pixelFileLocation);
	void Bind(XMMATRIX& world, Camera* camera, Texture* texture, D3D_PRIMITIVE_TOPOLOGY renderMode = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

private:
	TerrainShader(const TerrainShader&);

private:
	void OutputShaderErrorMessage(ID3D10Blob* errorMessage, WCHAR* fileLocation);
	bool UpdateConstantBuffers(XMMATRIX& world, Camera* camera);
	void SetTexture(Texture* texture);

private:
	ID3D11VertexShader*		m_vertexShader;
	ID3D11PixelShader*		m_pixelShader;
	ID3D11InputLayout*		m_layout;

	ID3D11Buffer*			m_matrixBuffer;
	ID3D11Buffer*			m_lightBuffer;

	ID3D11SamplerState*		m_samplerState;

private:
	struct MatrixBufferData
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
	};

	struct LightBufferData
	{
		XMFLOAT4 ambientColor;
		XMFLOAT4 diffuseColor;
		XMFLOAT3 lightDirection;
		float padding;
	};
};