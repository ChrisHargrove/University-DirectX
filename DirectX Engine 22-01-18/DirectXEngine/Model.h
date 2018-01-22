#pragma once

#include <d3d11.h>
#include <vector>
#include "Buffer.h"

class Model {

public:
	Model();
	~Model();

	bool Load(char* FileName, ID3D11Device* Device);

	void Render(ID3D11DeviceContext* deviceContext) const;
	void Update(); //May not need

private:
	Buffer			m_buffer;
	unsigned int	m_stride;
	unsigned int	m_offset;
};

