#include "DXInput.h"

#include <iostream>

DXInput::DXInput()
{
}


DXInput::~DXInput()
{
	std::cout << "shutting down dxinput" << std::endl;
}

void DXInput::Initialize()
{
	for (int i = 0; i<256; i++)
	{
		m_keys[i] = false;
	}
}

void DXInput::KeyDown(unsigned int key)
{
	m_keys[key] = true;
}

void DXInput::KeyUp(unsigned int key)
{
	m_keys[key] = false;
}

bool DXInput::IsKeyDown(unsigned int key)
{
	return m_keys[key];
}
