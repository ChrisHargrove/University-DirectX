#pragma once

/*******************************************************************************************************************
	ObjLoader.h, ObjLoader.cpp
	Created by Kim Kane
	Last updated: 09/01/2018

	Loads in an OBJ file.
	References: OpenGL Insights, by Patrick Cozzi and Cristophe Riccio

*******************************************************************************************************************/
#include <vector>
#include <map>
#include <string>
#include <d3d11.h>
#include <xnamath.h>

struct PackedVertex {
	XMFLOAT3 position;
	XMFLOAT2 textureCoord;
	XMFLOAT3 normal;
	bool operator<(const PackedVertex that) const {
		return memcmp((void*)this, (void*)&that, sizeof(PackedVertex))>0;
	};
};

class ObjLoader {

public:
	ObjLoader();
	~ObjLoader();

public:
	bool LoadObjFile(const char* fileLocation, std::vector<XMFLOAT3>& outVertices, std::vector<XMFLOAT2>& outTextureCoords, std::vector<XMFLOAT3>& outNormals, std::vector<unsigned int>& outIndices);

private:
	void PushData(std::vector<XMFLOAT3>& outVertices, std::vector<XMFLOAT2>& outTextureCoords, std::vector<XMFLOAT3>& outNormals, std::vector<unsigned int>& outIndices);
	bool GetSimilarVertexIndex(PackedVertex& packed, std::map<PackedVertex, unsigned int>& vertexToOutIndex, unsigned int& result);
	
	void GetVertices(std::vector<XMFLOAT3>& inVertices);
	void GetTextureCoords(std::vector<XMFLOAT2>& inTextureCoords);
	void GetNormals(std::vector<XMFLOAT3>& inNormals);
	void GetIndices(std::vector<unsigned int>& vertexIndices, std::vector<unsigned int>& textureCoordIndices, std::vector<unsigned int>& normalIndices);

	template <typename T> void CalibrateIndices(std::vector<T>& inData, std::vector<T>& finalData, std::vector<unsigned int>& finalIndices);

private:
	std::vector<XMFLOAT3> m_vertices;
	std::vector<XMFLOAT2> m_textureCoords;
	std::vector<XMFLOAT3> m_normals;
};

/*******************************************************************************************************************
	Template function that calibrates/adjusts all indices retrieved from an OBJ file, so they are correctly stored
*******************************************************************************************************************/
template <typename T> void ObjLoader::CalibrateIndices(std::vector<T>& inData, std::vector<T>& finalData, std::vector<unsigned int>& finalIndices)
{
	//---------------------------------------------------------------- We do -1, as the vertices from an obj file do not start from 0, they start from position 1. Where-as array's in C++ start from 0
	//---------------------------------------------------------------- But we also need to check that the actual indices in the file aren't zero, and if they are set them to 1, so then we access tempVertices[0] (Otherwise we would be accessing tempVertices[-1], which clearly breaks EVERYTHING
	for (unsigned int i = 0; i < finalIndices.size(); i++) {
		if (finalIndices[i] == 0) { finalIndices[i] = 1; }
		finalData.push_back(inData[finalIndices[i] - 1]);
	}
}