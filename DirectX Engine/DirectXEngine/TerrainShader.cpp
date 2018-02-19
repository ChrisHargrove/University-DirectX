#include <d3dcompiler.h>
#include <fstream>

#include "TerrainShader.h"
#include "GraphicsManager.h"
#include "ScreenManager.h"
#include "Camera.h"
#include "Buffer.h"
#include "Log.h"
#include "Texture.h"
#include "TexturePackage.h"

/*******************************************************************************************************************
Constructor with initializer list to set all default values of variables
*******************************************************************************************************************/
TerrainShader::TerrainShader()	:	m_vertexShader(nullptr),
									m_pixelShader(nullptr),
									m_layout(nullptr),
									m_matrixBuffer(nullptr),
									m_lightBuffer(nullptr),
									m_textureBuffer(nullptr)
{

}


/*******************************************************************************************************************
	Shut down all necessary procedures, release resources and clean up memory
*******************************************************************************************************************/
TerrainShader::~TerrainShader()
{
	if (m_textureBuffer)	{ m_textureBuffer->Release(); m_textureBuffer = nullptr; }
	if (m_lightBuffer)		{ m_lightBuffer->Release(); m_lightBuffer = nullptr; }
	if (m_matrixBuffer)		{ m_matrixBuffer->Release(); m_matrixBuffer = nullptr; }
	if (m_layout)			{ m_layout->Release(); m_layout = nullptr; }
	if (m_pixelShader)		{ m_pixelShader->Release(); m_pixelShader = nullptr; }
	if (m_vertexShader)		{ m_vertexShader->Release(); m_vertexShader = nullptr; }
}


/*******************************************************************************************************************
	Function that loads in a vertex and pixel shader
*******************************************************************************************************************/
bool TerrainShader::LoadShader(WCHAR* vertexFileLocation, WCHAR* pixelFileLocation)
{
	HRESULT result = S_OK;

	ID3D10Blob* errorMessage		= nullptr;
	ID3D10Blob* vertexShaderBuffer	= nullptr;

	//-------------------------------------------- Compile the vertex shader code
	result = D3DCompileFromFile(vertexFileLocation, nullptr, nullptr, "VertexMain", "vs_4_0",
		D3D10_SHADER_ENABLE_STRICTNESS, 0, &vertexShaderBuffer, &errorMessage);

	if (FAILED(result))
	{
		//-------------------------------------------- If the shader failed to compile it should have writen something to the error message
		if (errorMessage) { OutputShaderErrorMessage(errorMessage, vertexFileLocation); }

		//-------------------------------------------- If there was  nothing in the error message then it simply could not find the shader file itself
		else { MessageBox(Screen::Instance()->GetWindow(), (LPCSTR)vertexFileLocation, "Missing Shader File", MB_OK); }

		return false;
	}

	ID3D10Blob* pixelShaderBuffer = nullptr;

	//-------------------------------------------- Compile the pixel shader code
	result = D3DCompileFromFile(pixelFileLocation, nullptr, nullptr, "PixelMain", "ps_4_0",
		D3D10_SHADER_ENABLE_STRICTNESS, 0, &pixelShaderBuffer, &errorMessage);

	if (FAILED(result))
	{
		if (errorMessage) { OutputShaderErrorMessage(errorMessage, pixelFileLocation); }
		else { MessageBox(Screen::Instance()->GetWindow(), (LPCSTR)pixelFileLocation, "Missing Shader File", MB_OK); }

		return false;
	}

	//-------------------------------------------- Create the vertex shader from the buffer
	result = Graphics::Instance()->GetDevice()->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), nullptr, &m_vertexShader);
	if (FAILED(result)) {
		DX_LOG("[TERRAIN SHADER] Can't create vertex shader", DX_LOG_EMPTY, LOG_ERROR); return false;
	}

	//-------------------------------------------- Create the pixel shader from the buffer
	result = Graphics::Instance()->GetDevice()->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &m_pixelShader);
	if (FAILED(result)) {
		DX_LOG("[TERRAIN SHADER] Can't create pixel shader", DX_LOG_EMPTY, LOG_ERROR); return false;
	}

	//-------------------------------------------- Create the layout description
	D3D11_INPUT_ELEMENT_DESC layout[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	//-------------------------------------------- Create vertex input layout
	result = Graphics::Instance()->GetDevice()->CreateInputLayout(layout, _countof(layout), vertexShaderBuffer->GetBufferPointer(),
		vertexShaderBuffer->GetBufferSize(), &m_layout);
	if (FAILED(result)) {
		DX_LOG("[TERRAIN SHADER] Can't create the input layout", DX_LOG_EMPTY, LOG_ERROR); return false;
	}

	//-------------------------------------------- Release the vertex shader buffer and pixel shader buffer since they are no longer needed
	vertexShaderBuffer->Release();
	vertexShaderBuffer = nullptr;

	pixelShaderBuffer->Release();
	pixelShaderBuffer = nullptr;

	//-------------------------------------------- Generate the default sampler filter settings for the textures used within this shader
	if (!Texture::GenerateSamplerFilters())											{ return false; }

	//-------------------------------------------- Create the constant buffer within the shader, so we can access the data from the CPU
	if (!Buffer::CreateConstantBuffer(&m_matrixBuffer, sizeof(MatrixBufferData)))	{ return false; }
	if (!Buffer::CreateConstantBuffer(&m_lightBuffer, sizeof(LightBufferData)))		{ return false; }
	if (!Buffer::CreateConstantBuffer(&m_textureBuffer, sizeof(TextureBufferData)))	{ return false; }

	return true;
}


/*******************************************************************************************************************
	Function that outputs any shader errors generated to a file
*******************************************************************************************************************/
void TerrainShader::OutputShaderErrorMessage(ID3D10Blob* errorMessage, WCHAR* fileLocation)
{
	std::ofstream file;

	//-------------------------------------------- Get a pointer to the error message text buffer
	char* compileErrors = (char*)(errorMessage->GetBufferPointer());

	//-------------------------------------------- Get the length of the message
	unsigned long bufferSize = errorMessage->GetBufferSize();

	//-------------------------------------------- Open a file to write the error message to
	file.open("ShaderErrors.txt");

	//-------------------------------------------- Write out the error message
	for (unsigned long i = 0; i < bufferSize; i++) { file << compileErrors[i]; }

	//-------------------------------------------- Close file and release error message pointer
	file.close();
	errorMessage->Release();
	errorMessage = nullptr;

	//-------------------------------------------- Pop a message up on the screen to notify the user to check the text file for compile errors
	MessageBox(Screen::Instance()->GetWindow(), "Error compiling shader.  Check ShaderErrors.txt for message.", (LPCSTR)fileLocation, MB_OK);
}


/*******************************************************************************************************************
	Function that updates all of the constant buffers within the shader
*******************************************************************************************************************/
bool TerrainShader::UpdateConstantBuffers(XMMATRIX& world, Camera* camera, bool enableBlending)
{
	//-------------------------------------------- Check a shader exists first before trying to update it
	if (m_vertexShader == nullptr || m_pixelShader == nullptr) {
		DX_LOG("[TERRAIN SHADER] Trying to set constant buffers before loading in a shader file", DX_LOG_EMPTY, LOG_ERROR);
		return false;
	}

	//-------------------------------------------- Get the model transform matrix, camera view matrix, and screen projection matrix
	XMMATRIX worldMatrix		= world;
	XMMATRIX viewMatrix			= camera->GetViewMatrix();
	XMMATRIX projectionMatrix	= (Screen::Instance()->Is3dEnabled())	? Screen::Instance()->GetPerspectiveMatrix()
																		: Screen::Instance()->GetOrthographicMatrix();
	//-------------------------------------------- Transpose these matrices to prepare them for the shader
	worldMatrix			= XMMatrixTranspose(worldMatrix);
	viewMatrix			= XMMatrixTranspose(viewMatrix);
	projectionMatrix	= XMMatrixTranspose(projectionMatrix);

	//-------------------------------------------- Create a subresource here so we can access the data later, and lock the constant buffer so we can write to it
	D3D11_MAPPED_SUBRESOURCE mappedResource = { 0 };
	
	//-------------------------------------------- SEND MATRIX CONSTANT BUFFER DATA

	if (!Buffer::LockConstantBuffer(m_matrixBuffer, mappedResource)) { return false; }

		//-------------------------------------------- Get a pointer to the data in the constant buffer
		MatrixBufferData* matrixData = (MatrixBufferData*)mappedResource.pData;

		//-------------------------------------------- Copy the matrices above into the constant buffer
		matrixData->world		= worldMatrix;
		matrixData->view		= viewMatrix;
		matrixData->projection	= projectionMatrix;

	//-------------------------------------------- Unlock the constant buffer when we are finished altering the data
	Buffer::UnlockConstantBuffer(m_matrixBuffer);

	//-------------------------------------------- Finally, set the constant buffer in the vertex shader with the updated values
	Buffer::SetVertexConstantBuffer(0, m_matrixBuffer);


	//-------------------------------------------- SEND LIGHT CONSTANT BUFFER DATA

	if (!Buffer::LockConstantBuffer(m_lightBuffer, mappedResource)) { return false; }

		LightBufferData* lightData = (LightBufferData*)mappedResource.pData;

		lightData->ambientColor		= XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);
		lightData->diffuseColor		= XMFLOAT4(1.3f, 0.5f, 0.0f, 1.0f);
		lightData->lightDirection	= XMFLOAT3(0.0f, 0.0f, 1.0f);
		lightData->lightPadding		= 0.0f; //Ignore padding variables - see shader for more info.
	
	Buffer::UnlockConstantBuffer(m_lightBuffer);
	Buffer::SetPixelConstantBuffer(0, m_lightBuffer);

	//-------------------------------------------- SEND TEXTURE CONSTANT BUFFER DATA

	if (!Buffer::LockConstantBuffer(m_textureBuffer, mappedResource)) { return false; }

		TextureBufferData* textureData = (TextureBufferData*)mappedResource.pData;

		textureData->enableBlending = enableBlending;
		textureData->texturePadding = XMFLOAT3(0.0f, 0.0f, 0.0f); //Ignore padding variables - see shader for more info.

	Buffer::UnlockConstantBuffer(m_textureBuffer);
	Buffer::SetPixelConstantBuffer(1, m_textureBuffer);

	return true;
}


/*******************************************************************************************************************
	Function that sets this shader and vertex layout as the active shader and layout & sets shader parameters
*******************************************************************************************************************/
void TerrainShader::Bind(XMMATRIX& world, Camera* camera, TexturePackage* texturePackage, D3D_PRIMITIVE_TOPOLOGY renderMode)
{
	//-------------------------------------------- Set the vertex input layout
	Graphics::Instance()->GetDeviceContext()->IASetInputLayout(m_layout);

	//-------------------------------------------- Set how this will be drawn - triangles/lines/points, etc.
	Graphics::Instance()->GetDeviceContext()->IASetPrimitiveTopology(renderMode);

	//-------------------------------------------- Set the vertex and pixel shaders that will be used to render this object
	Graphics::Instance()->GetDeviceContext()->VSSetShader(m_vertexShader, nullptr, 0);
	Graphics::Instance()->GetDeviceContext()->PSSetShader(m_pixelShader, nullptr, 0);

	UpdateConstantBuffers(world, camera);

	Graphics::Instance()->GetDeviceContext()->PSSetSamplers(0, 1, Texture::GetSampler());

	SetTexturePackage(texturePackage);
}


void TerrainShader::SetTexturePackage(TexturePackage* texturePackage)
{		
	for (int i = 0; i < 5; i++) {
		if (texturePackage->GetPackedTexture(i)->GetTexture() != nullptr) {
			Graphics::Instance()->GetDeviceContext()->PSSetShaderResources(i, 1, texturePackage->GetPackedTexture(i)->GetTexture());
		}
	}
}
