#ifndef BUFFER_H_
#define BUFFER_H_

/*******************************************************************************************************************
	Buffer.h, Buffer.cpp
	Created by Kim Kane
	Last updated: 04/01/2018

	Creates an OpenGL buffer object, which we can use to send data to the graphics card.

*******************************************************************************************************************/
#include <d3d11.h>
#include <vector>
#include "objLoader.h"
#include <DxErr.h>

class Buffer {

public:
	Buffer();
	~Buffer();

public:
	unsigned int GetIndexCount() const;
	unsigned int GetVertexCount() const;

	ID3D11Buffer* const* GetVertexBuffer() const;
	ID3D11Buffer* const* GetIndexBuffer() const;

	bool Push(ID3D11Device* device, const std::vector<unsigned int>& indices);
	bool Push(ID3D11Device* device, const std::vector<PackedVertex>& vertexData);

public:
	void Destroy();

private:
	Buffer(const Buffer& other);

private:
	ID3D11Buffer*	m_vertexBufferObject;
	ID3D11Buffer*	m_indexBufferObject;

	unsigned int	m_vertexCount;
	unsigned int	m_indexCount;
};

#endif