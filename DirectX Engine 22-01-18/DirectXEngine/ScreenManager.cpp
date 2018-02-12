#include "ScreenManager.h"
#include "Log.h"
#include "Constants.h"

/*******************************************************************************************************************
	Pointer used to forward messages to user defined WindowProc function
	Contained within an empty namespace so it is local to this translation unit, this means that if another variable
	with the same name is defined elsewhere there will not be a violation of the One Definition Rule.
*******************************************************************************************************************/
namespace { ScreenManager* g_applicationHandle; }


LRESULT CALLBACK MessageHandler(HWND window, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (g_applicationHandle)	{ return g_applicationHandle->WindowProc(window, message, wParam, lParam); }
	else						{ return DefWindowProc(window, message, wParam, lParam); }
}


/*******************************************************************************************************************
	Constructor with initializer list to set all default values of variables
*******************************************************************************************************************/
ScreenManager::ScreenManager()	:	m_title("Window Title"),
									m_instance(nullptr),
									m_window(nullptr),
									m_width(GetSystemMetrics(SM_CXSCREEN)),
									m_height(GetSystemMetrics(SM_CYSCREEN)),
									m_fullScreen(false),
									m_vSync(false),
									m_is3dEnabled(true),
									m_perspectiveMatrix(XMMatrixIdentity()),
<<<<<<< HEAD
									m_orthographicMatrix(XMMatrixIdentity()),
									m_worldMatrix(XMMatrixIdentity())
=======
									m_orthographicMatrix(XMMatrixIdentity())
>>>>>>> master
{

	g_applicationHandle = this;

	{ DX_LOG("[SCREEN] Screen constructor initialized", DX_LOG_EMPTY, LOG_MESSAGE); }
}


/*******************************************************************************************************************
	Function that initializes the screen default setup settings
*******************************************************************************************************************/
void ScreenManager::Initialize(HINSTANCE instance, LPCSTR title, unsigned int width, unsigned int height, bool fullScreen, bool vSync)
{
	m_instance = instance;
	m_title = title;
	m_fullScreen = fullScreen;
	m_vSync = vSync;

	//-------------------------------------------- Initialize the window.
	if (!InitializeWindow(width, height)) { DX_LOG("[SCREEN] Problem initializing window", DX_LOG_EMPTY, LOG_ERROR); }
}


/*******************************************************************************************************************
	Function that shuts down all necessary procedures, releases resources and cleans up memory
*******************************************************************************************************************/
void ScreenManager::Shutdown()
{
	//-------------------------------------------- Show the mouse cursor.
	ShowCursor(true);

	//-------------------------------------------- Fix the display settings if leaving full screen mode.
	if (m_fullScreen) { ChangeDisplaySettings(NULL, 0); }

	//-------------------------------------------- Remove the window.
	DestroyWindow(m_window);
	m_window = nullptr;

	//-------------------------------------------- Remove the application instance.
	UnregisterClass(m_title, m_instance);
	m_instance = nullptr;


	DX_LOG("[SCREEN] Screen shutdown successfully", DX_LOG_EMPTY, LOG_SUCCESS);
}


/*******************************************************************************************************************
	Function that initializes a WIN32 Window
*******************************************************************************************************************/
bool ScreenManager::InitializeWindow(unsigned int width, unsigned int height)
{
	//-------------------------------------------- Create the window class.
	WNDCLASSEX WIN32Window = { 0 };

	//-------------------------------------------- The size, in bytes, of this structure.
	WIN32Window.cbSize = sizeof(WNDCLASSEX);

	//-------------------------------------------- Redraw the window if it is moved vertically or horizontally, give it its own device context.
	WIN32Window.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;

	//-------------------------------------------- What function to use when the class gets a message from Windows OS.
	WIN32Window.lpfnWndProc = MessageHandler;

	//-------------------------------------------- Number of extra bytes to allocate following the window class structure.
	WIN32Window.cbClsExtra = 0;

	//-------------------------------------------- Number of extra bytes to allocate following the window instance.
	WIN32Window.cbWndExtra = 0;

	//-------------------------------------------- It is a handle to a copy of our application.
	WIN32Window.hInstance = m_instance;

	//-------------------------------------------- A handle to the window icon. MAKEINTRESOURCE can be used for custom icons.
	WIN32Window.hIcon = LoadIcon(nullptr, IDI_WINLOGO);

	//-------------------------------------------- A handle to a small icon that is associated with the window.
	WIN32Window.hIconSm = WIN32Window.hIcon;

	//-------------------------------------------- Stores the default mouse cursor for the window.
	WIN32Window.hCursor = LoadCursor(nullptr, IDC_ARROW);

	//-------------------------------------------- The background colour of our window.
	WIN32Window.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);

	//-------------------------------------------- For menus.
	WIN32Window.lpszMenuName = nullptr;

	//-------------------------------------------- The name of our window class (the same name as the window title for simplicity).
	WIN32Window.lpszClassName = m_title;

	//-------------------------------------------- Register the window class.
	if (!RegisterClassEx(&WIN32Window)) {
		DX_LOG("[SCREEN] Couldn't register window class", DX_LOG_EMPTY, LOG_ERROR);
		return false; 
	}

	//-------------------------------------------- Setup the screen settings depending on whether it is running in full screen or in windowed mode.
	DEVMODE dmScreenSettings = { 0 };
	XMFLOAT2 windowPosition;

	if (m_fullScreen)
	{
		//-------------------------------------------- If full screen set the screen to maximum size of the users desktop and 32bit.
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = (unsigned long)m_width;
		dmScreenSettings.dmPelsHeight = (unsigned long)m_height;
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		//-------------------------------------------- Change the display settings to full screen.
		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		//-------------------------------------------- Set the position of the window to the top left corner.
		windowPosition.x = windowPosition.y = 0.0f;
		
		DX_LOG("[SCREEN] Fullscreen mode active", DX_LOG_EMPTY, LOG_MESSAGE);
	}
	else
	{
		//-------------------------------------------- If windowed then set window to resolution passed in to the function.
		m_width = width;
		m_height = height;

		//-------------------------------------------- Place the window in the middle of the screen.
<<<<<<< HEAD
		windowPosition.x = (GetSystemMetrics(SM_CXSCREEN) - m_width) / 2;
		windowPosition.y = (GetSystemMetrics(SM_CYSCREEN) - m_height) / 2;
=======
		windowPosition.x = (float)(GetSystemMetrics(SM_CXSCREEN) - m_width) / 2;
		windowPosition.y = (float)(GetSystemMetrics(SM_CYSCREEN) - m_height) / 2;
>>>>>>> master

		DX_LOG("[SCREEN] Fullscreen mode disabled", DX_LOG_EMPTY, LOG_MESSAGE);
	}

	//-------------------------------------------- Create the window with the screen settings and get the handle to it.
<<<<<<< HEAD
	m_window = CreateWindow(m_title, m_title, WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, windowPosition.x, windowPosition.y, m_width, m_height, nullptr, nullptr, m_instance, nullptr);
=======
	m_window = CreateWindow(m_title, m_title, WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, (int)windowPosition.x, (int)windowPosition.y, m_width, m_height, nullptr, nullptr, m_instance, nullptr);
>>>>>>> master
	//-------------------------------------------- Note: this will show unused in debug mode as 'unable to read memory'. unused is a 'dummy' variable and should be ignored

	//-------------------------------------------- Bring the window up on the screen and set it as main focus.
	ShowWindow(m_window, SW_SHOW);
	SetForegroundWindow(m_window);
	SetFocus(m_window);

	//-------------------------------------------- Hide the mouse cursor.
	//ShowCursor(false);

	return true;
}


/*******************************************************************************************************************
	WIN32 function that handles input from user and sends message back to system
*******************************************************************************************************************/
LRESULT ScreenManager::WindowProc(HWND window, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		//-------------------------------------------- Check if the window is being destroyed.
		case WM_DESTROY: { PostQuitMessage(0); break; }

		//-------------------------------------------- Check if the window is being closed.
		case WM_CLOSE: { PostQuitMessage(0); break; }

		//-------------------------------------------- Process all other messages.
		default: { return DefWindowProc(window, message, wParam, lParam); }
	}

	return 0;
}


/*******************************************************************************************************************
	Function that initializes a perspective view
*******************************************************************************************************************/
void ScreenManager::InitializePerspective()
{
	//---------------------------------------------------------------- Create a perspective projection matrix for 3D rendering
	m_perspectiveMatrix = XMMatrixPerspectiveFovLH(ScreenConstants::FieldOfView,
												   GetAspectRatio(),
												   ScreenConstants::NearView,
												   ScreenConstants::FarView);
}


/*******************************************************************************************************************
	Function that initializes an orthographic view
*******************************************************************************************************************/
void ScreenManager::InitializeOrthographic()
{
	//---------------------------------------------------------------- Create an orthographic projection matrix for 2D rendering
	m_orthographicMatrix = XMMatrixOrthographicLH((float)m_width,
												  (float)m_height,
												  ScreenConstants::NearView,
												  ScreenConstants::FarView);
}


/*******************************************************************************************************************
	Function that toggles between 2D and 3D projections - call this within the game states
*******************************************************************************************************************/
void ScreenManager::Enable3DView(bool viewIn3D)
{
	m_is3dEnabled = viewIn3D;

	(m_is3dEnabled)	? InitializePerspective()
					: InitializeOrthographic();
}


/*******************************************************************************************************************
	Accessor Methods
*******************************************************************************************************************/
HWND ScreenManager::GetWindow() const				{ return m_window; }
float ScreenManager::GetAspectRatio() const			{ return (float)m_width / (float)m_height; }
HINSTANCE ScreenManager::GetWindowInstance() const	{ return m_instance; }
<<<<<<< HEAD
unsigned int ScreenManager::GetWidth() const		{ return m_width; }
unsigned int ScreenManager::GetHeight()	const		{ return m_height; }
=======
int ScreenManager::GetWidth() const					{ return m_width; }
int ScreenManager::GetHeight()	const				{ return m_height; }
>>>>>>> master
bool ScreenManager::IsVSyncEnabled() const			{ return m_vSync; }
bool ScreenManager::IsFullScreen() const			{ return m_fullScreen; }
bool ScreenManager::Is3dEnabled() const				{ return m_is3dEnabled; }

XMMATRIX ScreenManager::GetPerspectiveMatrix()	{ return m_perspectiveMatrix; }
<<<<<<< HEAD
XMMATRIX ScreenManager::GetWorldMatrix()		{ return m_worldMatrix; }
=======
>>>>>>> master
XMMATRIX ScreenManager::GetOrthographicMatrix() { return m_orthographicMatrix; }