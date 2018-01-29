#pragma once

/*******************************************************************************************************************
	Terrain.h, Terrain.cpp
	Created by Kim Kane
	Last updated: 25/01/2018

	Terrain class that loads in a heightmap from a BMP file and sends this data to the GPU.
	Creates multi-height terrain.

*******************************************************************************************************************/
#include <d3d11.h>
#include <xnamath.h>
#include <vector>
#include "Buffer.h"
#include "BasicShader.h"

class Camera;

class Terrain {

private:
	struct HeightMapType
	{
		float x, y, z;
	};

public:
	Terrain();
	~Terrain();

	bool Initialize(const char* fileLocation);
	void Render(Camera* camera);

private:
	bool LoadHeightMap(const char* fileLocation);
	void NormalizeHeightMap();
	void SetVertexPosition(int position);
	bool InitializeBuffers();

private:
	int m_terrainWidth, m_terrainHeight;

	Buffer			m_buffer;
	unsigned int	m_stride;
	unsigned int	m_offset;

	BasicShader		m_basicShader;

	XMMATRIX		m_localTransform;

private:
	std::vector<HeightMapType>		m_heightMap;
	std::vector<TerrainVertexType>	m_vertices;
	std::vector<unsigned long>		m_indices;
};
