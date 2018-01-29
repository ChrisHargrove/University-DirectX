#include <fstream>
#include "Terrain.h"
#include "GraphicsManager.h"
#include "Constants.h"
#include "Camera.h"
#include "Log.h"

/*******************************************************************************************************************
	Struct that defines the index positions
*******************************************************************************************************************/
struct IndexType
{
	int bottomLeft, bottomRight, topLeft, topRight;

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
						m_stride(sizeof(TerrainVertexType)),
						m_offset(0),
						m_localTransform(XMMatrixIdentity())
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
	//---------------------------------------------------------------- Load in the shaders used for the terrain
	if (!m_basicShader.LoadShader(L"Assets\\Shaders\\basicShader.vs", L"Assets\\Shaders\\basicShader.ps")) { return false; }

	//---------------------------------------------------------------- Load in the heightmap for the terrain
	if (!LoadHeightMap(fileLocation)) { return false; }

	//---------------------------------------------------------------- Normalize the heightmap
	NormalizeHeightMap();

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
	m_terrainWidth = bitmapInfoHeader.biWidth;
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
	for (unsigned int z = 0; z < m_terrainHeight; z++)
	{
		for (unsigned int x = 0; x <m_terrainWidth; x++)
		{
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
	Function that normalizes a heightmap
*******************************************************************************************************************/
void Terrain::NormalizeHeightMap()
{
	int i, j;


	for (j = 0; j<m_terrainHeight; j++)
	{
		for (i = 0; i<m_terrainWidth; i++)
		{
			m_heightMap[(m_terrainHeight * j) + i].y /= 15.0f;
		}
	}

	return;
}


/*******************************************************************************************************************
	Function that sets a vertex position and colour and the index value and stores this into m_vertices/m_indices
*******************************************************************************************************************/
void Terrain::SetVertexPosition(int position)
{
	static int index = 0;

	//---------------------------------------------------------------- Resets the index every time we start a new game state
	if (index >= m_vertices.size()) { index = 0; }

	m_vertices[index].position	= XMFLOAT3(m_heightMap[position].x, m_heightMap[position].y, m_heightMap[position].z);
	m_vertices[index].color		= XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	m_indices[index]			= index;
	index++;
}


/*******************************************************************************************************************
	Function that initializes the terrain buffers, prior to sending the data to GPU for rendering
*******************************************************************************************************************/
bool Terrain::InitializeBuffers()
{
	//---------------------------------------------------------------- Calculate number of vertices in terrain mesh (12 points/vertices to make 1 face - 2 x triangles, 6 points per triangle
	unsigned int vertexCount = (m_terrainWidth - 1) * (m_terrainHeight - 1) * FileConstants::HEIGHTMAP_VERTICES;

	//---------------------------------------------------------------- Set the vertex count of the buffer object
	m_buffer.SetTerrainVertexCount(vertexCount);
	
	//---------------------------------------------------------------- Set the index count to the same as the vertex count (create a temporary indexCount variable for readability purposes only)
	unsigned int indexCount = vertexCount;
	m_buffer.SetTerrainIndexCount(indexCount);

	//---------------------------------------------------------------- Resize vectors to the size of the vertex and index count
	m_vertices.resize(vertexCount);
	m_indices.resize(indexCount);

	IndexType index = { 0 };

	//---------------------------------------------------------------- Loop through the terrain and place vertices in relevant positions, adding the data to the vertex and index vectors
	for (unsigned int y = 0; y < (m_terrainHeight - 1); y++)
	{
		for (unsigned int x = 0; x < (m_terrainWidth - 1); x++)
		{
			//---------------------------------------------------------------- Calculate the index coordinates
			index.SetCoordinates(m_terrainHeight, y, x);

			//---------------------------------------------------------------- First triangle vertex positions
			SetVertexPosition(index.topLeft);
			SetVertexPosition(index.topRight);
			SetVertexPosition(index.topRight);
			SetVertexPosition(index.bottomLeft);
			SetVertexPosition(index.bottomLeft);
			SetVertexPosition(index.topLeft);

			//---------------------------------------------------------------- Second triangle vertex positions
			SetVertexPosition(index.bottomLeft);
			SetVertexPosition(index.topRight);
			SetVertexPosition(index.topRight);
			SetVertexPosition(index.bottomRight);
			SetVertexPosition(index.bottomRight);
			SetVertexPosition(index.bottomLeft);
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
	m_basicShader.Bind(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
	m_basicShader.UpdateConstantBuffers(m_localTransform, camera);

	Graphics::Instance()->GetDeviceContext()->IASetVertexBuffers(0, 1, m_buffer.GetVertexBuffer(), &m_stride, &m_offset);
	Graphics::Instance()->GetDeviceContext()->IASetIndexBuffer(*m_buffer.GetIndexBuffer(), DXGI_FORMAT_R32_UINT, 0);
	Graphics::Instance()->GetDeviceContext()->DrawIndexed(m_buffer.GetIndexCount(), 0, 0);
}