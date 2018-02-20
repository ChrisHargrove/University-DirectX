#include "QuadTree.h"
#include "Camera.h"
#include "Log.h"

#include <iostream>

QuadTree::QuadTree():
	_ParentQuad(nullptr)
{
	_DrawCount = 0;
}

QuadTree::~QuadTree()
{
	ReleaseQuad(_ParentQuad);
	delete _ParentQuad;
	_ParentQuad = nullptr;
}


//collision
bool QuadTree::GetHeightAtPosition(float positionX, float positionZ, float& height)
{
	float meshMinX, meshMaxX, meshMinZ, meshMaxZ;


	meshMinX = _ParentQuad->_Position.x - (_ParentQuad->_Width / 2.0f);
	meshMaxX = _ParentQuad->_Position.x + (_ParentQuad->_Width / 2.0f);

	meshMinZ = _ParentQuad->_Position.y - (_ParentQuad->_Width / 2.0f);
	meshMaxZ = _ParentQuad->_Position.y + (_ParentQuad->_Width / 2.0f);

	// Make sure the coordinates are actually over a polygon.
	if ((positionX < meshMinX) || (positionX > meshMaxX) || (positionZ < meshMinZ) || (positionZ > meshMaxZ))
	{
		return false;
	}

	// Find the node which contains the polygon for this position.
	FindNode(_ParentQuad, positionX, positionZ, height);

	return true;
}

void QuadTree::FindNode(QuadType * node, float x , float z, float & height)
{
	float xMin, xMax, zMin, zMax;
	int count, i, index;
	float vertex1[3], vertex2[3], vertex3[3];
	bool foundHeight;


	// Calculate the dimensions of this node.
	xMin = node->_Position.x - (node->_Width / 2.0f);
	xMax = node->_Position.x + (node->_Width / 2.0f);

	zMin = node->_Position.y - (node->_Width / 2.0f);
	zMax = node->_Position.y + (node->_Width / 2.0f);

	// See if the x and z coordinate are in this node, if not then stop traversing this part of the tree.
	if ((x < xMin) || (x > xMax) || (z < zMin) || (z > zMax))
	{
		return;
	}

	// If the coordinates are in this node then check first to see if children nodes exist.
	count = 0;

	for (i = 0; i<4; i++)
	{
		if (node->_ChildQuads[i] != 0)
		{
			count++;
			FindNode(node->_ChildQuads[i], x, z, height);
		}
	}

	// If there were children nodes then return since the polygon will be in one of the children.
	if (count > 0)
	{
		return;
	}

	// If there were no children then the polygon must be in this node.  Check all the polygons in this node to find 
	// the height of which one the polygon we are looking for.
	for (i = 0; i<node->_Buffer.GetVertexCount() / 3; i++)
	{
		index = i * 3;
		vertex1[0] = node->_VertexArray[index].position.x;
		vertex1[1] = node->_VertexArray[index].position.y;
		vertex1[2] = node->_VertexArray[index].position.z;
						   					 
		index++;		   					
		vertex2[0] = node->_VertexArray[index].position.x;
		vertex2[1] = node->_VertexArray[index].position.y;
		vertex2[2] = node->_VertexArray[index].position.z;
						   					  
		index++;		   					  
		vertex3[0] = node->_VertexArray[index].position.x;
		vertex3[1] = node->_VertexArray[index].position.y;
		vertex3[2] = node->_VertexArray[index].position.z;

		// Check to see if this is the polygon we are looking for.
		foundHeight = CheckHeightOfTriangle(x, z, height, vertex1, vertex2, vertex3);

		// If this was the triangle then quit the function and the height will be returned to the calling function.
		if (foundHeight)
		{
			return;
		}
	}
	return;

}

bool QuadTree::CheckHeightOfTriangle(float x, float z, float & height, float v0[3], float v1[3], float v2[3])
{
	float startVector[3], directionVector[3], edge1[3], edge2[3], normal[3];
	float Q[3], e1[3], e2[3], e3[3], edgeNormal[3], temp[3];
	float magnitude, D, denominator, numerator, t, determinant;


	// Starting position of the ray that is being cast.
	startVector[0] = x;
	startVector[1] = 0.0f;
	startVector[2] = z;

	// The direction the ray is being cast.
	directionVector[0] = 0.0f;
	directionVector[1] = -1.0f;
	directionVector[2] = 0.0f;

	// Calculate the two edges from the three points given.
	edge1[0] = v1[0] - v0[0];
	edge1[1] = v1[1] - v0[1];
	edge1[2] = v1[2] - v0[2];

	edge2[0] = v2[0] - v0[0];
	edge2[1] = v2[1] - v0[1];
	edge2[2] = v2[2] - v0[2];

	// Calculate the normal of the triangle from the two edges.
	normal[0] = (edge1[1] * edge2[2]) - (edge1[2] * edge2[1]);
	normal[1] = (edge1[2] * edge2[0]) - (edge1[0] * edge2[2]);
	normal[2] = (edge1[0] * edge2[1]) - (edge1[1] * edge2[0]);

	magnitude = (float)sqrt((normal[0] * normal[0]) + (normal[1] * normal[1]) + (normal[2] * normal[2]));
	normal[0] = normal[0] / magnitude;
	normal[1] = normal[1] / magnitude;
	normal[2] = normal[2] / magnitude;

	// Find the distance from the origin to the plane.
	D = ((-normal[0] * v0[0]) + (-normal[1] * v0[1]) + (-normal[2] * v0[2]));

	// Get the denominator of the equation.
	denominator = ((normal[0] * directionVector[0]) + (normal[1] * directionVector[1]) + (normal[2] * directionVector[2]));

	// Make sure the result doesn't get too close to zero to prevent divide by zero.
	if (fabs(denominator) < 0.0001f)
	{
		return false;
	}

	// Get the numerator of the equation.
	numerator = -1.0f * (((normal[0] * startVector[0]) + (normal[1] * startVector[1]) + (normal[2] * startVector[2])) + D);

	// Calculate where we intersect the triangle.
	t = numerator / denominator;

	// Find the intersection vector.
	Q[0] = startVector[0] + (directionVector[0] * t);
	Q[1] = startVector[1] + (directionVector[1] * t);
	Q[2] = startVector[2] + (directionVector[2] * t);

	// Find the three edges of the triangle.
	e1[0] = v1[0] - v0[0];
	e1[1] = v1[1] - v0[1];
	e1[2] = v1[2] - v0[2];

	e2[0] = v2[0] - v1[0];
	e2[1] = v2[1] - v1[1];
	e2[2] = v2[2] - v1[2];

	e3[0] = v0[0] - v2[0];
	e3[1] = v0[1] - v2[1];
	e3[2] = v0[2] - v2[2];

	// Calculate the normal for the first edge.
	edgeNormal[0] = (e1[1] * normal[2]) - (e1[2] * normal[1]);
	edgeNormal[1] = (e1[2] * normal[0]) - (e1[0] * normal[2]);
	edgeNormal[2] = (e1[0] * normal[1]) - (e1[1] * normal[0]);

	// Calculate the determinant to see if it is on the inside, outside, or directly on the edge.
	temp[0] = Q[0] - v0[0];
	temp[1] = Q[1] - v0[1];
	temp[2] = Q[2] - v0[2];

	determinant = ((edgeNormal[0] * temp[0]) + (edgeNormal[1] * temp[1]) + (edgeNormal[2] * temp[2]));

	// Check if it is outside.
	if (determinant > 0.001f)
	{
		return false;
	}

	// Calculate the normal for the second edge.
	edgeNormal[0] = (e2[1] * normal[2]) - (e2[2] * normal[1]);
	edgeNormal[1] = (e2[2] * normal[0]) - (e2[0] * normal[2]);
	edgeNormal[2] = (e2[0] * normal[1]) - (e2[1] * normal[0]);

	// Calculate the determinant to see if it is on the inside, outside, or directly on the edge.
	temp[0] = Q[0] - v1[0];
	temp[1] = Q[1] - v1[1];
	temp[2] = Q[2] - v1[2];

	determinant = ((edgeNormal[0] * temp[0]) + (edgeNormal[1] * temp[1]) + (edgeNormal[2] * temp[2]));

	// Check if it is outside.
	if (determinant > 0.001f)
	{
		return false;
	}

	// Calculate the normal for the third edge.
	edgeNormal[0] = (e3[1] * normal[2]) - (e3[2] * normal[1]);
	edgeNormal[1] = (e3[2] * normal[0]) - (e3[0] * normal[2]);
	edgeNormal[2] = (e3[0] * normal[1]) - (e3[1] * normal[0]);

	// Calculate the determinant to see if it is on the inside, outside, or directly on the edge.
	temp[0] = Q[0] - v2[0];
	temp[1] = Q[1] - v2[1];
	temp[2] = Q[2] - v2[2];

	determinant = ((edgeNormal[0] * temp[0]) + (edgeNormal[1] * temp[1]) + (edgeNormal[2] * temp[2]));

	// Check if it is outside.
	if (determinant > 0.001f)
	{
		return false;
	}

	// Now we have our height.
	height = Q[1];
}
//end of collision



bool QuadTree::Initialize(Terrain * terrain)
{
	int vertCount;
	float centerX, centerZ, width;

	_Terrain = new Terrain();
	if (!_Terrain->Initialize("Assets\\Terrain\\heightMap.bmp")) { return false; }
	//get number of verts
	vertCount = _Terrain->GetBuffer()->GetVertexCount();
	//store total triangle copunt for vertex list.
	_TriCount = vertCount / 3;
	//cop vertices into the new array
	_VertexList = *_Terrain->GetTerrainVerts();

	//calculate the center of the mesh and width
	CalculateMeshDimensions(vertCount, centerX, centerZ, width);
	//create the parent quad
	_ParentQuad = new QuadType;

	//recursively build quad tree
	CreateQuadNode(_ParentQuad, centerX, centerZ, width);

	//once quadtree is built
	_VertexList.clear();	//be careful now :P

	DX_LOG("[QUADTREE] Quad Tree Successfully Created!", DX_LOG_EMPTY, LOG_SUCCESS);
	return true;
}

void QuadTree::Render(Frustum * frustum, TerrainShader * shader, Camera* camera)
{
	//reset the num triangles drawn
	_DrawCount = 0;
	//draw all quads.
	RenderQuad(_ParentQuad, frustum, _Terrain->GetShader(), camera);
}

void QuadTree::CalculateMeshDimensions(int vertexCount, float& centerX, float& centerZ, float& meshWidth)
{
	float maxWidth, maxDepth, minWidth, minDepth, width, depth, maxX, maxZ;

	//init center point
	centerX = centerZ = 0.0f;
	//sum all vertices in mesh
	for (int i = 0; i < vertexCount; i++) {
		centerX += _VertexList[i].position.x;
		centerZ += _VertexList[i].position.z;
	}
	//divide total by number of vertices to find mid point
	centerX /= (float)vertexCount;
	centerZ /= (float)vertexCount;

	//init maximums
	maxWidth = maxDepth = 0.0f;

	minWidth = fabsf(_VertexList[0].position.x - centerX);
	minDepth = fabsf(_VertexList[0].position.z - centerZ);

	//go through all vertices and find all dimensions
	for (int i = 0; i < vertexCount; i++) {
		width = fabsf(_VertexList[i].position.x - centerX);
		depth = fabsf(_VertexList[i].position.z - centerZ);

		if (width > maxWidth) { maxWidth = width; }
		if (depth > maxDepth) { maxDepth = depth; }
		if (width < minWidth) { minWidth = width; }
		if (depth < maxDepth) { maxDepth = depth; }
	}

	//find absolute distance between min and max
	maxX = (float)max(fabs(minWidth), fabs(maxWidth));
	maxZ = (float)max(fabs(minDepth), fabs(maxDepth));

	//calc the maximum dimaeter of mesh
	meshWidth = max(maxX, maxZ) * 2.0f;
}

void QuadTree::CreateQuadNode(QuadType * parent, float posX, float posZ, float width)
{
	int numTri, count, vertCount, index, vertIndex;
	float offsetX, offsetZ;
	std::vector<BufferConstants::PackedTerrainVertex> vertices;
	std::vector<unsigned long> indices;
	bool result;
	
	//init node
	parent->_Position.x = posX;
	parent->_Position.y = posZ; //just bear with it!
	parent->_Width = width;

	//init child nodes to nullptr;
	for (int i = 0; i < 4; i++) { parent->_ChildQuads[i] = nullptr;	}

	//count triangle count
	numTri = CountTriangles(posX, posZ, width);

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~CASE 1~~~~~~~~~~~~~~~~~No Triangles
	if (numTri == 0) { return; }

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~CASE 2~~~~~~~~~~~~~~~~~Too Many Triangles
	if (numTri > MAX_TRIANGLES) {
		for (int i = 0; i < 4; i++) {
			//calculate the position offsets for child
			offsetX = (((i % 2) < 1) ? -1.0f : 1.0f) * (width / 4.0f);
			offsetZ = (((i % 4) < 2) ? -1.0f : 1.0f) * (width / 4.0f);

			//see if any triangles in the new node
			count = CountTriangles((posX + offsetX), (posZ + offsetZ), (width / 2.0f));
			if (count > 0) {
				//if no triangle inside this node then make child
				parent->_ChildQuads[i] = new QuadType;
				//extend the tree from this child quad
				CreateQuadNode(parent->_ChildQuads[i], (posX + offsetX), (posZ + offsetZ), (width / 2.0f));
			}
		}
		return;
	}
	
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~CASE 3~~~~~~~~~~~~~~~~~Load Data!
	// Calculate the number of vertices.
	vertCount = numTri * 3;

	// Initialize the index for this new vertex and index array.
	index = 0;

	// Go through all the triangles in the vertex list.
	for (int i = 0; i<_TriCount; i++)
	{
		// If the triangle is inside this node then add it to the vertex array.
		result = IsTriangleContained(i, posX, posZ, width);
		if (result == true)
		{
			// Calculate the index into the terrain vertex list.
			vertIndex = i * 3;
			BufferConstants::PackedTerrainVertex temp;

			// Get the three vertices of this triangle from the vertex list.
			temp.position = _VertexList[vertIndex].position;
			temp.textureCoord = _VertexList[vertIndex].textureCoord;
			temp.normal = _VertexList[vertIndex].normal;
			parent->_VertexArray.push_back(temp);
			vertices.push_back(temp);
			indices.push_back(index);
			index++;

			vertIndex++;
			temp.position = _VertexList[vertIndex].position;
			temp.textureCoord = _VertexList[vertIndex].textureCoord;
			temp.normal = _VertexList[vertIndex].normal;
			vertices.push_back(temp);
			parent->_VertexArray.push_back(temp);
			indices.push_back(index);
			index++;

			vertIndex++;
			temp.position = _VertexList[vertIndex].position;
			temp.textureCoord = _VertexList[vertIndex].textureCoord;
			temp.normal = _VertexList[vertIndex].normal;
			vertices.push_back(temp);
			parent->_VertexArray.push_back(temp);
			indices.push_back(index);
			index++;
		}
	}
	if (!parent->_Buffer.Push(vertices)) { return; };
	if (!parent->_Buffer.Push(indices)) { return; };

}

int QuadTree::CountTriangles(float posX, float posZ, float width)
{
	int count = 0;
	bool result;

	for (int i = 0; i < _TriCount; i++) {
		result = IsTriangleContained(i, posX, posZ, width);
		if (result) { count++; }
	}
	return count;
}

bool QuadTree::IsTriangleContained(int index, float posX, float posZ, float width)
{
	float radius;
	int vertexIndex;
	float x1, z1, x2, z2, x3, z3;
	float minimumX, maximumX, minimumZ, maximumZ;


	// Calculate the radius of this node.
	radius = width / 2.0f;

	// Get the index into the vertex list.
	vertexIndex = index * 3;

	// Get the three vertices of this triangle from the vertex list.
	x1 = _VertexList[vertexIndex].position.x;
	z1 = _VertexList[vertexIndex].position.z;
	vertexIndex++;

	x2 = _VertexList[vertexIndex].position.x;
	z2 = _VertexList[vertexIndex].position.z;
	vertexIndex++;

	x3 = _VertexList[vertexIndex].position.x;
	z3 = _VertexList[vertexIndex].position.z;

	// Check to see if the minimum of the x coordinates of the triangle is inside the node.
	minimumX = min(x1, min(x2, x3));
	if (minimumX > (posX + radius))
	{
		return false;
	}

	// Check to see if the maximum of the x coordinates of the triangle is inside the node.
	maximumX = max(x1, max(x2, x3));
	if (maximumX < (posX - radius))
	{
		return false;
	}

	// Check to see if the minimum of the z coordinates of the triangle is inside the node.
	minimumZ = min(z1, min(z2, z3));
	if (minimumZ >(posZ + radius))
	{
		return false;
	}

	// Check to see if the maximum of the z coordinates of the triangle is inside the node.
	maximumZ = max(z1, max(z2, z3));
	if (maximumZ < (posZ - radius))
	{
		return false;
	}

	return true;
}

void QuadTree::ReleaseQuad(QuadType * parent)
{
	// Recursively go down the tree and release the bottom nodes first.
	for ( int i = 0; i<4; i++)
	{
		if (parent->_ChildQuads[i] != 0)
		{
			ReleaseQuad(parent->_ChildQuads[i]);
		}
	}

	// Release the four child nodes.
	for (int i = 0; i<4; i++)
	{
		if (parent->_ChildQuads[i])
		{
			delete parent->_ChildQuads[i];
			parent->_ChildQuads[i] = nullptr;
		}
	}
}

void QuadTree::RenderQuad(QuadType * quad, Frustum * frustum, TerrainShader * shader, Camera* camera)
{
	bool result;
	int count, i, indexCount;
	unsigned int stride, offset;

	// Check to see if the node can be viewed, height doesn't matter in a quad tree.
	result = frustum->CheckCube(quad->_Position.x, 0.0f, quad->_Position.y, (quad->_Width / 2.0f));
	// If it can't be seen then none of its children can either so don't continue down the tree, this is where the speed is gained.
	if (!result)
	{
		return;
	}

	// If it can be seen then check all four child nodes to see if they can also be seen.
	count = 0;
	for (i = 0; i<4; i++)
	{
		if (quad->_ChildQuads[i] != 0)
		{
			count++;
			RenderQuad(quad->_ChildQuads[i], frustum, shader, camera);
		}
	}

	// If there were any children nodes then there is no need to continue as parent nodes won't contain any triangles to render.
	if (count != 0)
	{
		return;
	}
	// Otherwise if this node can be seen and has triangles in it then render these triangles.

	stride = sizeof(BufferConstants::PackedTerrainVertex);


	shader->Bind(XMMatrixIdentity(), camera, _Terrain->GetPackage());
	quad->_Buffer.Render(stride, 0);

	// Increase the count of the number of polygons that have been rendered during this frame.
	_DrawCount += quad->_Buffer.GetIndexCount() / 3;
}
