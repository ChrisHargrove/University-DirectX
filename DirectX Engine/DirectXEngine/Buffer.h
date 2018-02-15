#pragma once

/*******************************************************************************************************************
	Buffer.h, Buffer.cpp
	Created by Kim Kane
	Last updated: 09/02/2018

	Creates a DirectX buffer object, which we can use to send data to the graphics card.

*******************************************************************************************************************/
#include <d3d11.h>
#include <vector>
#include "Constants.h"

class Buffer {

public:
	Buffer();
	~Buffer();

public:
	unsigned int GetIndexCount() const;
	unsigned int GetVertexCount() const;

	ID3D11Buffer* const* GetVertexBuffer() const;
	ID3D11Buffer* const* GetIndexBuffer() const;

public:
	void SetTerrainVertexCount(unsigned int vertexCount);
	void SetTerrainIndexCount(unsigned int indexCount);

public:
	bool Push(const std::vector<BufferConstants::PackedVertex>& vertices);
	bool Push(const std::vector<unsigned int>& indices);

	bool Push(const std::vector<BufferConstants::PackedTerrainVertex>& vertices);
	bool Push(const std::vector<unsigned long>& indices);

public:
	void Render(unsigned int stride, unsigned int offset) const;

public:
	static bool LockConstantBuffer(ID3D11Buffer* constantBuffer, D3D11_MAPPED_SUBRESOURCE& mappedResource);
	static void UnlockConstantBuffer(ID3D11Buffer* constantBuffer);
	static void SetVertexConstantBuffer(unsigned int location, ID3D11Buffer* constantBuffer);
	static void SetPixelConstantBuffer(unsigned int location, ID3D11Buffer* constantBuffer);
	static bool CreateConstantBuffer(ID3D11Buffer** constantBuffer, UINT bufferByteSize);

private:
	Buffer(const Buffer& other);

private:
	ID3D11Buffer*	m_vertexBufferObject;
	ID3D11Buffer*	m_indexBufferObject;

	unsigned int	m_vertexCount;
	unsigned int	m_indexCount;
};