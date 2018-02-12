#pragma once
/*******************************************************************************************************************
	BasicShader.h, BasicShader.cpp
	Created by Kim Kane
<<<<<<< HEAD
	Last updated: 26/01/2018

	Loads in a basic vertex and pixel shader.
	Attributes available: world, view, projection matrices, model position and colour
=======
	Last updated: 09/02/2018

	Loads in a basic vertex and pixel shader.
	Attributes available: world, view, projection matrices, model position and texture
>>>>>>> master

*******************************************************************************************************************/
#include <d3d11.h>
#include <xnamath.h>
#include <d3dx11async.h>

<<<<<<< HEAD
=======
class Texture;
>>>>>>> master
class Camera;

class BasicShader {

<<<<<<< HEAD
private:
	struct MatrixBufferType
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
	};

=======
>>>>>>> master
public:
	BasicShader();
	~BasicShader();

	bool LoadShader(WCHAR* vertexFileLocation, WCHAR* pixelFileLocation);
<<<<<<< HEAD
	void Bind(D3D_PRIMITIVE_TOPOLOGY renderMode = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST) const;
	bool UpdateConstantBuffers(XMMATRIX& world, Camera* camera);
=======
	void Bind(XMMATRIX& world, Camera* camera, Texture* texture, D3D_PRIMITIVE_TOPOLOGY renderMode = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
>>>>>>> master

private:
	BasicShader(const BasicShader&);

private:
	void OutputShaderErrorMessage(ID3D10Blob* errorMessage, WCHAR* fileLocation);
<<<<<<< HEAD
=======
	bool UpdateConstantBuffers(XMMATRIX& world, Camera* camera);
	void SetTexture(Texture* texture);
>>>>>>> master

private:
	ID3D11VertexShader*		m_vertexShader;
	ID3D11PixelShader*		m_pixelShader;
	ID3D11InputLayout*		m_layout;
	ID3D11Buffer*			m_matrixBuffer;
<<<<<<< HEAD
};
=======

private:
	struct MatrixBufferData
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
	};
};
>>>>>>> master
