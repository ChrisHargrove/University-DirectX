#ifndef DX_APPLICATION_H_
#define DX_APPLICATION_H_

#define WINDOWED WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX
#define FULLSCREEN WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP

#define MultiSampleCount 8
#define MultiSampleQuality 0

/*******************************************************************************************************************
Class that creates a WIN32 Window that supports Direct3D
*******************************************************************************************************************/
#include <d3d11.h>
#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <xnamath.h>

#include "DXInput.h"
#include "FirstPersonCamera.h"

class DXApplication {

public:
	DXApplication(HINSTANCE hInstance);
	virtual ~DXApplication();

	int			 Run();

	virtual bool Initialize();
	virtual void Shutdown();

	LRESULT		 MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

protected:
	virtual bool Update(float dt) = 0;
	virtual void Render(float dt) = 0;

protected:
	HRESULT LoadVertexShader(WCHAR* fileLocation);
	HRESULT LoadPixelShader(WCHAR* fileLocation);

protected:
	HINSTANCE					m_appInstance;
	HWND						m_appWindow;
	UINT						m_clientWidth;
	UINT						m_clientHeight;
	DWORD						m_windowStyle;
	LPCSTR						m_windowTitle;
	LPCSTR						m_windowClass;

	D3D_DRIVER_TYPE				m_driverType;
	D3D_FEATURE_LEVEL			m_featureLevel;
	ID3D11Device*				m_device;
	ID3D11Device1*				m_device1;
	ID3D11DeviceContext*		m_deviceContext;
	ID3D11DeviceContext1*		m_deviceContext1;
	IDXGISwapChain*				m_swapChain;
	IDXGISwapChain1*			m_swapChain1;
	ID3D11RenderTargetView*		m_backBuffer;
	ID3D11DepthStencilView*		m_depthStencilView;
	ID3D11Texture2D*			m_depthStencilBuffer;
	ID3D11DepthStencilState*	m_depthStencilState;
	ID3D11DepthStencilState*	m_depthDisabledStencilState;
	ID3D11BlendState*			m_alphaEnableBlendingState;
	ID3D11BlendState*			m_alphaDisableBlendingState;
	ID3D11PixelShader*			m_pixelShader;
	ID3D11VertexShader*			m_vertexShader;
	ID3D11InputLayout*			m_vertexLayout;
	ID3D11Buffer*				m_indexBuffer;
	ID3D11Buffer*				m_vertexBuffer;

protected:
	DXInput*					m_input;

private:
	HRESULT InitializeWindow();
	HRESULT InitializeDirect3D();
	HRESULT CreateSwapChain();
	HRESULT CreateRenderTargetView();
	HRESULT CreateDepthStencilView();
	void	SetupViewport();

	HRESULT CompileShaderFromFile(WCHAR* fileLocation, LPCSTR entryPoint, LPCSTR shaderVersion, ID3DBlob** errorBuffer);
};

#endif