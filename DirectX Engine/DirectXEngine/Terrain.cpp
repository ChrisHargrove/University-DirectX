#include <fstream>
#include "Terrain.h"
#include "GraphicsManager.h"
#include "Constants.h"
#include "Camera.h"
#include "Log.h"

/*******************************************************************************************************************
	Struct that defines the vertex locations on the screen, in reference to the terrain
*******************************************************************************************************************/
struct VertexLocation {

	unsigned int bottomLeft, bottomRight, topLeft, topRight;

	void SetCoordinates(int terrainHeight, unsigned int y, unsigned int x)
	{
		bottomLeft	= (terrainHeight * y) + x;
		bottomRight = (terrainHeight * y) + (x + 1);
		topLeft		= (terrainHeight * (y + 1)) + x;
		topRight	= (terrainHeight * (y + 1)) + (x + 1);
	}
};


/*******************************************************************************************************************
	Constructor with initializer list to set all default values of variables
*******************************************************************************************************************/
Terrain::Terrain()	:	m_terrainWidth(0),
						m_terrainHeight(0),
						m_terrainLevel(15.0f),
						m_stride(sizeof(BufferConstants::PackedTerrainVertex)),
						m_offset(0),
						m_transform(XMMatrixIdentity())
{
	DX_LOG("[TERRAIN] Terrain constructor initialized", DX_LOG_EMPTY, LOG_MESSAGE);
}


/*******************************************************************************************************************
	Shut down all necessary procedures, release resources and clean up memory
*******************************************************************************************************************/
Terrain::~Terrain()
{
	DX_LOG("[TERRAIN] Terrain destructor initialized", DX_LOG_EMPTY, LOG_MESSAGE);
}


/*******************************************************************************************************************
	Function that initializes all necessary start-up procedures
*******************************************************************************************************************/
bool Terrain::Initialize(const char* fileLocation)
{
	//---------------------------------------------------------------- Load in the texture/texture package used for the terrain
	if (!m_packedTextures.LoadTexturePackage("Bricks.jpg", "Grass.jpg", "Grass2.jpg", "Dirt.jpg", "BlendMap2.jpg")) { return false; }

	//---------------------------------------------------------------- Load in the shaders used for the terrain
	if (!m_terrainShader.LoadShader(L"Assets\\Shaders\\terrainShader.vs", L"Assets\\Shaders\\terrainShader.ps")) { return false; }

	//---------------------------------------------------------------- Load in the heightmap for the terrain
	if (!LoadHeightMap(fileLocation)) { return false; }

	//---------------------------------------------------------------- Level out the heightmap so that the height of the terrain is not too high
	LevelHeightMap();

	//---------------------------------------------------------------- Calculate normals for terrain lighting
	CalculateNormals();

	//---------------------------------------------------------------- Initialize the vertex and index buffer that hold the geometry for the terrain
	if (!InitializeBuffers()) { return false; }

	return true;
}


/*******************************************************************************************************************
	Function that loads in a BMP grayscale heightmap image
*******************************************************************************************************************/
bool Terrain::LoadHeightMap(const char* fileLocation)
{
	BITMAPFILEHEADER bitmapFileHeader;
	BITMAPINFOHEADER bitmapInfoHeader;

	std::ifstream file;

	//---------------------------------------------------------------- Open the heightmap file in binary mode 
	file.open(fileLocation, std::ios::binary);
	file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	//---------------------------------------------------------------- If there is a problem loading the file, return false
	if (!file.is_open())	{ DX_LOG("[TERRAIN] Heightmap file doesn't exist: ", fileLocation, LOG_ERROR); return false; }
	if (!file.good())		{ DX_LOG("[TERRAIN] Heightmap file corrupted or a file is already open: ", fileLocation, LOG_ERROR); return false; }

	DX_LOG("[TERRAIN] Heightmap file opened successfully: ", fileLocation, LOG_SUCCESS);
	
	//---------------------------------------------------------------- Try to read in the BMP file header and info header, if this fails it means the image is not BMP format
	try { 
		file.read((char*)&bitmapFileHeader, sizeof(BITMAPFILEHEADER)); 
		file.read((char*)&bitmapInfoHeader, sizeof(BITMAPINFOHEADER));
	}
	catch (std::ifstream::failure e) {
		DX_LOG("[TERRAIN] Problem reading BMP headers, heightmap must be BMP format: ", fileLocation, LOG_ERROR);
		file.close(); return false;
	}

	//---------------------------------------------------------------- Save the dimensions of the BMP file width and height in to our user-defined variables
	m_terrainWidth	= bitmapInfoHeader.biWidth;
	m_terrainHeight = bitmapInfoHeader.biHeight;

	//---------------------------------------------------------------- Calculate the size of the BMP image data - width x height x 3 RGB values for each pixel in the BMP
	int imageSize = m_terrainWidth * m_terrainHeight * FileConstants::RGB_OFFSET;

	//---------------------------------------------------------------- Allocate memory for the BMP image data
	std::vector<unsigned char> bitmapImage(imageSize);
	
	try {
		//---------------------------------------------------------------- Move to the beginning of the BMP data, disregarding the headers 
		file.seekg(bitmapFileHeader.bfOffBits, std::ios_base::beg);

		//---------------------------------------------------------------- Read in the BMP image data and store it in to the buffer (vector)
		file.read((char*)&bitmapImage.front(), imageSize);
	}
	catch (std::ifstream::failure e) {
		DX_LOG("[TERRAIN] Problem reading bitmap data: ", fileLocation, LOG_ERROR);
		file.close(); return false;
	}

	//---------------------------------------------------------------- Close the BMP file once we have the data
	file.close();

	//---------------------------------------------------------------- Create the structure to hold the height map data
	m_heightMap.resize(m_terrainWidth * m_terrainHeight);

	//---------------------------------------------------------------- Temporary variables to aid us in storing the data correctly
	int rgb = 0;
	unsigned char height = 0;
	int index = 0;

	//---------------------------------------------------------------- Loop through the heightmap pixels
	for (int z = 0; z < m_terrainHeight; z++) {
		for (int x = 0; x <m_terrainWidth; x++) {
			//---------------------------------------------------------------- Set the height variable to the RGB pixel value read in from the BMP file
			height = bitmapImage[rgb];

			//---------------------------------------------------------------- Move through the BMP file and increment the index each time
			index = (m_terrainHeight * z) + x;

			//---------------------------------------------------------------- Finally, set the heightmap data to the values calculated; y being the pixel data read in from the BMP, and z and x being the incrementation of our nested for loops (0 - width, 0 - height)
			m_heightMap[index].x = (float)x;
			m_heightMap[index].y = (float)height;
			m_heightMap[index].z = (float)z;

			//---------------------------------------------------------------- We only need to read in the 'r' (red) value, as RGB will be the same colour values, due to it being a grayscale image. So, we increment by 3, skipping the green and blue colour values
			rgb += FileConstants::RGB_OFFSET;
		}
	}

	return true;
}


/*******************************************************************************************************************
	Function that levels out a heightmap (flattens/expands the height of terrain)
*******************************************************************************************************************/
void Terrain::LevelHeightMap()
{
	for (int x = 0; x < m_terrainHeight; x++) {
		for (int z = 0; z < m_terrainWidth; z++) {
			m_heightMap[(m_terrainHeight * x) + z].y /= m_terrainLevel;
		}
	}
}


/*******************************************************************************************************************
	Function that calculates the normals of the terrain for lighting effects
*******************************************************************************************************************/
void Terrain::CalculateNormals()
{
	VertexLocation vertex = { 0 };

	XMFLOAT3 bottomLeft, bottomRight, topLeft;
	XMFLOAT3 firstVector, secondVector;

	int index = 0;

	//---------------------------------------------------------------- We do -1 to make the width and height of terrain an odd number, necessary for accurate calculation of normal data
	int offsetTerrainHeight = m_terrainHeight - 1;
	int offsetTerrainWidth	= m_terrainWidth - 1;

	std::vector<XMFLOAT3> normals((offsetTerrainWidth) * (offsetTerrainHeight));

	//---------------------------------------------------------------- Go through all the faces in the mesh and calculate their normals
	for (int y = 0; y < (offsetTerrainHeight); y++) {
		for (int x = 0; x < (offsetTerrainWidth); x++) {

			vertex.SetCoordinates(m_terrainHeight, y, x);

			//---------------------------------------------------------------- Get three vertices from the face.
			bottomLeft.x = m_heightMap[vertex.bottomLeft].x;
			bottomLeft.y = m_heightMap[vertex.bottomLeft].y;
			bottomLeft.z = m_heightMap[vertex.bottomLeft].z;
			
			bottomRight.x = m_heightMap[vertex.bottomRight].x;
			bottomRight.y = m_heightMap[vertex.bottomRight].y;
			bottomRight.z = m_heightMap[vertex.bottomRight].z;

			topLeft.x = m_heightMap[vertex.topLeft].x;
			topLeft.y = m_heightMap[vertex.topLeft].y;
			topLeft.z = m_heightMap[vertex.topLeft].z;

			//---------------------------------------------------------------- Calculate the two vectors for this face
			firstVector.x = bottomLeft.x - topLeft.x;
			firstVector.y = bottomLeft.y - topLeft.y;
			firstVector.z = bottomLeft.z - topLeft.z;

			secondVector.x = topLeft.x - bottomRight.x;
			secondVector.y = topLeft.y - bottomRight.y;
			secondVector.z = topLeft.z - bottomRight.z;

			index = (y * (offsetTerrainHeight)) + x;

			//---------------------------------------------------------------- Calculate the cross product of those two vectors to get the un-normalized value for this face normal
			normals[index].x = (firstVector.y * secondVector.z) - (firstVector.z * secondVector.y);
			normals[index].y = (firstVector.z * secondVector.x) - (firstVector.x * secondVector.z);
			normals[index].z = (firstVector.x * secondVector.y) - (firstVector.y * secondVector.x);
		}
	}

	XMFLOAT3 normal;
	int	count		= 0;
	float length	= 0.0f;

	//---------------------------------------------------------------- Now go through all the vertices and take an average of each face normal that the vertex touches to get the averaged normal for that vertex
	for (int y = 0; y < m_terrainHeight; y++) {
		for (int x = 0; x < m_terrainWidth; x++) {
		
			normal.x = 0.0f;
			normal.y = 0.0f;
			normal.z = 0.0f;
			
			count = 0;

			//---------------------------------------------------------------- Bottom left face
			if (((x - 1) >= 0) && ((y - 1) >= 0)) {
				
				index = ((y - 1) * (offsetTerrainHeight)) + (x - 1);

				normal.x += normals[index].x;
				normal.y += normals[index].y;
				normal.z += normals[index].z;
				count++;
			}

			//---------------------------------------------------------------- Bottom right face
			if ((x < (m_terrainWidth - 1)) && ((y - 1) >= 0)) {
				
				index = ((y - 1) * (offsetTerrainHeight)) + x;

				normal.x += normals[index].x;
				normal.y += normals[index].y;
				normal.z += normals[index].z;
				count++;
			}

			//---------------------------------------------------------------- Top left face
			if (((x - 1) >= 0) && (y < (offsetTerrainHeight))) {
				
				index = (y * (offsetTerrainHeight)) + (x - 1);

				normal.x += normals[index].x;
				normal.y += normals[index].y;
				normal.z += normals[index].z;
				count++;
			}

			//---------------------------------------------------------------- Top right face
			if ((x < (offsetTerrainWidth)) && (y < (offsetTerrainHeight))) {
				
				index = (y * (offsetTerrainHeight)) + x;

				normal.x += normals[index].x;
				normal.y += normals[index].y;
				normal.z += normals[index].z;
				count++;
			}

			//---------------------------------------------------------------- Take the average of the faces touching this vertex
			normal.x = (normal.x / (float)count);
			normal.y = (normal.y / (float)count);
			normal.z = (normal.z / (float)count);

			//---------------------------------------------------------------- Calculate the length of this normal
			length = sqrt((normal.x * normal.x) + (normal.y * normal.y) + (normal.z * normal.z));

			//---------------------------------------------------------------- Get an index to the vertex location in the height map array
			index = (y * m_terrainHeight) + x;

			//---------------------------------------------------------------- Normalize the final shared normal for this vertex and store it in the height map array
			m_heightMap[index].normal.x = (normal.x / length);
			m_heightMap[index].normal.y = (normal.y / length);
			m_heightMap[index].normal.z = (normal.z / length);
		}
	}
}


/*******************************************************************************************************************
	Function that sets a vertex position and colour and the index value and stores this into m_vertices/m_indices
*******************************************************************************************************************/
void Terrain::SetVertexPosition(int position)
{
	static unsigned int index	= 0;

	//---------------------------------------------------------------- Set the position of the vertices
	m_vertices[index].position		= XMFLOAT3(m_heightMap[position].x, m_heightMap[position].y, m_heightMap[position].z);

	//---------------------------------------------------------------- As we are using a blend map & multi-textures and deal with tiled terrain within the shader, we can just set the texture coordinates the same as the vertex x and z position
	m_vertices[index].textureCoord	= XMFLOAT2(m_heightMap[position].x, m_heightMap[position].z);

	//---------------------------------------------------------------- Set the normal positions for lighting calculations - normals are calculated manually in this class
	m_vertices[index].normal		= XMFLOAT3(m_heightMap[position].normal.x, m_heightMap[position].normal.y, m_heightMap[position].normal.z);
	
	//---------------------------------------------------------------- Set the indices
	m_indices[index]				= index;

	//---------------------------------------------------------------- Increment index so we will move to next vertex the next time this function is called
	index++;

	//---------------------------------------------------------------- Resets the index every time we start a new game state
	if (index >= m_vertices.size()) { index = 0; }
}


/*******************************************************************************************************************
	Function that initializes the terrain buffers, prior to sending the data to GPU for rendering
*******************************************************************************************************************/
bool Terrain::InitializeBuffers()
{
	//---------------------------------------------------------------- We do -1 to make the width and height of terrain an odd number, necessary for accurate placement of vertex data
	int offsetTerrainHeight = m_terrainHeight - 1;
	int offsetTerrainWidth	= m_terrainWidth - 1;

	//---------------------------------------------------------------- Calculate number of vertices in terrain mesh (12 points/vertices to make 1 face - 2 x triangles, 6 points per triangle
	unsigned int vertexCount = (offsetTerrainWidth) * (offsetTerrainHeight) * FileConstants::HEIGHTMAP_VERTICES;

	//---------------------------------------------------------------- Set the vertex count of the buffer object
	m_buffer.SetTerrainVertexCount(vertexCount);
	
	//---------------------------------------------------------------- Set the index count to the same as the vertex count (create a temporary indexCount variable for readability purposes only)
	unsigned int indexCount = vertexCount;
	m_buffer.SetTerrainIndexCount(indexCount);

	//---------------------------------------------------------------- Resize vectors to the size of the vertex and index count
	m_vertices.resize(vertexCount);
	m_indices.resize(indexCount);

	VertexLocation vertex = { 0 };

	//---------------------------------------------------------------- Loop through the terrain and place vertices in relevant positions, adding the data to the vertex and index vectors
	for (int y = 0; y < (offsetTerrainHeight); y++) {
		for (int x = 0; x < (offsetTerrainWidth); x++) {
			//---------------------------------------------------------------- Calculate the index coordinates
			vertex.SetCoordinates(m_terrainHeight, y, x);

			//---------------------------------------------------------------- Vertex positions
			SetVertexPosition(vertex.topLeft);
			SetVertexPosition(vertex.topRight);
			SetVertexPosition(vertex.bottomLeft);
			SetVertexPosition(vertex.bottomLeft);
			SetVertexPosition(vertex.topRight);
			SetVertexPosition(vertex.bottomRight);
		}
	}

	//---------------------------------------------------------------- Push the vertex and index data to the GPU
	if (!m_buffer.Push(m_vertices)) { return false; }
	if (!m_buffer.Push(m_indices))	{ return false; }

	return true;
}


/*******************************************************************************************************************
	Function that renders the terrain to the screen
*******************************************************************************************************************/
void Terrain::Render(Camera* camera)
{
	m_terrainShader.Bind(m_transform, camera, &m_packedTextures);
		m_buffer.Render(m_stride, m_offset);
}