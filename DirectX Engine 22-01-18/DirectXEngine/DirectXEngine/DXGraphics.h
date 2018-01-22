#ifndef DX_GRAPHICS_H_
#define DX_GRAPHICS_H_

#include "Direct3D.h"
#include "Camera.h"
#include "Model.h"
#include "Shader.h"

class DXGraphics
{
public:
	DXGraphics();
	~DXGraphics();

	bool Initialize(HWND appWindow, int clientWidth, int clientHeight, bool vSync, bool fullScreen);
	void Shutdown();
	bool Update();
	bool Render();

private:
	Direct3D* m_direct3D;
	float m_clientNear;
	float m_clientDepth;

	Camera* m_camera;
	Model* m_model;
	Shader* m_shader;


};

#endif

