#include "Buffer.h"
#include "objLoader.h"
#include "Log.h"

Buffer::Buffer()	:	m_vertexBufferObject(0),
						m_indexBufferObject(0),
						m_vertexCount(0),
						m_indexCount(0)
{

}

Buffer::~Buffer()
{

}


bool Buffer::Push(ID3D11Device* device, const std::vector<PackedVertex>& vertexData) {
	
	m_vertexCount = vertexData.size();

	if (vertexData.empty()) { return false; }

	//Create the data layout info for vertices.
	D3D11_BUFFER_DESC vertexDescription;

	ZeroMemory(&vertexDescription, sizeof(vertexDescription));
	vertexDescription.Usage = D3D11_USAGE_DEFAULT;
	vertexDescription.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexDescription.ByteWidth = sizeof(PackedVertex) * vertexData.size();

	D3D11_SUBRESOURCE_DATA resourceData;
	ZeroMemory(&resourceData, sizeof(resourceData));
	resourceData.pSysMem = &vertexData.front();

	if (!device->CreateBuffer(&vertexDescription, &resourceData, &m_vertexBufferObject)) {
		return false;
	}

	Log("[BUFFER] Pushed data to the graphics card...", EMPTY, LOG_MESSAGE);

	return true;
}

bool Buffer::Push(ID3D11Device* device, const std::vector<unsigned int>& indices)
{
	m_indexCount = indices.size();

	if (indices.empty()) { return false; }

	//Create the data layout info for vertices.
	D3D11_BUFFER_DESC indexDescription;

	ZeroMemory(&indexDescription, sizeof(indexDescription));
	indexDescription.Usage = D3D11_USAGE_DEFAULT;
	indexDescription.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexDescription.ByteWidth = sizeof(unsigned int) * indices.size();

	D3D11_SUBRESOURCE_DATA resourceData;
	ZeroMemory(&resourceData, sizeof(resourceData));
	resourceData.pSysMem = &indices.front();
	
	if (!device->CreateBuffer(&indexDescription, &resourceData, &m_indexBufferObject)) {
		return false;
	}

	Log("[INDEX BUFFER] Pushed data to the graphics card...", EMPTY, LOG_MESSAGE);

	return true;
}

unsigned int Buffer::GetIndexCount() const
{
	return m_indexCount;
}

unsigned int Buffer::GetVertexCount() const
{
	return m_vertexCount;
}

ID3D11Buffer* const* Buffer::GetVertexBuffer() const
{
	return &m_vertexBufferObject;
}

ID3D11Buffer* const* Buffer::GetIndexBuffer() const
{
	return &m_indexBufferObject;
}



void Buffer::Destroy()
{
	//Unload Buffer from memory.
	if (m_vertexBufferObject)	m_vertexBufferObject->Release();  m_vertexBufferObject = 0;
	if (m_indexBufferObject)	m_indexBufferObject->Release();   m_indexBufferObject = 0;
}