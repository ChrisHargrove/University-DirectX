#include "Game.h"
#include "Log.h"
#include "Memory.h"

/*******************************************************************************************************************
Game constructor to initialize default values
*******************************************************************************************************************/
Game::Game(HINSTANCE hInstance, const char* title, int clientWidth, int clientHeight) : DXApplication(hInstance)
{
	m_windowTitle = title;
	m_clientWidth = clientWidth;
	m_clientHeight = clientHeight;

	Log("[GAME] Game constructor initialized", EMPTY, LOG_MESSAGE);
}


/*******************************************************************************************************************
Initialize the game and run all startup functions
*******************************************************************************************************************/
bool Game::Initialize()
{
	DXApplication::Initialize();

	SetupConstantBuffers();
	SetupAlphaBlending();
	LoadResources();

	m_projectionMatrix = XMMatrixPerspectiveFovLH(XM_PIDIV4, 800.0f / 600.0f, 0.01f, 100.0f);
	m_projectionMatrix = XMMatrixTranspose(m_projectionMatrix);

	m_firstPersonCamera.SetDistance(12.0f, 4.0f, 20.0f);

	LaraObject = new GameObject(XMVectorSet(8.0, 0.5, -10.0, 1.0), &LaraModel, &LaraTexture);
	CubeObject = new GameObject(XMVectorSet(3.0, 0.5, 6.0, 1.0), &CubeModel, &CubeTexture);

	return true;
}


/*******************************************************************************************************************
Function that sets up all our game-specific constant buffers
*******************************************************************************************************************/
HRESULT Game::SetupConstantBuffers()
{
	HRESULT result = S_OK;

	D3D11_BUFFER_DESC constDesc;
	ZeroMemory(&constDesc, sizeof(constDesc));
	constDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constDesc.ByteWidth = sizeof(XMMATRIX);
	constDesc.Usage = D3D11_USAGE_DEFAULT;

	result = m_device->CreateBuffer(&constDesc, 0, &m_viewBuffer);
	if (FAILED(result)) { Log("[GAME] Couldn't create view buffer", EMPTY, LOG_ERROR);  return result; }

	result = m_device->CreateBuffer(&constDesc, 0, &m_projectionBuffer);
	if (FAILED(result)) { Log("[GAME] Couldn't create projection buffer", EMPTY, LOG_ERROR);  return result; }

	result = m_device->CreateBuffer(&constDesc, 0, &m_worldBuffer);
	if (FAILED(result)) { Log("[GAME] Couldn't create world buffer", EMPTY, LOG_ERROR);  return result; }

	constDesc.ByteWidth = sizeof(XMFLOAT4);
	result = m_device->CreateBuffer(&constDesc, 0, &m_cameraPositionBuffer);
	if (FAILED(result)) { Log("[GAME] Couldn't create camera position buffer", EMPTY, LOG_ERROR);  return result; }

	return S_OK;
}


/*******************************************************************************************************************
Function that sets up the alpha blending state
*******************************************************************************************************************/
HRESULT Game::SetupAlphaBlending()
{
	HRESULT result = S_OK;

	D3D11_BLEND_DESC blendStateDescription;
	ZeroMemory(&blendStateDescription, sizeof(D3D11_BLEND_DESC));
	// Create an alpha enabled blend state description.
	blendStateDescription.RenderTarget[0].BlendEnable = TRUE;
	//blendStateDescription.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	blendStateDescription.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendStateDescription.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendStateDescription.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	//blendStateDescription.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendStateDescription.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;
	blendStateDescription.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendStateDescription.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendStateDescription.RenderTarget[0].RenderTargetWriteMask = 0x0f;

	//---------------------------------------------------------------- Create the enabled blend state using the description above
	result = m_device->CreateBlendState(&blendStateDescription, &m_alphaEnableBlendingState);

	if (FAILED(result)) { Log("[GAME] Couldn't create enabled blend state", EMPTY, LOG_ERROR);  return result; }

	//---------------------------------------------------------------- Modify the description to create an alpha disabled blend state description
	blendStateDescription.RenderTarget[0].BlendEnable = FALSE;

	//---------------------------------------------------------------- Create the disabled blend state
	result = m_device->CreateBlendState(&blendStateDescription, &m_alphaDisableBlendingState);

	if (FAILED(result)) { Log("[GAME] Couldn't create disabled blend state", EMPTY, LOG_ERROR); return result; }

	return S_OK;
}


/*******************************************************************************************************************
Loads all game specific resources - temporary function until we add gamestates
*******************************************************************************************************************/
bool Game::LoadResources()
{
	//Resource manager?
	LoadVertexShader(L"Assets\\Shaders\\Effects.fx");
	LoadPixelShader(L"Assets\\Shaders\\Effects.fx");

	if (!LaraModel.Load("Assets\\Objects\\Lara.obj", m_device)) { return false; };
	if (!CubeModel.Load("Assets\\Objects\\Cube.obj", m_device)) { return false; };

	if (!LaraTexture.LoadTexture("Assets\\Textures\\Lara.png", m_device)) { return false; };
	if (!CubeTexture.LoadTexture("Assets\\Textures\\Cube.jpg", m_device)) { return false; };

	return true;
}


/*******************************************************************************************************************
Function that clears the screen (render target view and depth stencil view) every frame with colour provided
*******************************************************************************************************************/
void Game::ClearScreen()
{
	float colors[4] = { 0.2f, 0.2f, 0.5f, 1.0f }; //Read values in from file
	m_deviceContext->ClearRenderTargetView(m_backBuffer, colors);
	m_deviceContext->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0); //Read values in from file
}


/*******************************************************************************************************************
Function that turns alpha blending off or on
*******************************************************************************************************************/
void Game::AlphaBlendingState(bool state)
{
	float blendFactor[4]; //Read values in from file

	blendFactor[0] = 0.0f;
	blendFactor[1] = 0.0f;
	blendFactor[2] = 0.0f;
	blendFactor[3] = 0.0f;

	(state) ? m_deviceContext->OMSetBlendState(m_alphaEnableBlendingState, blendFactor, 0xffffffff)
			: m_deviceContext->OMSetBlendState(m_alphaDisableBlendingState, blendFactor, 0xffffffff);
}


/*******************************************************************************************************************
Function that turns Z buffer off or on
*******************************************************************************************************************/
void Game::ZBufferState(bool state)
{
	(state) ? m_deviceContext->OMSetDepthStencilState(m_depthStencilState, 1)
			: m_deviceContext->OMSetDepthStencilState(m_depthDisabledStencilState, 1);
}


/*******************************************************************************************************************
Virtual function used to update the screen every frame
*******************************************************************************************************************/
bool Game::Update(float dt)
{
	m_input->DetectInput();

	static float moveSpeed = 0.1f;
	static float zoom = 0.0f;
	float xRotation = 0.0f;
	float yRotation = 0.0f;

	float moveLeftRight = 0.0f;
	float moveBackForward = 0.0f;

	//Keyboard Testing
	if (m_input->IsKeyPressed(DIK_LEFT)) { moveLeftRight -= moveSpeed * 0.03f; }
	if (m_input->IsKeyPressed(DIK_RIGHT)) { moveLeftRight += moveSpeed * 0.03f;; }
	if (m_input->IsKeyPressed(DIK_DOWN)) { moveBackForward -= moveSpeed * 0.03f;; }
	if (m_input->IsKeyPressed(DIK_UP)) { moveBackForward += moveSpeed * 0.03f;; }
	if (m_input->IsKeyPressed(DIK_SUBTRACT)) { zoom += moveSpeed * 2.0f; }
	if (m_input->IsKeyPressed(DIK_ADD)) { zoom -= moveSpeed * 2.0f; }


	//TESTING--DONT TOUCH!
	if (m_input->IsKeyPressed(DIK_J)) {
		LaraObject->TranslateX(0.0001);
	}


	//Mouse Testing
	if (m_input->IsButtonPressed(0)) { yRotation -= 0.003f/ 70.00f; }
	if (m_input->IsButtonPressed(1)) { yRotation += 0.003f / 70.00f; }
	xRotation += m_input->GetMouseWheel() / 1000.0f;

	if (xRotation > XM_PI / 8) { xRotation = XM_PI / 8; }
	if (xRotation<-(XM_PI / 8)) { xRotation = -(XM_PI / 8); }

	//Camera testing
	m_firstPersonCamera.Move(moveLeftRight, moveBackForward);
	m_firstPersonCamera.ApplyZoom(zoom); //doesn't work
	m_firstPersonCamera.ApplyRotation(xRotation, yRotation);
	
	if (m_input->IsKeyPressed(DIK_ESCAPE)) { return false; }

	return true;
}


/*******************************************************************************************************************
Virtual function used to draw to the screen
*******************************************************************************************************************/
void Game::Render(float dt)
{
	ClearScreen();

	//This can go inside the DXApplication engine, unless we want multiple shaders
	m_deviceContext->IASetInputLayout(m_vertexLayout);
	m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_deviceContext->VSSetShader(m_vertexShader, 0, 0);
	m_deviceContext->PSSetShader(m_pixelShader, 0, 0);

	//Needs to be in a Transform class/hierarchy
	XMMATRIX worldMatrix = XMMatrixIdentity();
	worldMatrix = XMMatrixTranspose(worldMatrix);

	XMMATRIX viewMatrix = m_firstPersonCamera.GetViewMatrix();
	viewMatrix = XMMatrixTranspose(viewMatrix);

	XMFLOAT3 cameraPosition = m_firstPersonCamera.GetPosition();

	m_deviceContext->UpdateSubresource(m_worldBuffer, 0, 0, &worldMatrix, 0, 0);
	m_deviceContext->UpdateSubresource(m_viewBuffer, 0, 0, &viewMatrix, 0, 0);
	m_deviceContext->UpdateSubresource(m_projectionBuffer, 0, 0, &m_projectionMatrix, 0, 0);
	m_deviceContext->UpdateSubresource(m_cameraPositionBuffer, 0, 0, &cameraPosition, 0, 0);

	m_deviceContext->VSSetConstantBuffers(0, 1, &m_worldBuffer);
	m_deviceContext->VSSetConstantBuffers(1, 1, &m_viewBuffer);
	m_deviceContext->VSSetConstantBuffers(2, 1, &m_projectionBuffer);
	m_deviceContext->VSSetConstantBuffers(3, 1, &m_cameraPositionBuffer);

	//XMVECTOR cameraPosition = XMLoadFloat3(&m_firstPersonCamera.GetPosition());

	//Needs to be in game objects update function
	m_deviceContext->UpdateSubresource(m_worldBuffer, 0, 0, &LaraObject->GetWorldMatrix(), 0, 0);
	m_deviceContext->VSSetConstantBuffers(0, 1, &m_worldBuffer);

	//Render Lara Game Object
	LaraObject->Render(m_deviceContext);

	//Needs to be in game objects update function
	m_deviceContext->UpdateSubresource(m_worldBuffer, 0, 0, &CubeObject->GetWorldMatrix(), 0, 0);
	m_deviceContext->VSSetConstantBuffers(0, 1, &m_worldBuffer);

	//Render Car Game Object
	CubeObject->Render(m_deviceContext);

	m_swapChain->Present(0, 0);


	 /************************************************* TESTING AREA & COMMENTS ***********************************************************/

	///////////////////////////////rotational movement (from camera)////////////////////////

	//XMVECTOR DefaultForward_ ;
	//XMVECTOR DefaultRight_;
	//XMVECTOR camForward_;
	//XMVECTOR camRight_;

	///////////////////////////////////////////////////////////////////////////////////////////

	//SetDistance( 2.0f, 1.0f, 10.0f );
	//SetRotation( 0.0f, 0.0f, -XM_PIDIV4/3, XM_PIDIV4/2 );
	////SetRotation( 0.0f, 0.0f, -XM_PIDIV4/4, XM_PIDIV4/4 );

	//DefaultForward_ = XMVectorSet(0.0f,0.0f,1.0f, 0.0f);
	//DefaultRight_ = XMVectorSet(1.0f,0.0f,0.0f, 0.0f);
	//camForward_ = XMVectorSet(0.0f,0.0f,1.0f, 0.0f);
	//camRight_ = XMVectorSet(1.0f,0.0f,0.0f, 0.0f);

	//////////////////////////////////////////////////////////////////////////////////////////

	//XMMATRIX  camRotationMatrix = XMMatrixRotationRollPitchYaw(xRotation_, yRotation_, 0);
	//XMVECTOR camTarget = XMVector3TransformCoord(DefaultForward_, camRotationMatrix );
	//camTarget = XMVector3Normalize(camTarget);

	//XMMATRIX RotateYTempMatrix;
	//RotateYTempMatrix = XMMatrixRotationY(yRotation_);

	//camRight_ = XMVector3TransformCoord(DefaultRight_, RotateYTempMatrix);
	//camUp_ = XMVector3TransformCoord(camUp_, RotateYTempMatrix);
	//camForward_ = XMVector3TransformCoord(DefaultForward_, RotateYTempMatrix);

	//XMVECTOR pos = XMLoadFloat3( &position_ );

	//pos += leftRight*camRight_;
	//pos += backFoward*camForward_;

	//XMStoreFloat3( &position_, pos );
	////moveLeftRight = 0.0f;
	////moveBackForward = 0.0f;

	//camTarget = pos + camTarget;	

	//XMStoreFloat3( &direction_, camTarget );

	//camView_ = XMMatrixLookAtLH( pos, camTarget, camUp_ );

	///////////////////////////////(END) rotational movement (from camera)//////////////////


	/*
	// Update our time
	static float t = 0.0f;
	if (m_driverType == D3D_DRIVER_TYPE_REFERENCE)
	{
		t += (float)XM_PI * 0.0125f;
	}
	else
	{
		static ULONGLONG timeStart = 0;
		ULONGLONG timeCur = GetTickCount64();
		if (timeStart == 0)
			timeStart = timeCur;
		t = (timeCur - timeStart) / 1000.0f;
	}

	static float index = 0.0f;
	static float index2 = 0.0f;
	static float maximise = 0.0f;
	static float zaxis = 0.0f;
	static float index3 = 0.0f;

	if ((m_input->IsKeyPressed(DIK_DOWN))) {
		index2 -= 0.0003f;
	}

	
	if ((m_input->IsKeyPressed(DIK_LEFT))) {
		index -= 0.0003f;
	}
	if ((m_input->IsKeyPressed(DIK_RIGHT))) {
		index += 0.0003f;
	}

	if ((m_input->IsKeyPressed(DIK_UP))) {
		index2 += 0.0003f;
	}

	if ((m_input->IsKeyPressed(DIK_SPACE))) {
		maximise += 0.0003f;
		if (maximise >= 0.5f) {
			maximise = 0.0f;
		}
	}
	if ((m_input->IsKeyPressed(DIK_BACKSPACE))) {
		zaxis -= 0.0003f;
	}

	 if ((m_input->IsKeyPressed(DIK_RETURN))) {
		zaxis += 0.0003f;
	}

	index += m_input->GetMouseX() * 0.003f;
	index2 += m_input->GetMouseY() * 0.003f;
	zaxis += m_input->GetMouseWheel() * 0.003f;
	


	maximise += m_input->IsButtonPressed(LEFT)* 0.0000003f;
	//maximise -= m_input->GetMouseState().rgbButtons[1] * 0.0000003f;
	// 1st Cube: Rotate around the origin

	XMMATRIX m_Orbit0 = XMMatrixRotationY(t * 10.0f);
	XMMATRIX m_Translate0 = XMMatrixTranslation(index, index2, zaxis);
	XMMATRIX m_Scale0 = XMMatrixScaling(0.6f + maximise, 0.4f + maximise, 0.3f);
	m_World1 = m_Scale0 * m_Translate0;

	// 2nd Cube:  Rotate around origin
	// XMMATRIX mSpin = XMMatrixRotationZ( -t );
	XMMATRIX mOrbit = XMMatrixRotationY(t * 10.0f);
	XMMATRIX mTranslate = XMMatrixTranslation(-1.0f + index, index2, zaxis);
	XMMATRIX mScale = XMMatrixScaling(0.5f + maximise, 0.4f + maximise, 0.3f);

	m_World2 = mScale * mTranslate;//mScale * mTranslate * mOrbit;


								   // 3rd Cube:  Rotate around origin
								   // XMMATRIX mSpin = XMMatrixRotationZ( -t );
	XMMATRIX mOrbit2 = XMMatrixRotationY(t * 10.0f);
	XMMATRIX mTranslate2 = XMMatrixTranslation(1.0f + index, index2, zaxis);
	XMMATRIX mScale2 = XMMatrixScaling(0.5f + maximise, 0.4f + maximise, 0.3f);

	m_World3 = mScale2 * mTranslate2;//mScale2 * mTranslate2 * mOrbit2;

									 //
									 // Clear the back buffer
									 //

									 // Clear the back buffer
	m_deviceContext->ClearRenderTargetView(m_backBuffer, Colors::MidnightBlue);

	// Clear the depth buffer to 1.0 (max depth)
	m_deviceContext->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
	//
	// Update variables for the first cube
	//
	ConstantBuffer cb1;
	cb1.mWorld = XMMatrixTranspose(m_World1);
	cb1.mView = XMMatrixTranspose(m_View);
	cb1.mProjection = XMMatrixTranspose(m_Projection);
	m_deviceContext->UpdateSubresource(m_constantBuffer, 0, nullptr, &cb1, 0, 0);

	//
	// Render the first cube
	//
	m_deviceContext->VSSetShader(m_vertexShader, nullptr, 0);
	m_deviceContext->VSSetConstantBuffers(0, 1, &m_constantBuffer);
	m_deviceContext->PSSetShader(m_pixelShader, nullptr, 0);
	m_deviceContext->DrawIndexed(36, 0, 0);

	//
	// Update variables for the second cube
	//
	ConstantBuffer cb2;
	cb2.mWorld = XMMatrixTranspose(m_World2);
	cb2.mView = XMMatrixTranspose(m_View);
	cb2.mProjection = XMMatrixTranspose(m_Projection);
	m_deviceContext->UpdateSubresource(m_constantBuffer, 0, nullptr, &cb2, 0, 0);

	//
	// Render the second cube
	//
	m_deviceContext->DrawIndexed(36, 0, 0);


	//
	// Update variables for the third cube
	//
	ConstantBuffer cb3;
	cb3.mWorld = XMMatrixTranspose(m_World3);
	cb3.mView = XMMatrixTranspose(m_View);
	cb3.mProjection = XMMatrixTranspose(m_Projection);
	m_deviceContext->UpdateSubresource(m_constantBuffer, 0, nullptr, &cb3, 0, 0);

	//
	// Render the third cube
	//
	m_deviceContext->DrawIndexed(36, 0, 0);

	//
	// Present our back buffer to our front buffer
	//
	m_swapChain->Present(0, 0);

	*/
}


/*******************************************************************************************************************
User defined cleanup function that deletes all objects and shuts down the game safely
*******************************************************************************************************************/
void Game::Shutdown()
{
	LaraObject->GetTexture()->UnloadTexture();
	CubeObject->GetTexture()->UnloadTexture();

	SafeDelete(LaraObject);
	SafeDelete(CubeObject);
	SafeRelease(m_cameraPositionBuffer);
	SafeRelease(m_worldBuffer);
	SafeRelease(m_projectionBuffer);
	SafeRelease(m_viewBuffer);

	Log("[GAME] Game shut down successfully!", EMPTY, LOG_SUCCESS);
}


/*******************************************************************************************************************
Game destructor used for cleaning up memory, deleting all objects and calling shutdown functions
*******************************************************************************************************************/
Game::~Game()
{
	Shutdown();

	Log("[GAME] Game destructor initialized", EMPTY, LOG_MESSAGE);
}