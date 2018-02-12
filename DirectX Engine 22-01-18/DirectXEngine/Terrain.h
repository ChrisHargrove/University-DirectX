#pragma once

/*******************************************************************************************************************
	Terrain.h, Terrain.cpp
	Created by Kim Kane
<<<<<<< HEAD
	Last updated: 25/01/2018
=======
	Last updated: 10/02/2018
>>>>>>> master

	Terrain class that loads in a heightmap from a BMP file and sends this data to the GPU.
	Creates multi-height terrain.

*******************************************************************************************************************/
#include <d3d11.h>
#include <xnamath.h>
#include <vector>
#include "Buffer.h"
<<<<<<< HEAD
#include "BasicShader.h"

class Camera;

class Terrain {

private:
	struct HeightMapType
	{
		float x, y, z;
	};
=======
#include "TerrainShader.h"
#include "Constants.h"
#include "AlignedAllocationPolicy.h"

class Camera;

class Terrain : public AlignedAllocationPolicy<BYTE_16> {
>>>>>>> master

public:
	Terrain();
	~Terrain();

	bool Initialize(const char* fileLocation);
	void Render(Camera* camera);

private:
	bool LoadHeightMap(const char* fileLocation);
<<<<<<< HEAD
	void NormalizeHeightMap();
=======
	void LevelHeightMap();
	void CalculateNormals();
>>>>>>> master
	void SetVertexPosition(int position);
	bool InitializeBuffers();

private:
	int m_terrainWidth, m_terrainHeight;
<<<<<<< HEAD
=======
	
	float			m_terrainLevel;
>>>>>>> master

	Buffer			m_buffer;
	unsigned int	m_stride;
	unsigned int	m_offset;

<<<<<<< HEAD
	BasicShader		m_basicShader;

	XMMATRIX		m_localTransform;

private:
	std::vector<HeightMapType>		m_heightMap;
	std::vector<TerrainVertexType>	m_vertices;
	std::vector<unsigned long>		m_indices;
};
=======
	TerrainShader	m_terrainShader;

	XMMATRIX		m_transform;

private:
	struct HeightMapData
	{
		float		x, y, z;
		XMFLOAT3	normal;
	};

private:
	std::vector<HeightMapData>							m_heightMap;
	std::vector<BufferConstants::PackedTerrainVertex>	m_vertices;
	std::vector<unsigned long>							m_indices;
};
>>>>>>> master
