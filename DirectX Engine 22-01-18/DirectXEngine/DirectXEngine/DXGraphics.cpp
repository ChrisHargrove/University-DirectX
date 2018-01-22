#include "DXGraphics.h"

#include "Log.h"


DXGraphics::DXGraphics() : m_direct3D(nullptr), m_clientNear(0.01f), m_clientDepth(100.0f), m_camera(nullptr), m_model(nullptr), m_shader(nullptr)
{
}


DXGraphics::~DXGraphics()
{
}

bool DXGraphics::Initialize(HWND appWindow, int clientWidth, int clientHeight, bool vSync, bool fullScreen)
{

	HRESULT result;
	// Create the Direct3D object.
	m_direct3D = new Direct3D;
	if (!m_direct3D) { return false; }

	// Initialize the Direct3D object.
	result = m_direct3D->Initialize(appWindow, clientWidth, clientHeight, vSync, fullScreen, m_clientNear, m_clientDepth);
	if (FAILED(result))
	{
		Log("[DXAPP] cant init device in graphics intiialzie function!", EMPTY, LOG_SUCCESS);
		return false;
	}

	// Create the camera object.
	m_camera = new Camera;
	if (!m_camera) { return false; }

	// Set the initial position of the camera.
	m_camera->SetPosition(0.0f, 0.0f, -10.0f);

	// Create the model object.
	m_model = new Model; 
	if (!m_model) { return false; }

	// Initialize the model object.
	result = m_model->Initialize(m_direct3D->GetDevice());
	if (FAILED(result))
	{
		Log("[DXAPP] cant init model in graphics init function", EMPTY, LOG_SUCCESS);
		return false;
	}

	// Create the color shader object.
	m_shader = new Shader;
	if (!m_shader) { return false; }

	// Initialize the color shader object.
	result = m_shader->Initialize(m_direct3D->GetDevice(), appWindow);
	if (FAILED(result))
	{
		Log("[DXAPP] cant init shadel in graphics init function", EMPTY, LOG_SUCCESS);
		return false;
	}



	return true;
}

void DXGraphics::Shutdown()
{
	std::cout << "shutting down graphics" << std::endl;
	if (m_direct3D)
	{
		m_direct3D->Shutdown();
		delete m_direct3D;
		m_direct3D = nullptr;
	}

	// Release the color shader object.
	if (m_shader)
	{
		m_shader->Shutdown();
		delete m_shader;
		m_shader = nullptr;
	}

	// Release the model object.
	if (m_model)
	{
		m_model->Shutdown();
		delete m_model;
		m_model = nullptr;
	}

	// Release the camera object.
	if (m_camera)
	{
		delete m_camera;
		m_camera = nullptr;
	}
}

bool DXGraphics::Update()
{


	return true;
}

bool DXGraphics::Render()
{
	XMMATRIX viewMatrix, projectionMatrix, worldMatrix;
	HRESULT result;

	// Clear the buffers to begin the scene.
	m_direct3D->BeginScene(0.3f, 0.0f, 0.5f, 1.0f);

	//m_camera->Render();

	//m_camera->GetViewMatrix(viewMatrix);
	m_direct3D->GetWorldMatrix(worldMatrix);
	m_direct3D->GetViewMatrix(viewMatrix);
	m_direct3D->GetProjectionMatrix(projectionMatrix);

	m_model->Render(m_direct3D->GetDeviceContext());

	result = m_shader->Render(m_direct3D->GetDeviceContext(), m_model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix);

	if (FAILED(result)) { std::cout << "shader cant be rendered" << std::endl; }
	// Present the rendered scene to the screen.
	m_direct3D->EndScene();

	return true;
}
