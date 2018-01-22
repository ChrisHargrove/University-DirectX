#include "ObjLoader.h"
#include "FileManager.h"
#include "Constants.h"
#include "Log.h"
#include "Tools.h"

/*******************************************************************************************************************
	Constructor with initializer list to set default values of data members
*******************************************************************************************************************/
ObjLoader::ObjLoader()
{
	Log("[OBJ LOADER] OBJ Loader constructor initialized", EMPTY, LOG_MESSAGE);
}


/*******************************************************************************************************************
	Cleanup all memory usage, delete all objects and shut down all devices
*******************************************************************************************************************/
ObjLoader::~ObjLoader()
{
	Log("[OBJ LOADER] OBJ Loader destructor initialized", EMPTY, LOG_MESSAGE);
}


/*******************************************************************************************************************
	Load an OBJ file and store the contents into the vector containers passed in
*******************************************************************************************************************/
bool ObjLoader::LoadObjFile(const char* fileLocation, std::vector<XMFLOAT3>& outVertices, std::vector<XMFLOAT2>& outTextureCoords, std::vector<XMFLOAT3>& outNormals, std::vector<unsigned int>& outIndices)
{
	//---------------------------------------------------------------- Open the OBJ file for reading only
	if (!File::Instance()->OpenForReading(fileLocation)) { return false; }

	//---------------------------------------------------------------- Generate temporary vectors to store the faces/indices data
	std::vector<unsigned int> vertexIndices, textureCoordIndices, normalIndices;

	//---------------------------------------------------------------- Generate temporary vectors to store the other obj data
	std::vector<XMFLOAT3> inVertices;
	std::vector<XMFLOAT2> inTextureCoords;
	std::vector<XMFLOAT3> inNormals;

	//---------------------------------------------------------------- Get all the data from the file
	while (File::Instance()->ExtractFileData()) {

		//---------------------------------------------------------------- Check if the file contains "v", "vt", or "vn", remove this part of the string and read in the object data only
		if		(File::Instance()->FileDataContains(FileConstants::Vertices))		{ GetVertices(inVertices); }
		else if (File::Instance()->FileDataContains(FileConstants::TextureCoords))	{ GetTextureCoords(inTextureCoords); }
		else if (File::Instance()->FileDataContains(FileConstants::Normals))		{ GetNormals(inNormals); }
		else if (File::Instance()->FileDataContains(FileConstants::Faces))			{ GetIndices(vertexIndices, textureCoordIndices, normalIndices); }
	}

	//---------------------------------------------------------------- We then need to calibrate the indices (-1 all indices) before we push the data in to the m_vertices, m_textureCoords and m_normals vectors, because arrays in C++ start from 0, and OBJ files start from 1
	CalibrateIndices(inVertices, m_vertices, vertexIndices);
	CalibrateIndices(inTextureCoords, m_textureCoords, textureCoordIndices);
	CalibrateIndices(inNormals, m_normals, normalIndices);

	//---------------------------------------------------------------- As we want to draw elements index-based (to avoid duplicated vertex data), we then call this function, which checks for multiple vertex data and then finally pushes the data to the out vectors
	PushData(outVertices, outTextureCoords, outNormals, outIndices);

	//---------------------------------------------------------------- Close the file once we are finished with it. This is necessary as other OBJ files cannot be loaded as long as the file remains open
	File::Instance()->Close(fileLocation);

	return true;
}


/*******************************************************************************************************************
	Function that finalizes the file data, disregards duplicate vertices and pushes it to the vectors passed in
*******************************************************************************************************************/
void ObjLoader::PushData(std::vector<XMFLOAT3>& outVertices, std::vector<XMFLOAT2>& outTextureCoords, std::vector<XMFLOAT3>& outNormals, std::vector<unsigned int>& outIndices)
{
	std::map<PackedVertex, unsigned int> vertexToOutIndex;

	//---------------------------------------------------------------- For each vertex stored in our m_vertices vector
	for (unsigned int i = 0; i < m_vertices.size(); i++) {

		PackedVertex packed = { m_vertices[i], m_textureCoords[i], m_normals[i] };

		//---------------------------------------------------------------- Try to find a similar vertex in our vectors
		unsigned int index;
		bool found = GetSimilarVertexIndex(packed, vertexToOutIndex, index);

		//---------------------------------------------------------------- If we find a similar vertex, use that instead
		if (found) { outIndices.push_back(index); }

		//---------------------------------------------------------------- If not, push the data to the vectors passed in
		else {
			outVertices.push_back(m_vertices[i]);
			outTextureCoords.push_back(m_textureCoords[i]);
			outNormals.push_back(m_normals[i]);
			unsigned int newIndex = (unsigned int)outVertices.size() - 1;
			outIndices.push_back(newIndex);
			vertexToOutIndex[packed] = newIndex;
		}
	}
	
	//---------------------------------------------------------------- Clear the vectors so we don't over-burden them with data from all the models we load
	m_vertices.clear();
	m_textureCoords.clear();
	m_normals.clear();
}


/*******************************************************************************************************************
	Function that checks if we have a similar vertex already stored
*******************************************************************************************************************/
bool ObjLoader::GetSimilarVertexIndex(PackedVertex& packed, std::map<PackedVertex, unsigned int>& vertexToOutIndex, unsigned int& result)
{
	//---------------------------------------------------------------- Loop through the map, looking for a vertex
	std::map<PackedVertex, unsigned int>::iterator it = vertexToOutIndex.find(packed);

	//---------------------------------------------------------------- If vertex is not found in the map, then we need to add it in the IndexVBO function, so return false
	if (it == vertexToOutIndex.end()) { return false; }

	//---------------------------------------------------------------- Otherwise, if the vertex is found in the map, then we just return this vertex and don't create another one
	else { result = it->second; }

	return true;
}


/*******************************************************************************************************************
	Function that gets all the vertices from the OBJ file and stores them in to the vector passed in
*******************************************************************************************************************/
void ObjLoader::GetVertices(std::vector<XMFLOAT3>& inVertices)
{
	XMFLOAT3 vertex;

	//---------------------------------------------------------------- Disregard the "v" and the space after it, and store this new string in to a temporary string
	std::string vertices = File::Instance()->CutFileData(FileConstants::VERTICES_TITLE_OFFSET);

	//---------------------------------------------------------------- Store the temporary string we just created (which now only contains the vertices) in to the fileData stream
	std::stringstream fileData(vertices);

	//---------------------------------------------------------------- Add x, y and z data in our fileData to the x, y and z of our vertex
	fileData >> vertex.x >> vertex.y >> vertex.z;

	//---------------------------------------------------------------- Add all of the vertex data from the file to our temporary vertices vector
	inVertices.push_back(vertex);
}


/*******************************************************************************************************************
	Function that gets all the texture coordinates from the OBJ file and stores them in to the vector passed in
*******************************************************************************************************************/
void ObjLoader::GetTextureCoords(std::vector<XMFLOAT2>& inTextureCoords)
{
	XMFLOAT2 textureCoord;

	//---------------------------------------------------------------- Disregard the "vt" and the space after it, and store this new string in to a temporary string
	std::string textureCoords = File::Instance()->CutFileData(FileConstants::TEXTURE_COORDS_TITLE_OFFSET);

	//---------------------------------------------------------------- Store the temporary string we just created (which now only contains the texture coordinates) in to the fileData stream
	std::stringstream fileData(textureCoords);

	//---------------------------------------------------------------- Add s(x) and t(y) data in our fileData to the s and t of our texture coordinate
	fileData >> textureCoord.x >> textureCoord.y;

	//---------------------------------------------------------------- Add all of the texture coordinate data from the file to our temporary texture coords vector
	inTextureCoords.push_back(textureCoord);
}


/*******************************************************************************************************************
	Function that gets all the normals from the OBJ file and stores them in to the vector passed in
*******************************************************************************************************************/
void ObjLoader::GetNormals(std::vector<XMFLOAT3>& inNormals)
{
	XMFLOAT3 normal;

	//---------------------------------------------------------------- Disregard the "vn" and the space after it, and store this new string in to a temporary string
	std::string normals = File::Instance()->CutFileData(FileConstants::NORMALS_TITLE_OFFSET);

	//---------------------------------------------------------------- Store the temporary string we just created (which now only contains the normals) in to the fileData stream
	std::stringstream fileData(normals);

	//---------------------------------------------------------------- Add x, y and z data in our fileData to the x, y and z of our normal
	fileData >> normal.x >> normal.y >> normal.z;

	//---------------------------------------------------------------- Add all of the normal data from the file to our temporary normals vector
	inNormals.push_back(normal);
}


/*******************************************************************************************************************
	Function that gets all the indices from the OBJ file and stores them in to the vectors passed in
*******************************************************************************************************************/
void ObjLoader::GetIndices(std::vector<unsigned int>& vertexIndices, std::vector<unsigned int>& textureCoordIndices, std::vector<unsigned int>& normalIndices)
{
	XMFLOAT3 vertexIndex, textureCoordIndex, normalIndex;
	
	std::string faces = File::Instance()->CutFileData(FileConstants::FACES_TITLE_OFFSET);

	//---------------------------------------------------------------- We replace the "/" with a space, so that the stream reads in every value before or after the space (quick fix - there's better ways of doing this)
	ReplaceCharacter(faces, '/', ' ');
	
	std::stringstream fileData(faces);

	//---------------------------------------------------------------- Put all the file data in to relevant x, y and z
	fileData	>> vertexIndex.x >> textureCoordIndex.x >> normalIndex.x
				>> vertexIndex.y >> textureCoordIndex.y >> normalIndex.y
				>> vertexIndex.z >> textureCoordIndex.z >> normalIndex.z;

	//---------------------------------------------------------------- Loop through all the index values obtained above, and store them in to our faces (indices) vectors
		vertexIndices.push_back(vertexIndex.x);
		textureCoordIndices.push_back(textureCoordIndex.x);
		normalIndices.push_back(normalIndex.x);

		vertexIndices.push_back(vertexIndex.y);
		textureCoordIndices.push_back(textureCoordIndex.y);
		normalIndices.push_back(normalIndex.y);

		vertexIndices.push_back(vertexIndex.z);
		textureCoordIndices.push_back(textureCoordIndex.z);
		normalIndices.push_back(normalIndex.z);
}