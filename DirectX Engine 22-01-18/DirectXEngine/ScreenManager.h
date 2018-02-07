#pragma once

/*******************************************************************************************************************
	ScreenManager.h, ScreenManager.cpp
	Created by Kim Kane
	Last updated: 20/01/2018

	Singleton class that creates and initializes a WIN32 Window.

*******************************************************************************************************************/
#define WIN_32_LEAN_AND_MEAN

#include <Windows.h>
#include <d3d11.h>
#include <xnamath.h>
#include "Singleton.h"

class ScreenManager {
	
public:
	friend class Singleton<ScreenManager>;

public:
	void Initialize(HINSTANCE instance, LPCSTR title, unsigned int width, unsigned int height, bool fullScreen, bool vSync);
	void Shutdown();

	LRESULT WindowProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam);

public:
	HWND GetWindow() const;
	HINSTANCE GetWindowInstance() const;
	
public:
	float GetAspectRatio() const;
	unsigned int GetWidth() const;
	unsigned int GetHeight() const;

public:
	XMMATRIX GetPerspectiveMatrix();
	XMMATRIX GetWorldMatrix();
	XMMATRIX GetOrthographicMatrix();

public:
	bool IsVSyncEnabled() const;
	bool IsFullScreen() const;
	bool Is3dEnabled() const;

	void Enable3DView(bool viewIn3D);

private:
	ScreenManager();
	ScreenManager(const ScreenManager&) {}
	ScreenManager& operator=(const ScreenManager&) {}

private:
	bool InitializeWindow(unsigned int width, unsigned int height);
	void InitializePerspective();
	void InitializeOrthographic();

private:
	LPCSTR			m_title;
	HINSTANCE		m_instance;
	HWND			m_window;

	unsigned int	m_width, m_height;

	bool			m_fullScreen;
	bool			m_vSync;
	bool			m_is3dEnabled;

	XMMATRIX		m_perspectiveMatrix;
	XMMATRIX		m_orthographicMatrix;
	XMMATRIX		m_worldMatrix;
};

typedef Singleton<ScreenManager> Screen;