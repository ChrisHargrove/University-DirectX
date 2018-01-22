#ifndef DX_APPLICATION_H_
#define DX_APPLICATION_H_

#define WIN32_LEAN_AND_MEAN
#define WINDOWED WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX
#define FULLSCREEN WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP

#include "Windows.h"
#include "DXInput.h"
#include "DXGraphics.h"

class DXApplication {

public:
	DXApplication(HINSTANCE hInstance);
	virtual ~DXApplication();

	int Run();
	virtual bool Initialize();
	virtual bool Update(float dt) = 0;
	virtual bool Render(float dt) = 0;
	virtual void Shutdown();
	virtual LRESULT MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

private:
	HRESULT InitializeWindow();
	void ShutdownWindow();

protected:
	HINSTANCE				m_appInstance;
	HWND					m_appWindow;
	UINT					m_clientWidth;
	UINT					m_clientHeight;
	DWORD					m_windowStyle;
	LPCSTR					m_appTitle;
	LPCSTR					m_windowClass;
	bool					m_fullScreen;

	DXInput* m_dxInput;
	DXGraphics* m_dxGraphics;
};

#endif