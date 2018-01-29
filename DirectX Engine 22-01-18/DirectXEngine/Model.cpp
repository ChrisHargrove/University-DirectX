#include "Model.h"
#include "GraphicsManager.h"
#include "objLoader.h"
#include "Log.h"

Model::Model()	:	m_stride(sizeof(PackedVertex)),
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
	
	std::vector<PackedVertex> packedVertex(vertices.size());

	for (int i = 0; i < vertices.size(); i++) {
		packedVertex[i].position = vertices[i];
		packedVertex[i].textureCoord = textureCoords[i];
		packedVertex[i].normal = normals[i];
	}

	m_buffer.Push(packedVertex);
	m_buffer.Push(indices);
}


void Model::Render() const
{
	//RENDER SHADER
	Graphics::Instance()->GetDeviceContext()->IASetVertexBuffers(0, 1, m_buffer.GetVertexBuffer(), &m_stride, &m_offset);
	Graphics::Instance()->GetDeviceContext()->IASetIndexBuffer(*m_buffer.GetIndexBuffer(), DXGI_FORMAT_R32_UINT, m_offset);
	Graphics::Instance()->GetDeviceContext()->DrawIndexed(m_buffer.GetIndexCount(), 0, 0);
}
