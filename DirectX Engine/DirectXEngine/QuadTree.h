#pragma once

#include "Terrain.h"
#include "TerrainShader.h"
#include "Buffer.h"
#include "Frustum.h"
#include "Constants.h"

#include <vector>

class Camera;

class QuadTree
{
private:
	struct QuadType{
		XMFLOAT2 _Position;
		float	_Width;
		Buffer	_Buffer;
		std::vector<BufferConstants::PackedTerrainVertex> _VertexArray;
		QuadType* _ChildQuads[4];
	};

public:
	QuadTree();
	~QuadTree();

	//collision
	bool GetHeightAtPosition(float, float, float&);
	void FindNode(QuadType*, float, float, float&);
	bool CheckHeightOfTriangle(float, float, float&, float[3], float[3], float[3]);

	bool Initialize(Terrain* terrain);
	void Render(Frustum* frustum, TerrainShader* shader, Camera* camera);
	int GetDrawCount() { return _DrawCount; }

private:
	void CalculateMeshDimensions(int vertexCount, float& centerX, float& centerZ, float& width);
	void CreateQuadNode(QuadType * parent, float posX, float posZ, float width);
	int CountTriangles(float, float, float);
	bool IsTriangleContained(int, float, float, float);

	void ReleaseQuad(QuadType*);
	void RenderQuad(QuadType*, Frustum*, TerrainShader* shader, Camera* camera);

private:
	std::vector<BufferConstants::PackedTerrainVertex> _VertexList;
	int _TriCount, _DrawCount;

	QuadType*	_ParentQuad;

	const unsigned int MAX_TRIANGLES = 10000;

	Terrain* _Terrain;

};

