#include "Model.h"
#include "GraphicsManager.h"
#include "objLoader.h"
#include "Constants.h"
#include "Log.h"

Model::Model()	:	m_stride(sizeof(BufferConstants::PackedVertex)),
					m_offset(0)
{

}


Model::~Model()
{

}

bool Model::Load(const char* fileLocation)
{
	std::vector<XMFLOAT3> vertices;
	std::vector<XMFLOAT2> textureCoords;
	std::vector<XMFLOAT3> normals;
	std::vector<unsigned int> indices;

	//Create Obj Model to load to.
	ObjLoader objLoader;

	if (!objLoader.LoadObjFile(fileLocation, vertices, textureCoords, normals, indices)) {
		DX_LOG("[MODEL] Couldn't load OBJ file: ", fileLocation, LOG_ERROR);
		return false;
	}
	
	std::vector<BufferConstants::PackedVertex> packedVertex(vertices.size());

	for (unsigned int i = 0; i < vertices.size(); i++) {
		packedVertex[i].position		= vertices[i];
		packedVertex[i].textureCoord	= textureCoords[i];
		packedVertex[i].normal			= normals[i];
	}

	m_buffer.Push(packedVertex);
	m_buffer.Push(indices);

	return true;
}


void Model::Render() const
{
	m_buffer.Render(m_stride, m_offset);
}
