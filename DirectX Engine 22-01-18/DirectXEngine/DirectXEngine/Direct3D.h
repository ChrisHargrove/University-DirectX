#ifndef DIRECT_3D_H_
#define DIRECT_3D_H_

#include <d3d11.h>
#include <d3d11_1.h>
#include <DirectXColors.h>
#include <DirectXMath.h>

struct CBNeverChanges
{
	DirectX::XMMATRIX mView;
};

struct CBChangeOnResize
{
	DirectX::XMMATRIX mProjection;
};


__declspec(align(16)) class Direct3D
{
public:
	Direct3D();
	~Direct3D();


	void* operator new(size_t i)
	{
		return _mm_malloc(i, 16);
	}

	void operator delete(void* p)
	{
		_mm_free(p);
	}

	HRESULT Initialize(HWND appWindow, int clientWidth, int clientHeight, bool vSync, bool fullScreen, float clientNear, float clientDepth);
	HRESULT CreateSwapChain(HWND appWindow, int clientWidth, int clientHeight);
	HRESULT CreateRenderTargetView();
	HRESULT CreateDepthStencilView(int clientWidth, int clientHeight);
	HRESULT CreateRasterizer();
	void SetupViewport(int clientWidth, int clientHeight);

	void Shutdown();

	void BeginScene(float, float, float, float);
	void EndScene();

	ID3D11Device* GetDevice() { return m_device; }
	ID3D11DeviceContext* GetDeviceContext() { return m_deviceContext; }


	void GetWorldMatrix(DirectX::XMMATRIX& worldMatrix) { worldMatrix = m_worldMatrix; }
	void GetViewMatrix(DirectX::XMMATRIX& viewMatrix) { viewMatrix = m_viewMatrix; }
	void GetProjectionMatrix(DirectX::XMMATRIX& projectionMatrix) { projectionMatrix = m_projectionMatrix; }
	void GetOrthoMatrix(DirectX::XMMATRIX& orthoMatrix) { orthoMatrix = m_orthoMatrix; }

	void GetVideoCardInfo(char* cardName, int& memory) {
		strcpy_s(cardName, 128, m_videoCardDescription);
		memory = m_videoCardMemory;
	}

private:
	bool m_vSync;
	int m_videoCardMemory;
	char m_videoCardDescription[128];

	D3D_DRIVER_TYPE			m_driverType;
	D3D_FEATURE_LEVEL		m_featureLevel;
	ID3D11Device*			m_device;
	ID3D11Device1*			m_device1;
	ID3D11DeviceContext*	m_deviceContext;
	ID3D11DeviceContext1*	m_deviceContext1;
	IDXGISwapChain*			m_swapChain;
	IDXGISwapChain1*		m_swapChain1;
	ID3D11RenderTargetView* m_backBuffer;
	ID3D11Texture2D*			m_depthStencilBuffer;
	ID3D11DepthStencilState*	 m_depthStencilState;
	ID3D11DepthStencilView*			m_depthStencilView;
	ID3D11RasterizerState*		m_rasterState;
	ID3D11Buffer*				CBneverChanges;
	ID3D11Buffer*			CBchangeonResize;


	DirectX::XMMATRIX m_worldMatrix;
	DirectX::XMMATRIX m_viewMatrix;
	DirectX::XMMATRIX m_projectionMatrix;
	DirectX::XMMATRIX m_orthoMatrix;
};


#endif
