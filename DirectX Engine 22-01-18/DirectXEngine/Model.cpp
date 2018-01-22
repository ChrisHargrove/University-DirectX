#include "Model.h"
#include "objLoader.h"


Model::Model()	:	m_stride(sizeof(PackedVertex)),
					m_offset(0)
{

}


Model::~Model()
{

}

bool Model::Load(char* FileName, ID3D11Device* device)
{
	std::vector<XMFLOAT3> vertices;
	std::vector<XMFLOAT2> textureCoords;
	std::vector<XMFLOAT3> normals;
	std::vector<unsigned int> indices;

	//Create Obj Model to load to.
	ObjLoader model;

	if (!model.LoadObjFile(FileName, vertices, textureCoords, normals, indices)) {
		//DXTRACE_MSG("Error loading 3D model!");
		return false;
	}
	
	std::vector<PackedVertex> packedVertex(vertices.size());

	for (int i = 0; i < vertices.size(); i++) {
		packedVertex[i].position = vertices[i];
		packedVertex[i].textureCoord = textureCoords[i];
		packedVertex[i].normal = normals[i];
	}

	m_buffer.Push(device, packedVertex);
	m_buffer.Push(device, indices);
}


void Model::Render(ID3D11DeviceContext* deviceContext) const
{
	deviceContext->IASetVertexBuffers(0, 1, m_buffer.GetVertexBuffer(), &m_stride, &m_offset);
	deviceContext->IASetIndexBuffer(*m_buffer.GetIndexBuffer(), DXGI_FORMAT_R32_UINT, m_offset);

	deviceContext->DrawIndexed(m_buffer.GetIndexCount(), 0, 0);
}
