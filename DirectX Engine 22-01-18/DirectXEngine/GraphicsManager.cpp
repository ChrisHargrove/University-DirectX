#include "GraphicsManager.h"
#include "ScreenManager.h"
#include "Log.h"

/*******************************************************************************************************************
	Constructor with initializer list to set all default values of variables
*******************************************************************************************************************/
GraphicsManager::GraphicsManager()	:	m_videoCardMemory(0),
<<<<<<< HEAD
=======
										m_multiSampleCount(8),
										m_multiSampleQuality(0),
>>>>>>> master
										m_swapChain(nullptr),
										m_device(nullptr),
										m_deviceContext(nullptr),
										m_renderTargetView(nullptr),
										m_depthStencilBuffer(nullptr),
										m_depthStencilState(nullptr),
										m_depthStencilView(nullptr),
										m_rasterState(nullptr),
										m_depthDisabledStencilState(nullptr),
										m_alphaEnableBlendingState(nullptr),
										m_alphaDisableBlendingState(nullptr)
{
	DX_LOG("[GRAPHICS] Graphics constructor initialized", DX_LOG_EMPTY, LOG_MESSAGE);
}


/*******************************************************************************************************************
	Shut down all devices, destroy resources and clean up all memory
*******************************************************************************************************************/
void GraphicsManager::Shutdown()
{
	//---------------------------------------------------------------- Before shutting down set to windowed mode or when you release the swap chain it will throw an exception
	if (m_swapChain)					{ m_swapChain->SetFullscreenState(false, nullptr); }

	if (m_alphaDisableBlendingState)	{ m_alphaDisableBlendingState->Release(); m_alphaDisableBlendingState = nullptr; }
	if (m_alphaEnableBlendingState)		{ m_alphaEnableBlendingState->Release(); m_alphaEnableBlendingState = nullptr; }
	if (m_depthDisabledStencilState)	{ m_depthDisabledStencilState->Release(); m_depthDisabledStencilState = nullptr; }
	if (m_rasterState)					{ m_rasterState->Release(); m_rasterState = nullptr; }
	if (m_depthStencilView)				{ m_depthStencilView->Release(); m_depthStencilView = nullptr; }
	if (m_depthStencilState)			{ m_depthStencilState->Release(); m_depthStencilState = nullptr; }
	if (m_depthStencilBuffer)			{ m_depthStencilBuffer->Release(); m_depthStencilBuffer = nullptr; }
	if (m_renderTargetView)				{ m_renderTargetView->Release(); m_renderTargetView = nullptr; }
	if (m_deviceContext)				{ m_deviceContext->Release(); m_deviceContext = nullptr; }
	if (m_device)						{ m_device->Release(); m_device = nullptr; }
	if (m_swapChain)					{ m_swapChain->Release(); m_swapChain = nullptr; }

	DX_LOG("[GRAPHICS] Graphics shutdown successfully", DX_LOG_EMPTY, LOG_SUCCESS);
}


/*******************************************************************************************************************
	Function that initializes the graphics default setup settings
*******************************************************************************************************************/
bool GraphicsManager::Initialize()
{
	if (!InitializeInterface())				{ return false; }
	if (!InitializeRenderTargetView())		{ return false; }
	if (!InitializeDepthStencilFilters())	{ return false; }
	if (!InitializeRasterizerState())		{ return false; }

	InitializeViewport();

	if (!InitializeAlphaBlendingState())	{ return false; }
	
	DX_LOG("[GRAPHICS] Video Card Information: ", GetVideoCardInfo(), LOG_MESSAGE);
	DX_LOG("[GRAPHICS] Video Card Memory (MB): ", GetVideoCardMemory(), LOG_MESSAGE);

	DX_LOG("[GRAPHICS] DirectX initialized successfully", DX_LOG_EMPTY, LOG_SUCCESS);

	return true;
}


/*******************************************************************************************************************
	Function that obtains the GPU information and stores the data in to a character array
*******************************************************************************************************************/
bool GraphicsManager::StoreVideoCardInfo(IDXGIAdapter* adapter)
{
	HRESULT result = S_OK;

	//-------------------------------------------- Get the adapter (video card) description
	DXGI_ADAPTER_DESC adapterDesc = { 0 };
	result = adapter->GetDesc(&adapterDesc);
	
	if (FAILED(result)) { DX_LOG("[GRAPHICS] Couldn't retrieve the GPU information", DX_LOG_EMPTY, LOG_ERROR); return false; }

	//-------------------------------------------- Store the dedicated video card memory in megabytes
	m_videoCardMemory = (int)(adapterDesc.DedicatedVideoMemory / 1024 / 1024);
	
	//-------------------------------------------- Convert the name of the video card to a character array and store it
	unsigned int length;
	
	result = wcstombs_s(&length, &m_videoCardInfo[0], m_videoCardInfo.max_size(), adapterDesc.Description, m_videoCardInfo.max_size());
	
	if (FAILED(result)) { DX_LOG("[GRAPHICS] Problem converting GPU information to string", DX_LOG_EMPTY, LOG_ERROR); return false; }

	return true;
}


/*******************************************************************************************************************
	Function that initializes the DirectX device and swap chain
*******************************************************************************************************************/
bool GraphicsManager::InitializeSwapChain(unsigned int numerator, unsigned int denominator)
{
	HRESULT result = S_OK;

	//-------------------------------------------- Initialize the swap chain description
	DXGI_SWAP_CHAIN_DESC swapChainDescription = { 0 };

	//-------------------------------------------- Set to a single back buffer
<<<<<<< HEAD
	swapChainDescription.BufferCount = 1;

	//-------------------------------------------- Set the width and height of the back buffer
	swapChainDescription.BufferDesc.Width = Screen::Instance()->GetWidth();
	swapChainDescription.BufferDesc.Height = Screen::Instance()->GetHeight();
	
	//-------------------------------------------- Set regular 32-bit surface for the back buffer
	swapChainDescription.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
=======
	swapChainDescription.BufferCount							= 1;

	//-------------------------------------------- Set the width and height of the back buffer
	swapChainDescription.BufferDesc.Width						= Screen::Instance()->GetWidth();
	swapChainDescription.BufferDesc.Height						= Screen::Instance()->GetHeight();
	
	//-------------------------------------------- Set regular 32-bit surface for the back buffer
	swapChainDescription.BufferDesc.Format						= DXGI_FORMAT_R8G8B8A8_UNORM;
>>>>>>> master

	//-------------------------------------------- Set the refresh rate of the back buffer
	if (Screen::Instance()->IsVSyncEnabled())
	{
<<<<<<< HEAD
		swapChainDescription.BufferDesc.RefreshRate.Numerator = numerator;
=======
		swapChainDescription.BufferDesc.RefreshRate.Numerator	= numerator;
>>>>>>> master
		swapChainDescription.BufferDesc.RefreshRate.Denominator = denominator;
		
		DX_LOG("[GRAPHICS] Vsync mode enabled", DX_LOG_EMPTY, LOG_MESSAGE);
	}
	else
	{
<<<<<<< HEAD
		swapChainDescription.BufferDesc.RefreshRate.Numerator = 0;
=======
		swapChainDescription.BufferDesc.RefreshRate.Numerator	= 0;
>>>>>>> master
		swapChainDescription.BufferDesc.RefreshRate.Denominator = 1;

		DX_LOG("[GRAPHICS] Vsync mode disabled", DX_LOG_EMPTY, LOG_MESSAGE);
	}

	//-------------------------------------------- Set the usage of the back buffer
<<<<<<< HEAD
	swapChainDescription.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

	//-------------------------------------------- Set the handle for the window to render to
	swapChainDescription.OutputWindow = Screen::Instance()->GetWindow();

	//-------------------------------------------- Multisampling descriptions
	swapChainDescription.SampleDesc.Count = 1;
	swapChainDescription.SampleDesc.Quality = 0;
=======
	swapChainDescription.BufferUsage							= DXGI_USAGE_RENDER_TARGET_OUTPUT;

	//-------------------------------------------- Set the handle for the window to render to
	swapChainDescription.OutputWindow							= Screen::Instance()->GetWindow();

	//-------------------------------------------- Multisampling descriptions
	swapChainDescription.SampleDesc.Count						= m_multiSampleCount;
	swapChainDescription.SampleDesc.Quality						= m_multiSampleQuality;
>>>>>>> master

	//-------------------------------------------- Set to full screen or windowed mode
	if (Screen::Instance()->IsFullScreen()) { swapChainDescription.Windowed = false; }
	else									{ swapChainDescription.Windowed = true; }

	//-------------------------------------------- Set the scan line ordering and scaling to unspecified
<<<<<<< HEAD
	swapChainDescription.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDescription.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	//-------------------------------------------- Discard the back buffer contents after presenting
	swapChainDescription.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	//-------------------------------------------- Don't set the advanced flags
	swapChainDescription.Flags = 0;
=======
	swapChainDescription.BufferDesc.ScanlineOrdering			= DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDescription.BufferDesc.Scaling						= DXGI_MODE_SCALING_UNSPECIFIED;

	//-------------------------------------------- Discard the back buffer contents after presenting
	swapChainDescription.SwapEffect								= DXGI_SWAP_EFFECT_DISCARD;

	//-------------------------------------------- Don't set the advanced flags
	swapChainDescription.Flags									= 0;

>>>>>>> master

	D3D_DRIVER_TYPE driverType;

	D3D_DRIVER_TYPE driverTypes[] = {
		D3D_DRIVER_TYPE_HARDWARE, D3D_DRIVER_TYPE_WARP, D3D_DRIVER_TYPE_REFERENCE
	};

	D3D_FEATURE_LEVEL featureLevels[] = {
		D3D_FEATURE_LEVEL_11_1, D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_1, D3D_FEATURE_LEVEL_10_0
	};

	//---------------------------------------------------------------- Loop through the available drivers 
	for (UINT driverTypeIndex = 0; driverTypeIndex < _countof(driverTypes); driverTypeIndex++) {

		driverType = driverTypes[driverTypeIndex];

		//---------------------------------------------------------------- Create a GPU interface that supports up to Direct X 11.1 
		result = D3D11CreateDeviceAndSwapChain(nullptr, driverType, nullptr, 0, featureLevels, _countof(featureLevels), D3D11_SDK_VERSION, &swapChainDescription, &m_swapChain, &m_device, nullptr, &m_deviceContext);

		//---------------------------------------------------------------- Or if unsupported, create a GPU interface that supports up to Direct X 11 
		if (result == E_INVALIDARG) { result = D3D11CreateDeviceAndSwapChain(nullptr, driverType, nullptr, 0, &featureLevels[1], _countof(featureLevels) - 1, D3D11_SDK_VERSION, &swapChainDescription, &m_swapChain, &m_device, nullptr, &m_deviceContext); }

		if (SUCCEEDED(result)) { DX_LOG("[GRAPHICS] DirectX device and swap chain initialized successfully", DX_LOG_EMPTY, LOG_SUCCESS); break; }
	}

	if (FAILED(result)) { DX_LOG("[GRAPHICS] Couldn't initialize DirectX device and swap chain", DX_LOG_EMPTY, LOG_ERROR); return false; }

	return true;
}


/*******************************************************************************************************************
	Function that initializes the render target view
*******************************************************************************************************************/
bool GraphicsManager::InitializeRenderTargetView()
{
	HRESULT result = S_OK;

	//---------------------------------------------------------------- Get the pointer to the back buffer
	ID3D11Texture2D* backBuffer = nullptr;
	result = m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBuffer);
	if (FAILED(result)) { DX_LOG("[GRAPHICS] Couldn't obtain back buffer pointer", DX_LOG_EMPTY, LOG_ERROR);  return false; }

	//---------------------------------------------------------------- Create the render target view with the back buffer pointer
	result = m_device->CreateRenderTargetView(backBuffer, nullptr, &m_renderTargetView);
	if (FAILED(result)) { DX_LOG("[GRAPHICS] Couldn't create render target view", DX_LOG_EMPTY, LOG_ERROR); return false; }

	//---------------------------------------------------------------- Release pointer to back buffer as we no longer need it
	if (backBuffer) { backBuffer->Release(); backBuffer = nullptr; }
	
	DX_LOG("[GRAPHICS] Render target view initialized successfully", DX_LOG_EMPTY, LOG_SUCCESS);

	return true;
}


/*******************************************************************************************************************
	Function that initializes the depth stencil filters
*******************************************************************************************************************/
bool GraphicsManager::InitializeDepthStencilFilters()
{
	HRESULT result = S_OK;

	//---------------------------------------------------------------- Initialize the description of the depth buffer
<<<<<<< HEAD
	D3D11_TEXTURE2D_DESC depthBufferDesccription = { 0 };

	//---------------------------------------------------------------- Set up the description of the depth buffer
	depthBufferDesccription.Width = Screen::Instance()->GetWidth();
	depthBufferDesccription.Height = Screen::Instance()->GetHeight();
	depthBufferDesccription.MipLevels = 1;
	depthBufferDesccription.ArraySize = 1;
	depthBufferDesccription.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesccription.SampleDesc.Count = 1;
	depthBufferDesccription.SampleDesc.Quality = 0;
	depthBufferDesccription.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesccription.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesccription.CPUAccessFlags = 0;
	depthBufferDesccription.MiscFlags = 0;

	//---------------------------------------------------------------- Create the texture for the depth buffer using the filled out description
	result = m_device->CreateTexture2D(&depthBufferDesccription, nullptr, &m_depthStencilBuffer);
=======
	D3D11_TEXTURE2D_DESC depthBufferDescription = { 0 };

	//---------------------------------------------------------------- Set up the description of the depth buffer
	depthBufferDescription.Width				= Screen::Instance()->GetWidth();
	depthBufferDescription.Height				= Screen::Instance()->GetHeight();
	depthBufferDescription.MipLevels			= 1;
	depthBufferDescription.ArraySize			= 1;
	depthBufferDescription.Format				= DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDescription.SampleDesc.Count		= m_multiSampleCount;
	depthBufferDescription.SampleDesc.Quality	= m_multiSampleQuality;
	depthBufferDescription.Usage				= D3D11_USAGE_DEFAULT;
	depthBufferDescription.BindFlags			= D3D11_BIND_DEPTH_STENCIL;
	depthBufferDescription.CPUAccessFlags		= 0;
	depthBufferDescription.MiscFlags			= 0;

	//---------------------------------------------------------------- Create the texture for the depth buffer using the filled out description
	result = m_device->CreateTexture2D(&depthBufferDescription, nullptr, &m_depthStencilBuffer);
>>>>>>> master
	if (FAILED(result)) { DX_LOG("[GRAPHICS] Couldn't create depth buffer texture", DX_LOG_EMPTY, LOG_ERROR); return false; }

	//---------------------------------------------------------------- Initialize the description of the depth stencil state
	D3D11_DEPTH_STENCIL_DESC depthStencilDescription = { 0 };

	//---------------------------------------------------------------- Set up the description of the depth stencil state
<<<<<<< HEAD
	depthStencilDescription.DepthEnable = true;
	depthStencilDescription.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDescription.DepthFunc = D3D11_COMPARISON_LESS;
	depthStencilDescription.StencilEnable = true;
	depthStencilDescription.StencilReadMask = 0xFF;
	depthStencilDescription.StencilWriteMask = 0xFF;

	//---------------------------------------------------------------- Stencil operations if pixel is front-facing
	depthStencilDescription.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDescription.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDescription.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDescription.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	//---------------------------------------------------------------- Stencil operations if pixel is back-facing
	depthStencilDescription.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDescription.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDescription.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDescription.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
=======
	depthStencilDescription.DepthEnable			= true;
	depthStencilDescription.DepthWriteMask		= D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDescription.DepthFunc			= D3D11_COMPARISON_LESS;
	depthStencilDescription.StencilEnable		= true;
	depthStencilDescription.StencilReadMask		= 0xFF;
	depthStencilDescription.StencilWriteMask	= 0xFF;

	//---------------------------------------------------------------- Stencil operations if pixel is front-facing
	depthStencilDescription.FrontFace.StencilFailOp			= D3D11_STENCIL_OP_KEEP;
	depthStencilDescription.FrontFace.StencilDepthFailOp	= D3D11_STENCIL_OP_INCR;
	depthStencilDescription.FrontFace.StencilPassOp			= D3D11_STENCIL_OP_KEEP;
	depthStencilDescription.FrontFace.StencilFunc			= D3D11_COMPARISON_ALWAYS;

	//---------------------------------------------------------------- Stencil operations if pixel is back-facing
	depthStencilDescription.BackFace.StencilFailOp			= D3D11_STENCIL_OP_KEEP;
	depthStencilDescription.BackFace.StencilDepthFailOp		= D3D11_STENCIL_OP_DECR;
	depthStencilDescription.BackFace.StencilPassOp			= D3D11_STENCIL_OP_KEEP;
	depthStencilDescription.BackFace.StencilFunc			= D3D11_COMPARISON_ALWAYS;
>>>>>>> master

	//---------------------------------------------------------------- Create the depth stencil state
	result = m_device->CreateDepthStencilState(&depthStencilDescription, &m_depthStencilState);
	if (FAILED(result)) { DX_LOG("[GRAPHICS] Couldn't create depth stencil state", DX_LOG_EMPTY, LOG_ERROR); return false; }

	//---------------------------------------------------------------- Set the depth stencil state
	m_deviceContext->OMSetDepthStencilState(m_depthStencilState, 1);

	//---------------------------------------------------------------- Initialize the depth stencil view description
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDescription = {};

	//---------------------------------------------------------------- Set up the depth stencil view description
<<<<<<< HEAD
	depthStencilViewDescription.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDescription.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDescription.Texture2D.MipSlice = 0;
=======
	depthStencilViewDescription.Format				= DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDescription.ViewDimension		= D3D11_DSV_DIMENSION_TEXTURE2DMS;
	depthStencilViewDescription.Texture2D.MipSlice	= 0;
>>>>>>> master

	//---------------------------------------------------------------- Create the depth stencil view
	result = m_device->CreateDepthStencilView(m_depthStencilBuffer, &depthStencilViewDescription, &m_depthStencilView);
	if (FAILED(result)) { DX_LOG("[GRAPHICS] Couldn't create depth stencil view", DX_LOG_EMPTY, LOG_ERROR); return false; }

	//---------------------------------------------------------------- Bind the render target view and depth stencil buffer to the output render pipeline
	m_deviceContext->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilView);
	
	//---------------------------------------------------------------- Clear the second depth stencil state before setting the parameters
	D3D11_DEPTH_STENCIL_DESC depthDisabledStencilDescription = { 0 };

	//----------------------------------------------------------------  Now create a second depth stencil state which turns off the Z buffer for 2D rendering.  The only difference is 
	//----------------------------------------------------------------  that DepthEnable is set to false, all other parameters are the same as the other depth stencil state.
<<<<<<< HEAD
	depthDisabledStencilDescription.DepthEnable = false;
	depthDisabledStencilDescription.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthDisabledStencilDescription.DepthFunc = D3D11_COMPARISON_LESS;
	depthDisabledStencilDescription.StencilEnable = true;
	depthDisabledStencilDescription.StencilReadMask = 0xFF;
	depthDisabledStencilDescription.StencilWriteMask = 0xFF;
	depthDisabledStencilDescription.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDescription.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthDisabledStencilDescription.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDescription.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	depthDisabledStencilDescription.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDescription.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthDisabledStencilDescription.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDescription.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
=======
	depthDisabledStencilDescription.DepthEnable						= false;
	depthDisabledStencilDescription.DepthWriteMask					= D3D11_DEPTH_WRITE_MASK_ALL;
	depthDisabledStencilDescription.DepthFunc						= D3D11_COMPARISON_LESS;
	depthDisabledStencilDescription.StencilEnable					= true;
	depthDisabledStencilDescription.StencilReadMask					= 0xFF;
	depthDisabledStencilDescription.StencilWriteMask				= 0xFF;
	depthDisabledStencilDescription.FrontFace.StencilFailOp			= D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDescription.FrontFace.StencilDepthFailOp	= D3D11_STENCIL_OP_INCR;
	depthDisabledStencilDescription.FrontFace.StencilPassOp			= D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDescription.FrontFace.StencilFunc			= D3D11_COMPARISON_ALWAYS;
	depthDisabledStencilDescription.BackFace.StencilFailOp			= D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDescription.BackFace.StencilDepthFailOp		= D3D11_STENCIL_OP_DECR;
	depthDisabledStencilDescription.BackFace.StencilPassOp			= D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDescription.BackFace.StencilFunc			= D3D11_COMPARISON_ALWAYS;
>>>>>>> master

	//---------------------------------------------------------------- Create the disabled depth stencil state
	result = m_device->CreateDepthStencilState(&depthDisabledStencilDescription, &m_depthDisabledStencilState);
	if (FAILED(result)) { DX_LOG("[GRAPHICS] Couldn't create disabled depth stencil state", DX_LOG_EMPTY, LOG_ERROR); return false; }

	DX_LOG("[GRAPHICS] Depth stencil filters set successfully", DX_LOG_EMPTY, LOG_SUCCESS);

	return true;
}


/*******************************************************************************************************************
	Function that initializes the rasterizer state
*******************************************************************************************************************/
bool GraphicsManager::InitializeRasterizerState()
{
	HRESULT result = S_OK;

	//---------------------------------------------------------------- Setup the raster description which will determine how and what polygons will be drawn
	D3D11_RASTERIZER_DESC rasterizerDescription = {};
<<<<<<< HEAD
	rasterizerDescription.AntialiasedLineEnable = false;
	rasterizerDescription.CullMode = D3D11_CULL_BACK;
	rasterizerDescription.DepthBias = 0;
	rasterizerDescription.DepthBiasClamp = 0.0f;
	rasterizerDescription.DepthClipEnable = true;
	rasterizerDescription.FillMode = D3D11_FILL_SOLID;
	rasterizerDescription.FrontCounterClockwise = false;
	rasterizerDescription.MultisampleEnable = false;
	rasterizerDescription.ScissorEnable = false;
	rasterizerDescription.SlopeScaledDepthBias = 0.0f;
=======

	rasterizerDescription.AntialiasedLineEnable		= false;
	rasterizerDescription.CullMode					= D3D11_CULL_BACK;
	//RasterizerDesc.FillMode						= D3D11_FILL_WIREFRAME;
	//RasterizerDesc.CullMode						= D3D11_CULL_NONE;
	rasterizerDescription.DepthBias					= 0;
	rasterizerDescription.DepthBiasClamp			= 0.0f;
	rasterizerDescription.DepthClipEnable			= true;
	rasterizerDescription.FillMode					= D3D11_FILL_SOLID;
	rasterizerDescription.FrontCounterClockwise		= false;
	rasterizerDescription.MultisampleEnable			= true;
	rasterizerDescription.ScissorEnable				= false;
	rasterizerDescription.SlopeScaledDepthBias		= 0.0f;
>>>>>>> master

	//---------------------------------------------------------------- Create the rasterizer state from the description we just filled out
	result = m_device->CreateRasterizerState(&rasterizerDescription, &m_rasterState);
	if (FAILED(result)) { DX_LOG("[GRAPHICS] Couldn't initialize the rasterizer state", DX_LOG_EMPTY, LOG_ERROR); return false; }

	//---------------------------------------------------------------- Now set the rasterizer state
	m_deviceContext->RSSetState(m_rasterState);

	DX_LOG("[GRAPHICS] Rasterizer state initialized successfully", DX_LOG_EMPTY, LOG_SUCCESS);

	return true;
}


/*******************************************************************************************************************
	Function that initializes the viewport(s) in use
*******************************************************************************************************************/
void GraphicsManager::InitializeViewport()
{
	D3D11_VIEWPORT viewport = { 0 };
<<<<<<< HEAD
	viewport.Width = (float)Screen::Instance()->GetWidth();
	viewport.Height = (float)Screen::Instance()->GetHeight();
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;
=======

	viewport.Width		= (float)Screen::Instance()->GetWidth();
	viewport.Height		= (float)Screen::Instance()->GetHeight();
	viewport.MinDepth	= 0.0f;
	viewport.MaxDepth	= 1.0f;
	viewport.TopLeftX	= 0.0f;
	viewport.TopLeftY	= 0.0f;
>>>>>>> master

	//---------------------------------------------------------------- Create the viewport
	m_deviceContext->RSSetViewports(1, &viewport);

	DX_LOG("[GRAPHICS] Viewport initialized successfully", DX_LOG_EMPTY, LOG_SUCCESS);
}


/*******************************************************************************************************************
	Function that initializes the alpha blending state
*******************************************************************************************************************/
bool GraphicsManager::InitializeAlphaBlendingState()
{
	HRESULT result = S_OK;
	
	//---------------------------------------------------------------- Clear the blending state description
	D3D11_BLEND_DESC blendStateDescription = { 0 };

	//---------------------------------------------------------------- Create an alpha enabled blend state description
<<<<<<< HEAD
	blendStateDescription.RenderTarget[0].BlendEnable = TRUE;
	blendStateDescription.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	blendStateDescription.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendStateDescription.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendStateDescription.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendStateDescription.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendStateDescription.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendStateDescription.RenderTarget[0].RenderTargetWriteMask = 0x0f;
=======
	blendStateDescription.RenderTarget[0].BlendEnable				= TRUE;
	blendStateDescription.RenderTarget[0].SrcBlend					= D3D11_BLEND_ONE;
	blendStateDescription.RenderTarget[0].DestBlend					= D3D11_BLEND_INV_SRC_ALPHA;
	blendStateDescription.RenderTarget[0].BlendOp					= D3D11_BLEND_OP_ADD;
	blendStateDescription.RenderTarget[0].SrcBlendAlpha				= D3D11_BLEND_ONE;
	blendStateDescription.RenderTarget[0].DestBlendAlpha			= D3D11_BLEND_ZERO;
	blendStateDescription.RenderTarget[0].BlendOpAlpha				= D3D11_BLEND_OP_ADD;
	blendStateDescription.RenderTarget[0].RenderTargetWriteMask		= 0x0f;
>>>>>>> master

	//---------------------------------------------------------------- Create the blend state using the description above
	result = m_device->CreateBlendState(&blendStateDescription, &m_alphaEnableBlendingState);
	if (FAILED(result)) { DX_LOG("[GRAPHICS] Couldn't initialize enabled alpha blend state", DX_LOG_EMPTY, LOG_ERROR); return false; }

	//---------------------------------------------------------------- Modify the description to create an alpha disabled blend state description
	blendStateDescription.RenderTarget[0].BlendEnable = FALSE;
	
	//---------------------------------------------------------------- Create the second blend state using the description
	result = m_device->CreateBlendState(&blendStateDescription, &m_alphaDisableBlendingState);
	if (FAILED(result)) { DX_LOG("[GRAPHICS] Couldn't initialize disabled alpha blend state", DX_LOG_EMPTY, LOG_ERROR); return false; }

	DX_LOG("[GRAPHICS] Alpha blend state initialized successfully", DX_LOG_EMPTY, LOG_SUCCESS);

	return true;
}


/*******************************************************************************************************************
	Function that initializes the interface (video card) and monitor in use
*******************************************************************************************************************/
bool GraphicsManager::InitializeInterface()
{
	HRESULT result = S_OK;

	//---------------------------------------------------------------- Create a DirectX graphics interface factory
	IDXGIFactory* factory = nullptr;
	result = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
	if (FAILED(result)) { DX_LOG("[GRAPHICS] Couldn't create graphics interface factory", DX_LOG_EMPTY, LOG_ERROR); return false; }

	//---------------------------------------------------------------- Use the factory to create an adapter for the primary graphics interface (video card)
	IDXGIAdapter* adapter = nullptr;
	result = factory->EnumAdapters(0, &adapter);
	if (FAILED(result)) { DX_LOG("[GRAPHICS] Couldn't create adapter", DX_LOG_EMPTY, LOG_ERROR); return false; }

	//---------------------------------------------------------------- Enumerate the primary adapter output (monitor)
	IDXGIOutput* adapterOutput = nullptr;
	result = adapter->EnumOutputs(0, &adapterOutput);
	if (FAILED(result)) { DX_LOG("[GRAPHICS] Problem finding primary monitor", DX_LOG_EMPTY, LOG_ERROR); return false; }

	//---------------------------------------------------------------- Get the number of modes that fit the DXGI_FORMAT_R8G8B8A8_UNORM display format for the adapter output (monitor)
	unsigned int numModes = 0;
	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, nullptr);
	if (FAILED(result)) { DX_LOG("[GRAPHICS] Problem obtaining display format", DX_LOG_EMPTY, LOG_ERROR); return false; }

	//---------------------------------------------------------------- Create a list to hold all the possible display modes for this monitor/video card combination
<<<<<<< HEAD
	DXGI_MODE_DESC* displayModeList = nullptr;
	displayModeList = new DXGI_MODE_DESC[numModes];
=======
	DXGI_MODE_DESC* displayModeList = new DXGI_MODE_DESC[numModes];
>>>>>>> master
	if (!displayModeList) { DX_LOG("[GRAPHICS] Couldn't create display mode list object", DX_LOG_EMPTY, LOG_ERROR); return false; }

	//---------------------------------------------------------------- Now fill the display mode list structures
	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList);
	if (FAILED(result)) { DX_LOG("[GRAPHICS] Couldn't fill display mode list structures", DX_LOG_EMPTY, LOG_ERROR); return false; }

	//---------------------------------------------------------------- Now go through all the display modes and find the one that matches the screen width and height
	//---------------------------------------------------------------- When a match is found store the numerator and denominator of the refresh rate for that monitor
	unsigned int numerator = 0, denominator = 0;

	for (unsigned int i = 0; i < numModes; i++)
	{
		if (displayModeList[i].Width == (unsigned int)Screen::Instance()->GetWidth())
		{
			if (displayModeList[i].Height == (unsigned int)Screen::Instance()->GetHeight())
			{
<<<<<<< HEAD
				numerator = displayModeList[i].RefreshRate.Numerator;
=======
				numerator	= displayModeList[i].RefreshRate.Numerator;
>>>>>>> master
				denominator = displayModeList[i].RefreshRate.Denominator;
			}
		}
	}

	//---------------------------------------------------------------- Store GPU info into a character array
	if (!StoreVideoCardInfo(adapter)) { return false; }
	
	//---------------------------------------------------------------- Release the display mode list
	if (displayModeList) { delete[] displayModeList; displayModeList = nullptr; }

	//---------------------------------------------------------------- Release the adapter output
	adapterOutput->Release();
	adapterOutput = nullptr;

	//---------------------------------------------------------------- Release the adapter
	adapter->Release();
	adapter = nullptr;

	//---------------------------------------------------------------- Release the factory
	factory->Release();
	factory = nullptr;
	
	DX_LOG("[GRAPHICS] DirectX interface initialized successfully", DX_LOG_EMPTY, LOG_SUCCESS);

	//---------------------------------------------------------------- Initialize the DirectX device and swap chain
<<<<<<< HEAD
	InitializeSwapChain(numerator, denominator);
=======
	if (!InitializeSwapChain(numerator, denominator)) { return false; }
>>>>>>> master

	return true;
}


/*******************************************************************************************************************
	Function that clears the back buffer and depth buffer
*******************************************************************************************************************/
void GraphicsManager::BeginScene(float red, float green, float blue, float alpha)
{
	float color[4];

	//---------------------------------------------------------------- Setup the color to clear the buffer to
	color[0] = red;
	color[1] = green;
	color[2] = blue;
	color[3] = alpha;

	//---------------------------------------------------------------- Clear the back buffer
	m_deviceContext->ClearRenderTargetView(m_renderTargetView, color);

	//---------------------------------------------------------------- Clear the depth buffer
	m_deviceContext->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}


/*******************************************************************************************************************
	Function that presents the back buffer to the screen
*******************************************************************************************************************/
void GraphicsManager::EndScene()
{
	//---------------------------------------------------------------- If VSync enabled, present the back buffer to the screen at 60fps
	if (Screen::Instance()->IsVSyncEnabled())	{ m_swapChain->Present(1, 0); }

	//---------------------------------------------------------------- Otherwise, present the back buffer to the screen as fast as possible
	else										{ m_swapChain->Present(0, 0); }
}


/*******************************************************************************************************************
	Function that toggles the depth buffer on/off. Set true for 3D rendering and false when rendering in 2D
*******************************************************************************************************************/
void GraphicsManager::EnableDepthBuffer(bool enable3D)
{
	//---------------------------------------------------------------- If enable3D is true, then the depth buffer will be active
	(enable3D)	? m_deviceContext->OMSetDepthStencilState(m_depthStencilState, 1)
				: m_deviceContext->OMSetDepthStencilState(m_depthDisabledStencilState, 1);
}


/*******************************************************************************************************************
	Function that toggles the alpha blend on/off. Set true for 2D rendering and false when rendering in 3D
*******************************************************************************************************************/
void GraphicsManager::EnableAlphaBlending(bool render2D)
{
	float blendFactor[4];

	blendFactor[0] = 0.0f;
	blendFactor[1] = 0.0f;
	blendFactor[2] = 0.0f;
	blendFactor[3] = 0.0f;

	//---------------------------------------------------------------- If render2D is true, then alpha blending will be active
	(render2D)	? m_deviceContext->OMSetBlendState(m_alphaEnableBlendingState, blendFactor, 0xffffffff)
				: m_deviceContext->OMSetBlendState(m_alphaDisableBlendingState, blendFactor, 0xffffffff);
}


/*******************************************************************************************************************
	Accessor methods
*******************************************************************************************************************/
ID3D11Device* GraphicsManager::GetDevice()	const				{ return m_device; }
ID3D11DeviceContext* GraphicsManager::GetDeviceContext() const	{ return m_deviceContext; }
const char* GraphicsManager::GetVideoCardInfo() const			{ return &m_videoCardInfo[0]; }
int GraphicsManager::GetVideoCardMemory() const					{ return m_videoCardMemory; }