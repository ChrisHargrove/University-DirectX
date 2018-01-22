#include "DXInput.h"

#include "Log.h"
#include "Memory.h"

/*******************************************************************************************************************
Input constructor to initialize default values
*******************************************************************************************************************/
DXInput::DXInput() :	m_interface(nullptr), 
						m_keyboard(nullptr),
						m_mouse(nullptr),
						m_mouseState({ 0 })
{
	Log("[INPUT] Input constructor initialized", EMPTY, LOG_MESSAGE);
}


/*******************************************************************************************************************
Accessor Methods
*******************************************************************************************************************/
bool DXInput::IsKeyPressed(unsigned short key) const { return m_keyState[key] & 0x80; }
bool DXInput::IsButtonPressed(unsigned char button) const { return m_mouseState.rgbButtons[button] & 0x80; }

long DXInput::GetMouseX() const { return m_mouseState.lX; }
long DXInput::GetMouseY() const { return m_mouseState.lY; }
long DXInput::GetMouseWheel() const { return m_mouseState.lZ; }


/*******************************************************************************************************************
Initialize the Input and run all startup functions
*******************************************************************************************************************/
void DXInput::Initialize(HINSTANCE hInstance, HWND window)
{
	//---------------------------------------------------------------- Create a direct input interface
	DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_interface, NULL);

	//---------------------------------------------------------------- Create and prepare the keyboard device
	m_interface->CreateDevice(GUID_SysKeyboard, &m_keyboard, NULL);
	m_keyboard->SetDataFormat(&c_dfDIKeyboard);
	m_keyboard->SetCooperativeLevel(window, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);

	//---------------------------------------------------------------- Create and prepare the mouse device
	m_interface->CreateDevice(GUID_SysMouse, &m_mouse, NULL);
	m_mouse->SetDataFormat(&c_dfDIMouse);
	m_mouse->SetCooperativeLevel(window, DISCL_NONEXCLUSIVE | DISCL_NOWINKEY | DISCL_FOREGROUND);
	m_mouse->Acquire();
}


/*******************************************************************************************************************
Runs every frame, detects any input from user
*******************************************************************************************************************/
void DXInput::DetectInput()
{
	//---------------------------------------------------------------- Aquire device every update (mandatory if user clicks away from game window)
	m_keyboard->Acquire();
	m_mouse->Acquire();

	m_keyboard->GetDeviceState(TOTAL_KEYS, (LPVOID)&m_keyState);
	m_mouse->GetDeviceState(sizeof(DIMOUSESTATE), &m_mouseState);
}


/*******************************************************************************************************************
User defined cleanup function that deletes all objects and shuts down the input safely
*******************************************************************************************************************/
void DXInput::Shutdown()
{
	if (m_keyboard) { m_keyboard->Unacquire(); }
	if (m_mouse)	{ m_mouse->Unacquire(); }
	SafeRelease(m_interface);

	Log("[INPUT] Input shut down successfully!", EMPTY, LOG_SUCCESS);
}


/*******************************************************************************************************************
Input destructor used for cleaning up memory, deleting all objects and calling shutdown functions
*******************************************************************************************************************/
DXInput::~DXInput()
{
	Shutdown();

	Log("[INPUT] Input destructor initialized", EMPTY, LOG_MESSAGE);
}