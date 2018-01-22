#include "DXApplication.h"

#include "Log.h"
#include "Memory.h"

/*******************************************************************************************************************
Pointer used to forward messages to user defined MsgProc function (Callback functions don't work within classes)
*******************************************************************************************************************/
namespace
{
	DXApplication* g_appPointer = nullptr;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (g_appPointer) { return g_appPointer->MsgProc(hwnd, msg, wParam, lParam); }
	else { return DefWindowProc(hwnd, msg, wParam, lParam); }
}


/*******************************************************************************************************************
DirectXApp constructor to initialize default values
*******************************************************************************************************************/
DXApplication::DXApplication(HINSTANCE hInstance) :
	//---------------------------------------------------------------- Set the default values for WIN32 Window
	m_appInstance(hInstance),
	m_appWindow(nullptr),
	m_clientWidth(800),
	m_clientHeight(600),
	m_windowStyle(WINDOWED),
	m_windowTitle("DirectX Application Title"),
	m_windowClass("DXAPPWNDCLASS"),
	//---------------------------------------------------------------- Set the default values for Direct3D device
	m_driverType(D3D_DRIVER_TYPE_NULL),
	m_featureLevel(D3D_FEATURE_LEVEL_11_0),
	m_device(nullptr),
	m_device1(nullptr),
	m_deviceContext(nullptr),
	m_deviceContext1(nullptr),
	m_swapChain(nullptr),
	m_swapChain1(nullptr),
	m_backBuffer(nullptr),
	m_depthStencilView(nullptr),
	m_depthStencilBuffer(nullptr),
	m_depthStencilState(nullptr),
	m_depthDisabledStencilState(nullptr),
	m_alphaEnableBlendingState(nullptr),
	m_alphaDisableBlendingState(nullptr),
	m_pixelShader(nullptr),
	m_vertexShader(nullptr),
	m_vertexLayout(nullptr),
	m_input(nullptr)
{
	g_appPointer = this;

	Log("[DXAPP] DXApplication constructor initialized", EMPTY, LOG_MESSAGE);
}


/*******************************************************************************************************************
Initialize the DirectXApp application and run all startup functions
*******************************************************************************************************************/
bool DXApplication::Initialize()
{
	if (InitializeWindow() == 1)	{ Log("[DXAPP] Failed to initialize window", EMPTY, LOG_ERROR); return false; }
	if (InitializeDirect3D() == 1)	{ Log("[DXAPP] Failed to initialize Direct3D", EMPTY, LOG_ERROR); return false; }
	
	m_input = new DXInput;
	
	if (!m_input) { Log("[DXAPP] Failed to create input object", EMPTY, LOG_ERROR); return false; }

	m_input->Initialize(m_appInstance, m_appWindow);

	return true;
}


/*******************************************************************************************************************
Initialize the WIN32 Window and set its attributes
*******************************************************************************************************************/
HRESULT DXApplication::InitializeWindow()
{
	//---------------------------------------------------------------- Create our window class
	WNDCLASSEX WIN32Window;
	ZeroMemory(&WIN32Window, sizeof(WNDCLASSEX));

	//---------------------------------------------------------------- The windows measurements
	WIN32Window.cbSize = sizeof(WNDCLASSEX);

	//---------------------------------------------------------------- Redraw the window if it is moved vertically or horizontally
	WIN32Window.style = CS_HREDRAW | CS_VREDRAW;

	//---------------------------------------------------------------- What function to use when the class gets a message from Windows
	WIN32Window.lpfnWndProc = WndProc;

	//---------------------------------------------------------------- Number of extra bytes to allocate following the window class structure
	WIN32Window.cbClsExtra = 0;

	//---------------------------------------------------------------- Number of extra bytes to allocate following the window instance
	WIN32Window.cbWndExtra = 0;

	//---------------------------------------------------------------- It is a handle to a copy of our application
	WIN32Window.hInstance = m_appInstance;

	//---------------------------------------------------------------- A handle to the window icon. MAKEINTRESOURCE can be used for custom icons
	WIN32Window.hIcon = LoadIcon(NULL, IDI_APPLICATION);

	//---------------------------------------------------------------- Stores the default mouse cursor for the window
	WIN32Window.hCursor = LoadCursor(nullptr, IDC_ARROW);

	//---------------------------------------------------------------- Contains the 'brush' that will be used to colour the background of our window
	WIN32Window.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

	//---------------------------------------------------------------- For menus
	WIN32Window.lpszMenuName = nullptr;

	//---------------------------------------------------------------- The name of our window class
	WIN32Window.lpszClassName = m_windowClass;

	//---------------------------------------------------------------- A handle to a small icon that is associated with the window
	WIN32Window.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	//---------------------------------------------------------------- Register the window class using the descriptions set above
	if (!RegisterClassEx(&WIN32Window)) { Log("[DXAPP] Failed to create window class", EMPTY, LOG_ERROR); return E_FAIL; }

	RECT windowRect = { 0, 0, (LONG)m_clientWidth, (LONG)m_clientHeight };  
	AdjustWindowRect(&windowRect, WINDOWED, FALSE);   

	//---------------------------------------------------------------- Create the window
	m_appWindow = CreateWindow(m_windowClass, m_windowTitle, m_windowStyle, CW_USEDEFAULT, CW_USEDEFAULT, windowRect.right - windowRect.left, windowRect.bottom - windowRect.top, nullptr, nullptr, m_appInstance, nullptr);

	if (!m_appWindow) { Log("[DXAPP] Failed to create window", EMPTY, LOG_ERROR); return E_FAIL; }

	//---------------------------------------------------------------- Display the window on the screen
	ShowWindow(m_appWindow, SW_SHOW);

	//---------------------------------------------------------------- Brings the thread that created the specified window into the foreground and activates the window
	SetForegroundWindow(m_appWindow);

	//---------------------------------------------------------------- Sets the keyboard focus to the specified window
	SetFocus(m_appWindow);

	Log("[DXAPP] Window created successfully!", EMPTY, LOG_SUCCESS);

	return S_OK;
}


/*******************************************************************************************************************
Initialize the Direct3D Device
*******************************************************************************************************************/
HRESULT DXApplication::InitializeDirect3D()
{
	HRESULT result = S_OK;

	//---------------------------------------------------------------- Get the width and height of our window
	RECT windowRect;
	GetClientRect(m_appWindow, &windowRect);

	//---------------------------------------------------------------- Store the width and height in to our user-defined variables
	m_clientWidth = windowRect.right - windowRect.left;
	m_clientHeight = windowRect.bottom - windowRect.top;

	UINT createDeviceFlags = 0;

#ifdef _DEBUG
	//createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_DRIVER_TYPE driverTypes[] = {
		D3D_DRIVER_TYPE_HARDWARE, D3D_DRIVER_TYPE_WARP, D3D_DRIVER_TYPE_REFERENCE
	};

	D3D_FEATURE_LEVEL featureLevels[] = {
		D3D_FEATURE_LEVEL_11_1, D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_1, D3D_FEATURE_LEVEL_10_0
	};

	//---------------------------------------------------------------- Loop through the available drivers 
	for (UINT driverTypeIndex = 0; driverTypeIndex < _countof(driverTypes); driverTypeIndex++) {

		m_driverType = driverTypes[driverTypeIndex];

		//---------------------------------------------------------------- Create a GPU interface that supports up to Direct X 11.1 
		result = D3D11CreateDevice(nullptr, m_driverType, nullptr, createDeviceFlags, featureLevels, _countof(featureLevels), D3D11_SDK_VERSION, &m_device, &m_featureLevel, &m_deviceContext);

		//---------------------------------------------------------------- Or if unsupported, create a GPU interface that supports up to Direct X 11 
		if (result == E_INVALIDARG) { result = D3D11CreateDevice(nullptr, m_driverType, nullptr, createDeviceFlags, &featureLevels[1], _countof(featureLevels) - 1, D3D11_SDK_VERSION, &m_device, &m_featureLevel, &m_deviceContext); }

		if (SUCCEEDED(result)) { break; }
	}

	if (FAILED(result)) { return result; }

	CreateSwapChain();
	CreateRenderTargetView();
	CreateDepthStencilView();
	SetupViewport();

	Log("[DXAPP] Direct3D initialized successfully!", EMPTY, LOG_SUCCESS);

	return S_OK;
}


/*******************************************************************************************************************
WIN32 function that handles input from user and sends message back to system
*******************************************************************************************************************/
LRESULT DXApplication::MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT paintStruct;
	HDC handleDeviceContext;

	switch (msg) {
		//---------------------------------------------------------------- Prepares the specified window for painting & fills struct with info about painting
	case WM_PAINT: { handleDeviceContext = BeginPaint(hwnd, &paintStruct); EndPaint(hwnd, &paintStruct); break; }

				   //---------------------------------------------------------------- Close the application entirely
	case WM_DESTROY: { PostQuitMessage(0); break; }
	case WM_CLOSE: { PostQuitMessage(0); break; }

				   //---------------------------------------------------------------- Handle any other messages the switch statement didn't
	default: { return DefWindowProc(hwnd, msg, wParam, lParam); }
	}

	return 0;
}


/*******************************************************************************************************************
WIN32 Main message loop that listens for input from user
*******************************************************************************************************************/
int DXApplication::Run()
{
	//---------------------------------------------------------------- A struct that holds windows event messages
	MSG msg = { 0 };

	bool endGame = false;
	bool handleEvents = true;

	while (!endGame) {

		//---------------------------------------------------------------- Dispatch incoming sent messages, check thread message queue for message & retrieves message
		if (PeekMessage(&msg, nullptr, NULL, NULL, PM_REMOVE)) {

			//---------------------------------------------------------------- Translate keystroke messages into the right format
			TranslateMessage(&msg);

			//---------------------------------------------------------------- Send the message to the WndProc function
			DispatchMessage(&msg);
		}

		if (msg.message == WM_QUIT) { endGame = true; }
		else {
			//---------------------------------------------------------------- If Update returns false, handleEvents will return false and the game will quit
			handleEvents = Update(0.0f);
			Render(0.0f);

			if (!handleEvents) { endGame = true; }
		}
	}
	//---------------------------------------------------------------- Return this part of the WM_QUIT message to Windows
	return (int)msg.wParam;
}


/*******************************************************************************************************************
Create swap chain (set up double buffering) depending on Direct X version the user is using
*******************************************************************************************************************/
HRESULT DXApplication::CreateSwapChain()
{
	HRESULT result = S_OK;

	//---------------------------------------------------------------- We retrieve the factory that is used to create the device above, so that we can create our Swap Chain
	IDXGIFactory1* dxgiFactory = nullptr;
	IDXGIDevice* dxgiDevice = nullptr;

	result = m_device->QueryInterface(__uuidof(IDXGIDevice), (void**)(&dxgiDevice));

	if (SUCCEEDED(result)) {

		IDXGIAdapter* adapter = nullptr;
		result = dxgiDevice->GetAdapter(&adapter);

		if (SUCCEEDED(result)) {

			result = adapter->GetParent(__uuidof(IDXGIFactory1), (void**)(&dxgiFactory));
			SafeRelease(adapter);
		}

		SafeRelease(dxgiDevice);
	}

	if (FAILED(result)) { Log("[DXAPP] Error retrieving device factory", EMPTY, LOG_ERROR); return result; }

	IDXGIFactory2* dxgiFactory2 = nullptr;
	result = dxgiFactory->QueryInterface(__uuidof(IDXGIFactory2), (void**)(&dxgiFactory2));

	if (dxgiFactory2) {
		//---------------------------------------------------------------- If the feature level supports up to Direct X 11.1
		result = m_device->QueryInterface(__uuidof(ID3D11Device1), (void**)(&m_device1));

		if (SUCCEEDED(result)) { (void)m_deviceContext->QueryInterface(__uuidof(ID3D11DeviceContext1), (void**)(&m_deviceContext1)); }

		//---------------------------------------------------------------- Implement our Swap Chain description
		DXGI_SWAP_CHAIN_DESC1 swapChainDesc;

		//---------------------------------------------------------------- ZeroMemory to make sure object is cleaned out before use
		ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

		//---------------------------------------------------------------- The width of our resolution we are going to use
		swapChainDesc.Width = m_clientWidth;

		//---------------------------------------------------------------- The height of our resolution we are going to use
		swapChainDesc.Height = m_clientHeight;

		//---------------------------------------------------------------- 8 bits for RGBA values (standard)
		swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

		//---------------------------------------------------------------- The number of multisamples per pixel
		swapChainDesc.SampleDesc.Count = MultiSampleCount;

		//---------------------------------------------------------------- The image quality level
		swapChainDesc.SampleDesc.Quality = MultiSampleQuality;

		//---------------------------------------------------------------- Describes the access the CPU has to surface of the back buffer. We are rendering to it
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

		//---------------------------------------------------------------- The number of back buffers we will use. Set to 1 for double buffering
		swapChainDesc.BufferCount = 1;

		//---------------------------------------------------------------- Create our Swap Chain
		result = dxgiFactory2->CreateSwapChainForHwnd(m_device, m_appWindow, &swapChainDesc, nullptr, nullptr, &m_swapChain1);

		if (SUCCEEDED(result)) { result = m_swapChain1->QueryInterface(__uuidof(IDXGISwapChain), (void**)(&m_swapChain)); }

		SafeRelease(dxgiFactory2);
	}
	else {
		//---------------------------------------------------------------- If the feature level supports up to Direct X 11
		DXGI_SWAP_CHAIN_DESC swapChainDesc;
		ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
		swapChainDesc.BufferDesc.Width = m_clientWidth;
		swapChainDesc.BufferDesc.Height = m_clientHeight;
		swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swapChainDesc.SampleDesc.Count = MultiSampleCount;
		swapChainDesc.SampleDesc.Quality = MultiSampleQuality;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.BufferCount = 1;

		//---------------------------------------------------------------- Describing the refresh rate in hertz. Set to 60/1 or 60hz
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;

		//---------------------------------------------------------------- The handle to our window
		swapChainDesc.OutputWindow = m_appWindow;

		//---------------------------------------------------------------- Windowed is true or false for fullscreen
		swapChainDesc.Windowed = TRUE;

		//---------------------------------------------------------------- Create our Swap Chain
		result = dxgiFactory->CreateSwapChain(m_device, &swapChainDesc, &m_swapChain);
	}

	//---------------------------------------------------------------- Disable Alt+Enter shortcut
	dxgiFactory->MakeWindowAssociation(m_appWindow, DXGI_MWA_NO_ALT_ENTER);

	SafeRelease(dxgiFactory);

	if (FAILED(result)) { Log("[DXAPP] Error retrieving device factory", EMPTY, LOG_ERROR); return result; }

	return S_OK;
}


/*******************************************************************************************************************
A function that creates the back buffer and sets up the rasterizer
*******************************************************************************************************************/
HRESULT DXApplication::CreateRenderTargetView()
{
	HRESULT result = S_OK;

	D3D11_RASTERIZER_DESC RasterizerDesc;
	ZeroMemory(&RasterizerDesc, sizeof(RasterizerDesc));
	RasterizerDesc.FillMode = D3D11_FILL_SOLID;
	//RasterizerDesc.FillMode = D3D11_FILL_WIREFRAME;
	RasterizerDesc.CullMode = D3D11_CULL_FRONT;
	//RasterizerDesc.CullMode = D3D11_CULL_NONE;
	RasterizerDesc.FrontCounterClockwise = TRUE;
	RasterizerDesc.DepthBias = 0;
	RasterizerDesc.DepthBiasClamp = 0;
	RasterizerDesc.SlopeScaledDepthBias = 0;
	RasterizerDesc.DepthClipEnable = TRUE;
	RasterizerDesc.ScissorEnable = FALSE;
	RasterizerDesc.MultisampleEnable = TRUE;
	//RasterizerDesc.AntialiasedLineEnable = TRUE;
	RasterizerDesc.AntialiasedLineEnable = FALSE;

	ID3D11RasterizerState *rasterizerState = nullptr;
	result = m_device->CreateRasterizerState(&RasterizerDesc, &rasterizerState);

	m_deviceContext->RSSetState(rasterizerState);
	
	SafeRelease(rasterizerState);

	//---------------------------------------------------------------- Create back buffer, which we will use to create our render target view
	ID3D11Texture2D* backBuffer = nullptr;

	result = m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBuffer);

	if (FAILED(result)) { Log("[DXAPP] Error retrieving swap chain buffer", EMPTY, LOG_ERROR); return result; }

	//---------------------------------------------------------------- Create render target view
	result = m_device->CreateRenderTargetView(backBuffer, nullptr, &m_backBuffer);
	SafeRelease(backBuffer);

	if (FAILED(result)) { Log("[DXAPP] Error creating render target view", EMPTY, LOG_ERROR); return result; }

	return S_OK;
}


/*******************************************************************************************************************
Function that creates the depth stencil view
*******************************************************************************************************************/
HRESULT DXApplication::CreateDepthStencilView()
{
	HRESULT result = S_OK;

	D3D11_TEXTURE2D_DESC descDepth;
	ZeroMemory(&descDepth, sizeof(descDepth));
	descDepth.Width = m_clientWidth;
	descDepth.Height = m_clientHeight;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	descDepth.SampleDesc.Count = MultiSampleCount;
	descDepth.SampleDesc.Quality = MultiSampleQuality;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;
	result = m_device->CreateTexture2D(&descDepth, nullptr, &m_depthStencilBuffer);

	if (FAILED(result)) { Log("[DXAPP] Error creating the depth stencil buffer", EMPTY, LOG_ERROR); return result; }

	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	ZeroMemory(&descDSV, sizeof(descDSV));
	descDSV.Format = descDepth.Format;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
	descDSV.Texture2D.MipSlice = 0;
	result = m_device->CreateDepthStencilView(m_depthStencilBuffer, &descDSV, &m_depthStencilView);
	
	if (FAILED(result)) { Log("[DXAPP] Error creating the depth stencil view", EMPTY, LOG_ERROR); return result; }

	//---------------------------------------------------------------- Create the enabled depth stencil state
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	//---------------------------------------------------------------- Stencil operations if pixel is front-facing
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	//---------------------------------------------------------------- Stencil operations if pixel is back-facing
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	
	result = m_device->CreateDepthStencilState(&depthStencilDesc, &m_depthStencilState);
	if (FAILED(result)) { Log("[DXAPP] Error creating enabled depth stencil state", EMPTY, LOG_ERROR); return result; }

	m_deviceContext->OMSetDepthStencilState(m_depthStencilState, 1);

	//---------------------------------------------------------------- Set the depth stencil state
	m_deviceContext->OMSetRenderTargets(1, &m_backBuffer, m_depthStencilView);

	//---------------------------------------------------------------- Create the disabled depth stencil state, which turns of Z buffer for 2D rendering
	//---------------------------------------------------------------- The only difference is that DepthEnable is set to false
	D3D11_DEPTH_STENCIL_DESC depthDisabledStencilDesc;
	ZeroMemory(&depthDisabledStencilDesc, sizeof(depthDisabledStencilDesc));
	depthDisabledStencilDesc.DepthEnable = false;
	depthDisabledStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthDisabledStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthDisabledStencilDesc.StencilEnable = true;
	depthDisabledStencilDesc.StencilReadMask = 0xFF;
	depthDisabledStencilDesc.StencilWriteMask = 0xFF;
	depthDisabledStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthDisabledStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	depthDisabledStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthDisabledStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	result = m_device->CreateDepthStencilState(&depthDisabledStencilDesc, &m_depthDisabledStencilState);
	if (FAILED(result)) { Log("[DXAPP] Error creating disabled depth stencil state", EMPTY, LOG_ERROR); return result; }

	return S_OK;
}


/*******************************************************************************************************************
Function that creates a viewport
*******************************************************************************************************************/
void DXApplication::SetupViewport()
{	//---------------------------------------------------------------- Create viewport descriptions
	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));
	viewport.Width = (FLOAT)m_clientWidth;
	viewport.Height = (FLOAT)m_clientHeight;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;

	//---------------------------------------------------------------- Set the viewport. First parameter is the number of viewports, second parameter is the address to a viewport or viewport array
	m_deviceContext->RSSetViewports(1, &viewport); 

	Log("[DXAPP] Viewport created successfully!", EMPTY, LOG_SUCCESS);
}


/*******************************************************************************************************************
Function that compiles a shader (.fx) file and checks for errors
*******************************************************************************************************************/
HRESULT DXApplication::CompileShaderFromFile(WCHAR* fileLocation, LPCSTR entryPoint, LPCSTR shaderVersion, ID3DBlob** errorBuffer)
{
	HRESULT result = S_OK;

	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;

#ifdef _DEBUG
	dwShaderFlags |= D3DCOMPILE_DEBUG;
	dwShaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

	ID3DBlob* errorBlob = nullptr;

	//---------------------------------------------------------------- Compile a shader file and store any errors in to the error buffer
	result = D3DCompileFromFile(fileLocation, nullptr, nullptr, entryPoint, shaderVersion,
								dwShaderFlags, 0, errorBuffer, &errorBlob);

	if (FAILED(result)) {
		
		Log("[DXAPP] Error compiling shader file", EMPTY, LOG_ERROR);
		
		if (errorBlob) {

			OutputDebugStringA((const char*)(errorBlob->GetBufferPointer()));
			Log("[DXAPP] Shader Error: ", errorBlob->GetBufferPointer(), LOG_ERROR);
			errorBlob->Release();
		}

		return result;
	}

	if (errorBlob) { errorBlob->Release(); }

	return S_OK;
}


/*******************************************************************************************************************
Function that loads in a vertex shader (.fx) file
*******************************************************************************************************************/
HRESULT DXApplication::LoadVertexShader(WCHAR* fileLocation)
{
	HRESULT result = S_OK;

	ID3DBlob* vertexShaderBlob = nullptr;

	//---------------------------------------------------------------- Compile vertex shader
	result = CompileShaderFromFile(fileLocation, "VS_Main", "vs_4_0", &vertexShaderBlob);

	if (FAILED(result)) {

		MessageBox(nullptr,
			"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", "Error", MB_OK);
		return result;
	}

	//---------------------------------------------------------------- Create Vertex Shader and store it into our vertex shader object
	result = m_device->CreateVertexShader(vertexShaderBlob->GetBufferPointer(), vertexShaderBlob->GetBufferSize(), nullptr, &m_vertexShader);

	if (FAILED(result)) { Log("[DXAPP] Couldn't create Vertex Shader", EMPTY, LOG_ERROR); SafeRelease(vertexShaderBlob); return result; }

	//---------------------------------------------------------------- Define the input layout
	D3D11_INPUT_ELEMENT_DESC layout[] = {
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};

	//---------------------------------------------------------------- Create the input layout
	result = m_device->CreateInputLayout(layout, _countof(layout), vertexShaderBlob->GetBufferPointer(),
										vertexShaderBlob->GetBufferSize(), &m_vertexLayout);

	SafeRelease(vertexShaderBlob);

	if (FAILED(result)) { Log("[DXAPP] Couldn't create input layout", EMPTY, LOG_ERROR); return result; }

	//---------------------------------------------------------------- Set the input layout
	m_deviceContext->IASetInputLayout(m_vertexLayout);

	return S_OK;
}


/*******************************************************************************************************************
Function that loads in a pixel shader (.fx) file
*******************************************************************************************************************/
HRESULT DXApplication::LoadPixelShader(WCHAR* fileLocation)
{
	HRESULT result = S_OK;

	ID3DBlob* pixelShaderBlob = nullptr;

	//---------------------------------------------------------------- Compile pixel shader
	result = CompileShaderFromFile(fileLocation, "PS_Main", "ps_4_0", &pixelShaderBlob);

	if (FAILED(result)) {

		MessageBox(nullptr,
			"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", "Error", MB_OK);
		return result;
	}

	//---------------------------------------------------------------- Create Pixel Shader and store it into our pixel shader object
	result = m_device->CreatePixelShader(pixelShaderBlob->GetBufferPointer(), pixelShaderBlob->GetBufferSize(), nullptr, &m_pixelShader);
	
	SafeRelease(pixelShaderBlob);

	if (FAILED(result)) { Log("[DXAPP] Couldn't create Pixel Shader", EMPTY, LOG_ERROR); return result; }

	return S_OK;
}


/*******************************************************************************************************************
User defined cleanup function that deletes all objects and shuts down the DX application safely
*******************************************************************************************************************/
void DXApplication::Shutdown()
{
	//---------------------------------------------------------------- Safe release all objects and reset pointers to nullptr
	if (m_deviceContext) { m_deviceContext->ClearState(); }
	SafeDelete(m_input);
	SafeRelease(m_vertexBuffer);
	SafeRelease(m_indexBuffer);
	SafeRelease(m_vertexLayout);
	SafeRelease(m_vertexShader);
	SafeRelease(m_pixelShader);
	SafeRelease(m_alphaDisableBlendingState);
	SafeRelease(m_alphaEnableBlendingState);
	SafeRelease(m_depthDisabledStencilState);
	SafeRelease(m_depthStencilState);
	SafeRelease(m_depthStencilBuffer);
	SafeRelease(m_depthStencilView);
	SafeRelease(m_backBuffer);
	SafeRelease(m_swapChain1);
	SafeRelease(m_swapChain);
	SafeRelease(m_deviceContext1);
	SafeRelease(m_deviceContext);
	SafeRelease(m_device1);
	SafeRelease(m_device);

	//---------------------------------------------------------------- Destroy the WIN32 Window
	DestroyWindow(m_appWindow);
	m_appWindow = nullptr;

	//---------------------------------------------------------------- Remove the application instance
	UnregisterClass(m_windowClass, m_appInstance);
	m_appInstance = nullptr;

	Log("[DXAPP] Device shut down successfully!", EMPTY, LOG_SUCCESS);
}


/*******************************************************************************************************************
DirectXApp destructor used for cleaning up memory, deleting all objects and calling shutdown functions
*******************************************************************************************************************/
DXApplication::~DXApplication()
{
	Shutdown();

	Log("[DXAPP] DXApplication destructor initialized", EMPTY, LOG_MESSAGE);
}
