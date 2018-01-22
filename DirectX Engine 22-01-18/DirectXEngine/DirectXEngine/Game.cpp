#include "Game.h"

#include "Log.h"

Game::Game(HINSTANCE hInstance, const char* title, int clientWidth, int clientHeight) : DXApplication(hInstance)
{
	m_appTitle = title;
	m_clientWidth = clientWidth;
	m_clientHeight = clientHeight;

	Log("[GAME] Game constructor initialized", EMPTY);
}

Game::~Game()
{

}

bool Game::Initialize()
{
	return DXApplication::Initialize();
}

bool Game::Update(float dt)
{
	bool result;


	// Check if the user pressed escape and wants to exit the application.
	if (m_dxInput->IsKeyDown(VK_ESCAPE))
	{
		return false;
	}

	// Do the frame processing for the graphics object.
	result = m_dxGraphics->Update();
	if (!result)
	{
		std::cout << "cant update graphics" << std::endl;

		return false;
	}

	return true;
}

bool Game::Render(float dt)
{

	bool result;


	// Render the graphics scene.
	result = m_dxGraphics->Render();
	if (!result)
	{
		std::cout << "cant render graphics" << std::endl;

		return false;
	}

	return true;
	// clear the back buffer to a deep blue
	//m_pDeviceContext->ClearRenderTargetView(m_pBackBuffer, DirectX::Colors::MidnightBlue);

	//m_pSwapChain->Present(0, 0);


	// do 3D rendering on the back buffer here


	// switch the back buffer and the front buffer


	/*
	
	
		// Present the back buffer to the screen since rendering is complete.
		//VSYNC////////////////////////////////////////////////////////////
	if(m_vsync_enabled)
	{
		// Lock to screen refresh rate.
		m_swapChain->Present(1, 0);
	}
	else
	{
		// Present as fast as possible.
		m_swapChain->Present(0, 0);
	}*/





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

	// 1st Cube: Rotate around the origin
	m_World1 = XMMatrixRotationY(t);

	// 2nd Cube:  Rotate around origin
	XMMATRIX mSpin = XMMatrixRotationZ(-t);
	XMMATRIX mOrbit = XMMatrixRotationY(-t * 2.0f);
	XMMATRIX mTranslate = XMMatrixTranslation(-4.0f, 0.0f, 0.0f);
	XMMATRIX mScale = XMMatrixScaling(0.3f, 0.3f, 0.3f);

	m_World2 = mScale * mSpin * mTranslate * mOrbit;

	// Setup our lighting parameters
	XMFLOAT4 vLightDirs[2] =
	{
	XMFLOAT4(-0.577f, 0.577f, -0.577f, 1.0f),
	XMFLOAT4(0.0f, 0.0f, -1.0f, 1.0f),
	};
	XMFLOAT4 vLightColors[2] =
	{
	XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f),
	XMFLOAT4(0.5f, 0.0f, 0.0f, 1.0f)
	};

	// Rotate the second light around the origin
	XMMATRIX mRotate = XMMatrixRotationY(-2.0f * t);
	XMVECTOR vLightDir = XMLoadFloat4(&vLightDirs[1]);
	vLightDir = XMVector3Transform(vLightDir, mRotate);
	XMStoreFloat4(&vLightDirs[1], vLightDir);

	// Clear the back buffer
	m_pImmediateContext->ClearRenderTargetView(m_pRenderTargetView, Colors::MidnightBlue);


	//
	// Clear the depth buffer to 1.0 (max depth)
	//
	m_pImmediateContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);


	//
	// Update matrix variables and lighting variables
	//
	ConstantBuffer cb1;
	cb1.mWorld = XMMatrixTranspose(m_World1);
	cb1.mView = XMMatrixTranspose(m_View);
	cb1.mProjection = XMMatrixTranspose(m_Projection);
	cb1.vLightDir[0] = vLightDirs[0];
	cb1.vLightDir[1] = vLightDirs[1];
	cb1.vLightColor[0] = vLightColors[0];
	cb1.vLightColor[1] = vLightColors[1];
	cb1.vOutputColor = XMFLOAT4(0, 0, 0, 0);
	m_pImmediateContext->UpdateSubresource(m_pConstantBuffer, 0, nullptr, &cb1, 0, 0);

	//
	// Render the cube
	//
	m_pImmediateContext->VSSetShader(m_pVertexShader, nullptr, 0);
	m_pImmediateContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);
	m_pImmediateContext->PSSetShader(m_pPixelShader, nullptr, 0);
	m_pImmediateContext->PSSetConstantBuffers(0, 1, &m_pConstantBuffer);
	m_pImmediateContext->DrawIndexed(36, 0, 0);

	//
	// Render each light
	//
	for (int m = 0; m < 2; m++)
	{
	XMMATRIX mLight = XMMatrixTranslationFromVector(5.0f * XMLoadFloat4(&vLightDirs[m]));
	XMMATRIX mLightScale = XMMatrixScaling(0.2f, 0.2f, 0.2f);
	mLight = mLightScale * mLight;

	// Update the world variable to reflect the current light
	cb1.mWorld = XMMatrixTranspose(mLight);
	cb1.vOutputColor = vLightColors[m];
	m_pImmediateContext->UpdateSubresource(m_pConstantBuffer, 0, nullptr, &cb1, 0, 0);

	m_pImmediateContext->PSSetShader(m_pPixelShaderSolid, nullptr, 0);
	m_pImmediateContext->DrawIndexed(36, 0, 0);
	}

	// Update variables for first cube
	//
	ConstantBuffer cb1;
	cb1.mWorld = XMMatrixTranspose(m_World1);
	cb1.mView = XMMatrixTranspose(m_View);
	cb1.mProjection = XMMatrixTranspose(m_Projection);
	m_pImmediateContext->UpdateSubresource(m_pConstantBuffer, 0, nullptr, &cb1, 0, 0);

	//
	// Render the first cube
	//
	m_pImmediateContext->VSSetShader(m_pVertexShader, nullptr, 0);
	m_pImmediateContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);
	m_pImmediateContext->PSSetShader(m_pPixelShader, nullptr, 0);
	m_pImmediateContext->DrawIndexed(36, 0, 0);        // 36 vertices needed for 12 triangles in a triangle list


	//
	// Update variables for the second cube
	//
	ConstantBuffer cb2;
	cb2.mWorld = XMMatrixTranspose(m_World2);
	cb2.mView = XMMatrixTranspose(m_View);
	cb2.mProjection = XMMatrixTranspose(m_Projection);
	m_pImmediateContext->UpdateSubresource(m_pConstantBuffer, 0, nullptr, &cb2, 0, 0);

	//
	// Render the second cube
	//
	m_pImmediateContext->DrawIndexed(36, 0, 0);

	// Present our back buffer to our front buffer

	m_pSwapChain->Present(0, 0);*/
}