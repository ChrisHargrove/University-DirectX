#pragma once
/*******************************************************************************************************************
	BasicShader.h, BasicShader.cpp
	Created by Kim Kane
	Last updated: 09/02/2018

	Loads in a basic vertex and pixel shader.
	Attributes available: world, view, projection matrices, model position and texture

*******************************************************************************************************************/
#include <d3d11.h>
#include <xnamath.h>
#include <d3dx11async.h>
#include <string>

class Texture;
class Camera;

class BasicShader {

public:
	BasicShader();
	~BasicShader();

	bool LoadShader(const std::wstring& vertexFileLocation, const std::wstring& pixelFileLocation);
	void Bind(XMMATRIX& world, Camera* camera, Texture* texture, D3D_PRIMITIVE_TOPOLOGY renderMode = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

private:
	BasicShader(const BasicShader&);

private:
	void OutputShaderErrorMessage(ID3D10Blob* errorMessage, const WCHAR* fileLocation);
	bool UpdateConstantBuffers(XMMATRIX& world, Camera* camera);
	void SetTexture(Texture* texture);

private:
	ID3D11VertexShader*		m_vertexShader;
	ID3D11PixelShader*		m_pixelShader;
	ID3D11InputLayout*		m_layout;
	ID3D11Buffer*			m_matrixBuffer;

private:
	struct MatrixBufferData
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
	};
};