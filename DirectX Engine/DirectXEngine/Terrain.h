#pragma once

/*******************************************************************************************************************
	Terrain.h, Terrain.cpp
	Created by Kim Kane
	Last updated: 10/02/2018

	Terrain class that loads in a heightmap from a BMP file and sends this data to the GPU.
	Creates multi-height terrain.

*******************************************************************************************************************/
#include <d3d11.h>
#include <xnamath.h>
#include <vector>

#include "Buffer.h"
#include "TerrainShader.h"
#include "Constants.h"
#include "AlignedAllocationPolicy.h"
#include "TexturePackage.h"

class Camera;

class Terrain : public AlignedAllocationPolicy<BYTE_16> {

public:
	Terrain();
	~Terrain();

	bool Initialize(const char* fileLocation);
	void Render(Camera* camera);

public:
	Buffer* GetBuffer();
	std::vector<BufferConstants::PackedTerrainVertex>*	GetTerrainVerts();

	const XMMATRIX& GetWorldMatrix() { return m_transform; }


	//we are not keeping this!!! >:( :)
	TerrainShader* GetShader() { return &m_terrainShader; }
	TexturePackage* GetPackage() { return &m_packedTextures; }

private:
	bool LoadHeightMap(const char* fileLocation);
	void LevelHeightMap();
	void CalculateNormals();
	void SetVertexPosition(int position);
	bool InitializeBuffers();

private:
	int m_terrainWidth, m_terrainHeight;
	
	float			m_terrainLevel;

	Buffer			m_buffer;
	unsigned int	m_stride;
	unsigned int	m_offset;

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

	TexturePackage										m_packedTextures;
};