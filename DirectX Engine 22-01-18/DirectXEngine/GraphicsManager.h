#pragma once

/*******************************************************************************************************************
	GraphicsManager.h, GraphicsManager.cpp
	Created by Kim Kane
	Last updated: 09/02/2018

	Singleton class that creates and initializes the DirectX graphics API.
	
*******************************************************************************************************************/
#pragma comment(lib, "dxgi.lib")

#include "Singleton.h"
#include <d3d11.h>
#include <d3dcompiler.h>
#include <dxgi.h>
#include <array>
#include "Constants.h"

class GraphicsManager {

public:
	friend class Singleton<GraphicsManager>;

public:
	bool Initialize();
	void Shutdown();

public:
	void BeginScene(float red, float green, float blue, float alpha);
	void EndScene();

public:
	ID3D11Device* GetDevice() const;
	ID3D11DeviceContext* GetDeviceContext() const;
	
	const char* GetVideoCardInfo() const;
	int GetVideoCardMemory() const;

public:
	void EnableDepthBuffer(bool enable3D);
	void EnableAlphaBlending(bool render2D);

private:
	bool InitializeInterface();
	bool StoreVideoCardInfo(IDXGIAdapter* adapter);
	bool InitializeSwapChain(unsigned int numerator, unsigned int denominator);
	bool InitializeRenderTargetView();
	bool InitializeDepthStencilFilters();
	bool InitializeRasterizerState();
	void InitializeViewport();
	bool InitializeAlphaBlendingState();

private:
	GraphicsManager();
	GraphicsManager(const GraphicsManager&);
	GraphicsManager& operator=(const GraphicsManager&) {}

private:
	int							m_videoCardMemory;
	const int					m_multiSampleCount, m_multiSampleQuality;

	IDXGISwapChain*				m_swapChain;
	ID3D11Device*				m_device;
	ID3D11DeviceContext*		m_deviceContext;
	ID3D11RenderTargetView*		m_renderTargetView;
	ID3D11Texture2D*			m_depthStencilBuffer;
	ID3D11DepthStencilState*	m_depthStencilState;
	ID3D11DepthStencilView*		m_depthStencilView;
	ID3D11RasterizerState*		m_rasterState;
	ID3D11DepthStencilState*	m_depthDisabledStencilState;
	ID3D11BlendState*			m_alphaEnableBlendingState;
	ID3D11BlendState*			m_alphaDisableBlendingState;

private:
	std::array<char, GraphicConstants::BUFFER_SIZE>	m_videoCardInfo;
};

typedef Singleton<GraphicsManager> Graphics;