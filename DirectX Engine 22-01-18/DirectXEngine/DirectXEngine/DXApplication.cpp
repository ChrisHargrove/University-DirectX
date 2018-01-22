#include "DXApplication.h"
#include "Log.h"


namespace
{
	DXApplication* g_appPointer = nullptr;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (g_appPointer) { return g_appPointer->MsgProc(hwnd, msg, wParam, lParam); }
	else { return DefWindowProc(hwnd, msg, wParam, lParam); }
}


DXApplication::DXApplication(HINSTANCE hInstance) :
m_appInstance(hInstance),
m_appWindow(nullptr),
m_clientWidth(GetSystemMetrics(SM_CXSCREEN)),
m_clientHeight(GetSystemMetrics(SM_CYSCREEN)),
m_windowStyle(WINDOWED),
m_appTitle("DirectX Application Title"),
m_windowClass("DXApplication"),
m_fullScreen(false),
m_dxInput(nullptr),
m_dxGraphics(nullptr)
{
	g_appPointer = this;
}

DXApplication::~DXApplication()
{
	Shutdown();
	std::cout << "dxapp destructor called";
}

int DXApplication::Run()
{
	// this struct holds Windows event messages
	MSG msg = { 0 };
	
	bool endGame = false;
	bool handleEvents;

	while (!endGame) {
		//Dispatches incoming sent messages, checks the thread message queue for a posted message, and retrieves the message(if any exist)
		if (PeekMessage(&msg, nullptr, NULL, NULL, PM_REMOVE)) {

			// translate keystroke messages into the right format
			TranslateMessage(&msg);

			// send the message to the WindowProc function
			DispatchMessage(&msg);
		}
		if (msg.message == WM_QUIT)
		{
			endGame = true;
		}
		else {
			
			handleEvents = Update(0.0f);
			Render(0.0f);
			if (!handleEvents) { endGame = true; }
		}
	}

	// return this part of the WM_QUIT message to Windows
	return (int)msg.wParam;
}

bool DXApplication::Initialize()
{
	HRESULT result;

	InitializeWindow();
	// Create the input object.  This object will be used to handle reading the keyboard input from the user.
	m_dxInput = new DXInput;
	if (!m_dxInput)	{ return false; }

	// Initialize the input object.
	m_dxInput->Initialize();

	// Create the graphics object.  This object will handle rendering all the graphics for this application.
	m_dxGraphics = new DXGraphics;
	if (!m_dxGraphics) { return false; }

	// Initialize the graphics object.
	result = m_dxGraphics->Initialize(m_appWindow, m_clientWidth, m_clientHeight, false, m_fullScreen);
	if (!result) { return false; }

	return true;
}



void DXApplication::Shutdown()
{
	// Release the graphics object.
	if (m_dxGraphics)
	{
		m_dxGraphics->Shutdown();
		delete m_dxGraphics;
		m_dxGraphics = nullptr;
	}

	// Release the input object.
	if (m_dxInput)
	{
		delete m_dxInput;
		m_dxInput = nullptr;
	}

	// Shutdown the window.
	ShutdownWindow();
}


HRESULT DXApplication::InitializeWindow()
{
	//to stop distortions we get the client size first before determining window size
	RECT windowRect = { 0, 0, (LONG)m_clientWidth, (LONG)m_clientHeight };    // set the size, but not the position
	AdjustWindowRect(&windowRect, WINDOWED, FALSE);    // adjust the size

													   // this struct holds information for the window class
	WNDCLASSEX wcex;
	DEVMODE dmScreenSettings;
	UINT windowPosition_x, windowPosition_y;

	// clear out the window class for use
	ZeroMemory(&wcex, sizeof(WNDCLASSEX));

	// fill in the struct with the needed information
	wcex.cbSize = sizeof(WNDCLASSEX); //what it's measurements are
	wcex.style = CS_HREDRAW | CS_VREDRAW; //tell Windows to redraw the window if it is moved vertically or horizontally
	wcex.lpfnWndProc = WndProc; //tells the window class what function to use when it gets a message from Windows
	wcex.cbClsExtra = 0; //the number of extra bytes to allocate following the window-class structure.
	wcex.cbWndExtra = 0; //The number of extra bytes to allocate following the window instance. 
	wcex.hInstance = m_appInstance; //It is a handle to a copy of our application.
	wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION); // A handle to the class icon. MAKEINTRESOURCE use for custom icons
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW); //stores the default mouse cursor for the window class
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1); //contains the 'brush' that will be used to color the background of our window
	wcex.lpszMenuName = nullptr; //for menus
	wcex.lpszClassName = m_windowClass; //This is the name of the window class we are building
	wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION); //A handle to a small icon that is associated with the window class.

													// register the window class
	if (!RegisterClassEx(&wcex)) { Log("[DXAPP] Failed to create window class", EMPTY, LOG_ERROR); return E_FAIL; }

	// Setup the screen settings depending on whether it is running in full screen or in windowed mode.
	if (m_fullScreen)
	{
		std::cout << "fullscreen enabled" << std::endl;
		// If full screen set the screen to maximum size of the users desktop and 32bit.
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = (unsigned long)m_clientWidth;
		dmScreenSettings.dmPelsHeight = (unsigned long)m_clientHeight;
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// Change the display settings to full screen.
		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);
		
		m_windowStyle = FULLSCREEN;
		
		// Set the position of the window to the top left corner.
		windowPosition_x = windowPosition_y = 0;
	}
	else
	{
		std::cout << "fullscreen disabled" << std::endl;
		// If windowed then set it to 800x600 resolution.
		m_clientWidth = 800;
		m_clientHeight = 600;

		// Place the window in the middle of the screen.
		windowPosition_x = (GetSystemMetrics(SM_CXSCREEN) - windowRect.right - windowRect.left) / 2;
		windowPosition_y = (GetSystemMetrics(SM_CYSCREEN) - windowRect.bottom - windowRect.top) / 2;
	}


	// create the window and use the result as the handle
	//win class name, window title, style of the window, x, y, width, height, parent window(parent window is a window that contains other windows e.g. microsoft word), handle to menu bar, creating multiple windows
	m_appWindow = CreateWindow(m_windowClass, m_appTitle, m_windowStyle, windowPosition_x, windowPosition_y, windowRect.right - windowRect.left, windowRect.bottom - windowRect.top, nullptr, nullptr, m_appInstance, nullptr);

	if (!m_appWindow) { Log("[DXAPP] Failed to create window", EMPTY, LOG_ERROR); return E_FAIL; }

	// display the window on the screen
	ShowWindow(m_appWindow, SW_SHOW);

	//Brings the thread that created the specified window into the foreground and activates the window.
	SetForegroundWindow(m_appWindow);

	//Sets the keyboard focus to the specified window
	SetFocus(m_appWindow);

	Log("[DXAPP] Window created successfully!", EMPTY, LOG_SUCCESS);

	return S_OK;
}


void DXApplication::ShutdownWindow()
{
	std::cout << "shutting down window" << std::endl;
	// Fix the display settings if leaving full screen mode.
	if (m_fullScreen)
	{
		ChangeDisplaySettings(NULL, 0);
	}

	// Remove the window.
	DestroyWindow(m_appWindow);
	m_appWindow = nullptr;

	// Remove the application instance.
	UnregisterClass(m_windowClass, m_appInstance);
	m_appInstance = nullptr;
}



LRESULT DXApplication::MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;

	// sort through and find what code to run for the message given
	switch (msg) {

	case WM_PAINT:
		//prepares the specified window for painting and fills a PAINTSTRUCT structure with information about the painting
		hdc = BeginPaint(hwnd, &ps);
		//marks the end of painting in the specified window. 
		EndPaint(hwnd, &ps);
		return 0;

	case WM_DESTROY:
		// close the application entirely
		PostQuitMessage(0);
		return 0;

	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;

		// Check if a key has been pressed on the keyboard.
	case WM_KEYDOWN:
	{
		// If a key is pressed send it to the input object so it can record that state.
		m_dxInput->KeyDown((unsigned int)wParam);
		return 0;
	}

	// Check if a key has been released on the keyboard.
	case WM_KEYUP:
	{
		// If a key is released then send it to the input object so it can unset the state for that key.
		m_dxInput->KeyUp((unsigned int)wParam);
		return 0;
	}

	default:
		// Handle any messages the switch statement didn't
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
}