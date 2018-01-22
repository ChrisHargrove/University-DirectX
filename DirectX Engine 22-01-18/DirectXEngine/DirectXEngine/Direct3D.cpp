#include "Direct3D.h"

#include "Log.h"
#include "Memory.h"

Direct3D::Direct3D() :
	m_device(nullptr),
	m_device1(nullptr),
	m_deviceContext(nullptr),
	m_deviceContext1(nullptr),
	m_swapChain(nullptr),
	m_swapChain1(nullptr),
	m_backBuffer(nullptr),
	m_depthStencilBuffer(nullptr),
	m_depthStencilState(nullptr),
	m_depthStencilView(nullptr),
	m_rasterState(nullptr)
{
}


Direct3D::~Direct3D()
{
}

HRESULT Direct3D::Initialize(HWND appWindow, int clientWidth, int clientHeight, bool vSync, bool fullScreen, float clientNear, float clientDepth)
{
	HRESULT result;

	RECT windowRect;
	GetClientRect(appWindow, &windowRect);
	clientWidth = windowRect.right - windowRect.left;
	clientHeight = windowRect.bottom - windowRect.top;

	m_vSync = vSync;
	if(m_vSync) { std::cout << "vsync enabled" << std::endl; }
	else { std::cout << "vsync disabled" << std::endl; }

	UINT createDeviceFlags = NULL;

#ifdef _DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_DRIVER_TYPE driverTypes[] = {
		D3D_DRIVER_TYPE_HARDWARE, D3D_DRIVER_TYPE_WARP, D3D_DRIVER_TYPE_REFERENCE
	};

	D3D_FEATURE_LEVEL featureLevels[] = {
		D3D_FEATURE_LEVEL_11_1, D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_1, D3D_FEATURE_LEVEL_10_0
	};

	for (auto driverTypeIndex : driverTypes) {

		m_driverType = driverTypes[driverTypeIndex];

		result = D3D11CreateDevice(nullptr, m_driverType, nullptr, createDeviceFlags, featureLevels, _countof(featureLevels), D3D11_SDK_VERSION, &m_device, &m_featureLevel, &m_deviceContext);

		if (result == E_INVALIDARG) { result = D3D11CreateDevice(nullptr, m_driverType, nullptr, createDeviceFlags, &featureLevels[1], _countof(featureLevels) - 1, D3D11_SDK_VERSION, &m_device, &m_featureLevel, &m_deviceContext); }

		if (SUCCEEDED(result)) { break; }
	}

	if (FAILED(result)) { Log("[DXAPP]cant initialize device", EMPTY, LOG_SUCCESS); return result; }

	CreateSwapChain(appWindow, clientWidth, clientHeight);
	CreateRenderTargetView();
	CreateDepthStencilView(clientWidth, clientHeight);
	CreateRasterizer();
	SetupViewport(clientWidth, clientHeight);

	float screenAspect = (float)clientWidth / (float)clientHeight;

	// Initialize the world matrices
	m_worldMatrix = DirectX::XMMatrixIdentity();

	// Initialize the view matrix
	DirectX::XMVECTOR Eye = DirectX::XMVectorSet(0.0f, 3.0f, -50.0f, 0.0f);
	DirectX::XMVECTOR At = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	DirectX::XMVECTOR Up = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	m_viewMatrix = DirectX::XMMatrixLookAtLH(Eye, At, Up);

	//CBNeverChanges cbNeverChanges;
	//cbNeverChanges.mView = XMMatrixTranspose(m_viewMatrix);
	//m_deviceContext->UpdateSubresource(CBneverChanges, 0, nullptr, &cbNeverChanges, 0, 0);


	// Create the projection matrix for 3D rendering.
	m_projectionMatrix = DirectX::XMMatrixPerspectiveFovLH(DirectX::XM_PIDIV2, screenAspect, clientNear, clientDepth);
	
	//CBChangeOnResize cbChangesOnResize;
	//cbChangesOnResize.mProjection = XMMatrixTranspose(m_projectionMatrix);
	//m_deviceContext->UpdateSubresource(CBchangeonResize, 0, nullptr, &cbChangesOnResize, 0, 0);

	// Create an orthographic projection matrix for 2D rendering.
//	m_orthoMatrix = DirectX::XMMatrixOrthographicLH((float)clientWidth, (float)clientHeight, clientNear, clientDepth);

	Log("[DXAPP] Direct3D initialized successfully!", EMPTY, LOG_SUCCESS);

	return S_OK;
}

HRESULT Direct3D::CreateSwapChain(HWND appWindow, int clientWidth, int clientHeight)
{
	HRESULT result;

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

	if (FAILED(result)) { Log("[DXAPP] cant get adapter", EMPTY, LOG_SUCCESS); return result; }

	IDXGIFactory2* dxgiFactory2 = nullptr;
	result = dxgiFactory->QueryInterface(__uuidof(IDXGIFactory2), (void**)(&dxgiFactory2));

	if (dxgiFactory2) {
		// DirectX 11.1 or later
		result = m_device->QueryInterface(__uuidof(ID3D11Device1), (void**)(&m_device1));

		if (SUCCEEDED(result)) { (void)m_deviceContext->QueryInterface(__uuidof(ID3D11DeviceContext1), (void**)(&m_deviceContext1)); }

		DXGI_SWAP_CHAIN_DESC1 sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.Width = clientWidth;
		sd.Height = clientHeight;
		sd.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.BufferCount = 1;

		result = dxgiFactory2->CreateSwapChainForHwnd(m_device, appWindow, &sd, nullptr, nullptr, &m_swapChain1);

		if (SUCCEEDED(result)) { result = m_swapChain1->QueryInterface(__uuidof(IDXGISwapChain), (void**)(&m_swapChain)); }

		SafeRelease(dxgiFactory2);
	}
	else {
		// DirectX 11.0 systems
		DXGI_SWAP_CHAIN_DESC sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.BufferCount = 1;
		sd.BufferDesc.Width = clientWidth;
		sd.BufferDesc.Height = clientHeight;
		sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		sd.BufferDesc.RefreshRate.Numerator = 60;
		sd.BufferDesc.RefreshRate.Denominator = 1;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.OutputWindow = appWindow;
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
		sd.Windowed = TRUE;

		result = dxgiFactory->CreateSwapChain(m_device, &sd, &m_swapChain);
	}

	dxgiFactory->MakeWindowAssociation(appWindow, DXGI_MWA_NO_ALT_ENTER);

	SafeRelease(dxgiFactory);

	if (FAILED(result)) { Log("[DXAPP]cant create swap chain", EMPTY, LOG_SUCCESS); return result; }

	return S_OK;
}


HRESULT Direct3D::CreateRenderTargetView()
{
	HRESULT result;

	ID3D11Texture2D* backBuffer = nullptr;

	result = m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBuffer);

	if (FAILED(result)) { Log("[DXAPP]cant get the buffer", EMPTY, LOG_SUCCESS); return result; }

	result = m_device->CreateRenderTargetView(backBuffer, nullptr, &m_backBuffer);
	SafeRelease(backBuffer);

	if (FAILED(result)) { Log("[DXAPP] cant set up render target view", EMPTY, LOG_SUCCESS); return result; }

	return S_OK;
}


HRESULT Direct3D::CreateDepthStencilView(int clientWidth, int clientHeight)
{
	HRESULT result;

	D3D11_TEXTURE2D_DESC depthBufferDesc;
	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));
	depthBufferDesc.Width = clientWidth;
	depthBufferDesc.Height = clientHeight;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;
	result = m_device->CreateTexture2D(&depthBufferDesc, nullptr, &m_depthStencilBuffer);

	if (FAILED(result)) { Log("[DXAPP] cant create depth stencil buffer", EMPTY, LOG_SUCCESS); return result; }

	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	// Set up the description of the stencil state.
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	// Stencil operations if pixel is front-facing.
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Stencil operations if pixel is back-facing.
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		// Create the depth stencil state.
	result = m_device->CreateDepthStencilState(&depthStencilDesc, &m_depthStencilState);
	if (FAILED(result)) { Log("[DXAPP] cant creeate depth stencil state", EMPTY, LOG_SUCCESS); return result; }

	// Set the depth stencil state.
	m_deviceContext->OMSetDepthStencilState(m_depthStencilState, 1);


	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));
	depthStencilViewDesc.Format = depthStencilViewDesc.Format;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;
	result = m_device->CreateDepthStencilView(m_depthStencilBuffer, &depthStencilViewDesc, &m_depthStencilView);

	if (FAILED(result)) { Log("cant create depth stencil view", EMPTY, LOG_SUCCESS); return result; }

	m_deviceContext->OMSetRenderTargets(1, &m_backBuffer, m_depthStencilView);

	return S_OK;
}


HRESULT Direct3D::CreateRasterizer()
{
	HRESULT result;
	// Setup the raster description which will determine how and what polygons will be drawn.
	D3D11_RASTERIZER_DESC rasterDesc;
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_BACK;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	// Create the rasterizer state from the description we just filled out.
	result = m_device->CreateRasterizerState(&rasterDesc, &m_rasterState);
	if (FAILED(result)){ Log("[DXAPP] cant create rasterizer", EMPTY, LOG_SUCCESS); return result;}

	// Now set the rasterizer state.
	m_deviceContext->RSSetState(m_rasterState);

	return S_OK;
}



void Direct3D::SetupViewport(int clientWidth, int clientHeight)
{
	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));
	viewport.Width = (FLOAT)clientWidth;
	viewport.Height = (FLOAT)clientHeight;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;

	/*RSSetViewports() is a function that activates viewport structs. The first parameter is the number of viewports being used, and the second parameter is the address of a list of pointers to the viewport structs.
	Using multiple viewports is handy in certain advanced situations, but we won't get into them here. For now, 1 and '&viewport' will suit the parameters fine.*/

	m_deviceContext->RSSetViewports(1, &viewport);
}




void Direct3D::Shutdown()
{
	std::cout << "shutting down direct 3d" << std::endl;
	// Before shutting down set to windowed mode or when you release the swap chain it will throw an exception.
	if (m_swapChain){ m_swapChain->SetFullscreenState(false, NULL); }

	if (m_rasterState){
		m_rasterState->Release();
		m_rasterState = 0;
	}

	if (m_depthStencilView){
		m_depthStencilView->Release();
		m_depthStencilView = 0;
	}

	if (m_depthStencilState){
		m_depthStencilState->Release();
		m_depthStencilState = 0;
	}

	if (m_depthStencilBuffer){
		m_depthStencilBuffer->Release();
		m_depthStencilBuffer = 0;
	}

	if (m_backBuffer){
		m_backBuffer->Release();
		m_backBuffer = 0;
	}

	if (m_swapChain1)
	{
		m_swapChain1->Release();
		m_swapChain1 = 0;
	}

	if (m_swapChain)
	{
		m_swapChain->Release();
		m_swapChain = 0;
	}

	if (m_deviceContext1)
	{
		m_deviceContext1->Release();
		m_deviceContext1 = 0;
	}

	if (m_deviceContext)
	{
		m_deviceContext->Release();
		m_deviceContext = 0;
	}

	if (m_device1)
	{
		m_device1->Release();
		m_device1 = 0;
	}


	if (m_device)
	{
		m_device->Release();
		m_device = 0;
	}

}

void Direct3D::BeginScene(float red, float green, float blue, float alpha)
{
	float color[4];

	// Setup the color to clear the buffer to.
	color[0] = red;
	color[1] = green;
	color[2] = blue;
	color[3] = alpha;

	// Clear the back buffer.
	m_deviceContext->ClearRenderTargetView(m_backBuffer, color);

	// Clear the depth buffer.
	m_deviceContext->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void Direct3D::EndScene()
{
	// Present the back buffer to the screen since rendering is complete.
	if (m_vSync)
	{
		// Lock to screen refresh rate.
		m_swapChain->Present(1, 0);
	}
	else
	{
		// Present as fast as possible.
		m_swapChain->Present(0, 0);
	}

}
