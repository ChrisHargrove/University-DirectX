#include "Buffer.h"
#include "objLoader.h"
#include "Log.h"
#include "GraphicsManager.h"

/*******************************************************************************************************************
	Constructor with initializer list to set all default values of variables
*******************************************************************************************************************/
Buffer::Buffer()	:	m_vertexBufferObject(nullptr),
						m_indexBufferObject(nullptr),
						m_vertexCount(0),
						m_indexCount(0)
{
	DX_LOG("[BUFFER] Buffer constructor initialized", DX_LOG_EMPTY, LOG_MESSAGE);
}


/*******************************************************************************************************************
	Shut down all necessary procedures, release resources and clean up memory
*******************************************************************************************************************/
Buffer::~Buffer()
{
	if (m_vertexBufferObject)	m_vertexBufferObject->Release();  m_vertexBufferObject = nullptr;
	if (m_indexBufferObject)	m_indexBufferObject->Release();   m_indexBufferObject = nullptr;

	DX_LOG("[BUFFER] Buffer destructor initialized", DX_LOG_EMPTY, LOG_MESSAGE);
}


/*******************************************************************************************************************
	Function that sends user-defined or OBJ model vertex data to the GPU
*******************************************************************************************************************/
bool Buffer::Push(const std::vector<PackedVertex>& vertices) {
	
	HRESULT result = S_OK;

	m_vertexCount = vertices.size();

	if (vertices.empty()) { 
		DX_LOG("[VERTEX BUFFER] Model vertices vector container is empty", DX_LOG_EMPTY, LOG_ERROR); return false;
	}

	D3D11_BUFFER_DESC vertexDescription = { 0 };
	vertexDescription.Usage = D3D11_USAGE_DEFAULT;
	vertexDescription.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexDescription.ByteWidth = sizeof(PackedVertex) * vertices.size();

	D3D11_SUBRESOURCE_DATA vertexData = { 0 };
	vertexData.pSysMem = &vertices.front();

	result = Graphics::Instance()->GetDevice()->CreateBuffer(&vertexDescription, &vertexData, &m_vertexBufferObject);
	
	if (FAILED(result)) {
		DX_LOG("[VERTEX BUFFER] Problem creating model vertex buffer", DX_LOG_EMPTY, LOG_ERROR); return false;
	}

	DX_LOG("[VERTEX BUFFER] Pushed data to the graphics card...", DX_LOG_EMPTY, LOG_MESSAGE);

	return true;
}


/*******************************************************************************************************************
	Function that sends user-defined or OBJ model index data to the GPU
*******************************************************************************************************************/
bool Buffer::Push(const std::vector<unsigned int>& indices)
{
	HRESULT result = S_OK;

	m_indexCount = indices.size();

	if (indices.empty()) {
		DX_LOG("[INDEX BUFFER] Model indices vector container is empty", DX_LOG_EMPTY, LOG_ERROR); return false;
	}

	D3D11_BUFFER_DESC indexDescription = { 0 };
	indexDescription.Usage = D3D11_USAGE_DEFAULT;
	indexDescription.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexDescription.ByteWidth = sizeof(unsigned int) * indices.size();

	D3D11_SUBRESOURCE_DATA indexData = { 0 };
	indexData.pSysMem = &indices.front();
	
	result = Graphics::Instance()->GetDevice()->CreateBuffer(&indexDescription, &indexData, &m_indexBufferObject);
	
	if (FAILED(result)) {
		DX_LOG("[INDEX BUFFER] Problem creating model index buffer", DX_LOG_EMPTY, LOG_ERROR); return false;
	}

	DX_LOG("[INDEX BUFFER] Pushed data to the graphics card...", DX_LOG_EMPTY, LOG_MESSAGE);

	return true;
}


/*******************************************************************************************************************
	Function that sends user-defined terrain vertex data to the GPU
*******************************************************************************************************************/
bool Buffer::Push(const std::vector<TerrainVertexType>& vertices)
{
	HRESULT result = S_OK;

	if (vertices.empty()) {
		DX_LOG("[TERRAIN VERTEX BUFFER] Terrain vertices vector container is empty", DX_LOG_EMPTY, LOG_ERROR); return false;
	}

	D3D11_BUFFER_DESC vertexDescription = { 0 };
	vertexDescription.Usage = D3D11_USAGE_DEFAULT;
	vertexDescription.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexDescription.ByteWidth = sizeof(TerrainVertexType) * vertices.size();

	D3D11_SUBRESOURCE_DATA vertexData = { 0 };
	vertexData.pSysMem = &vertices.front();

	result = Graphics::Instance()->GetDevice()->CreateBuffer(&vertexDescription, &vertexData, &m_vertexBufferObject);

	if (FAILED(result)) {
		DX_LOG("[TERRAIN VERTEX BUFFER] Problem creating terrain vertex buffer", DX_LOG_EMPTY, LOG_ERROR); return false;
	}

	DX_LOG("[TERRAIN VERTEX BUFFER] Pushed data to the graphics card...", DX_LOG_EMPTY, LOG_MESSAGE);

	return true;
}


/*******************************************************************************************************************
	Function that sends user-defined terrain index data to the GPU
*******************************************************************************************************************/
bool Buffer::Push(const std::vector<unsigned long>& indices)
{
	HRESULT result = S_OK;

	if (indices.empty()) {
		DX_LOG("[TERRAIN INDEX BUFFER] Terrain indices vector container is empty", DX_LOG_EMPTY, LOG_ERROR); return false;
	}

	D3D11_BUFFER_DESC indexDescription = { 0 };
	indexDescription.Usage = D3D11_USAGE_DEFAULT;
	indexDescription.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexDescription.ByteWidth = sizeof(unsigned long) * indices.size();

	D3D11_SUBRESOURCE_DATA indexData = { 0 };
	indexData.pSysMem = &indices.front();

	result = Graphics::Instance()->GetDevice()->CreateBuffer(&indexDescription, &indexData, &m_indexBufferObject);
	
	if (FAILED(result)) {
		DX_LOG("[TERRAIN INDEX BUFFER] Problem creating terrain index buffer", DX_LOG_EMPTY, LOG_ERROR); return false;
	}

	DX_LOG("[TERRAIN INDEX BUFFER] Pushed data to the graphics card...", DX_LOG_EMPTY, LOG_MESSAGE);

	return true;
}


/*******************************************************************************************************************
	Accessor Methods
*******************************************************************************************************************/
unsigned int Buffer::GetIndexCount() const	{ return m_indexCount; }
unsigned int Buffer::GetVertexCount() const { return m_vertexCount; }

ID3D11Buffer* const* Buffer::GetVertexBuffer() const	{ return &m_vertexBufferObject; }
ID3D11Buffer* const* Buffer::GetIndexBuffer() const		{ return &m_indexBufferObject; }


/*******************************************************************************************************************
	Modifier Methods
*******************************************************************************************************************/
void Buffer::SetTerrainVertexCount(unsigned int vertexCount)	{ m_vertexCount = vertexCount; }
void Buffer::SetTerrainIndexCount(unsigned int indexCount)		{ m_indexCount = indexCount; }


/*******************************************************************************************************************
	Static functions and variables
*******************************************************************************************************************/
bool Buffer::LockConstantBuffer(ID3D11Buffer* constantBuffer, D3D11_MAPPED_SUBRESOURCE& mappedResource)
{	
	//-------------------------------------------- Lock the constant buffer so it can be written to
	HRESULT result = S_OK;
	result = Graphics::Instance()->GetDeviceContext()->Map(constantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result)) { 
		DX_LOG("[CONSTANT BUFFER] Problem writing to the constant buffer", DX_LOG_EMPTY, LOG_ERROR); return false; 
	}

	return true;
}


void Buffer::UnlockConstantBuffer(ID3D11Buffer* constantBuffer)
{
	//-------------------------------------------- Unlock the constant buffer after writing to it
	Graphics::Instance()->GetDeviceContext()->Unmap(constantBuffer, 0);
}


void Buffer::SetConstantBuffer(unsigned int location, ID3D11Buffer* constantBuffer)
{
	//-------------------------------------------- Sets the constant buffer in the vertex shader with the updated values
	Graphics::Instance()->GetDeviceContext()->VSSetConstantBuffers(location, 1, &constantBuffer);
}


bool Buffer::CreateConstantBuffer(ID3D11Buffer** constantBuffer, UINT bufferByteSize)
{
	//-------------------------------------------- Set up the description of the dynamic constant buffer that is in the vertex shader
	HRESULT result = S_OK;
	D3D11_BUFFER_DESC bufferDescription = { 0 };
	bufferDescription.Usage = D3D11_USAGE_DYNAMIC;
	bufferDescription.ByteWidth = bufferByteSize;
	bufferDescription.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDescription.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bufferDescription.MiscFlags = 0;
	bufferDescription.StructureByteStride = 0;

	result = Graphics::Instance()->GetDevice()->CreateBuffer(&bufferDescription, nullptr, constantBuffer);
	if (FAILED(result)) { 
		DX_LOG("[CONSTANT BUFFER] Problem creating constant buffer", DX_LOG_EMPTY, LOG_ERROR); return false; 
	}

	return true;
}