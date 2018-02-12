#include "InputManager.h"
#include "ScreenManager.h"
#include "Log.h"

/*******************************************************************************************************************
	Constructor with initializer list to set default values of data members
*******************************************************************************************************************/
InputManager::InputManager()	:	m_interface(nullptr),
									m_keyboard(nullptr),
									m_mouse(nullptr),
									m_mouseState({ 0 }),
									m_mousePosition({ 0 })
{
	DX_LOG("[INPUT] Input constructor initialized", DX_LOG_EMPTY, LOG_MESSAGE);
}


/*******************************************************************************************************************
	Shut down all devices, destroy resources and clean up all memory
*******************************************************************************************************************/
void InputManager::Shutdown()
{
	if (m_mouse)		{ m_mouse->Unacquire(); m_mouse->Release(); m_mouse = nullptr; }
	if (m_keyboard)		{ m_keyboard->Unacquire(); m_keyboard->Release(); m_keyboard = nullptr; }
	if (m_interface)	{ m_interface->Release(); m_interface = nullptr; }

	DX_LOG("[INPUT] Input shutdown successfully", DX_LOG_EMPTY, LOG_SUCCESS);
}


/*******************************************************************************************************************
	Function that initializes the inputs default setup settings
*******************************************************************************************************************/
void InputManager::Initialize()
{
	//---------------------------------------------------------------- Create a direct input interface
	DirectInput8Create(Screen::Instance()->GetWindowInstance(), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_interface, nullptr);

	//---------------------------------------------------------------- Create and prepare the keyboard device
	m_interface->CreateDevice(GUID_SysKeyboard, &m_keyboard, nullptr);
	m_keyboard->SetDataFormat(&c_dfDIKeyboard);
	m_keyboard->SetCooperativeLevel(Screen::Instance()->GetWindow(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	m_keyboard->Acquire();

	//---------------------------------------------------------------- Create and prepare the mouse device
	m_interface->CreateDevice(GUID_SysMouse, &m_mouse, nullptr);
	m_mouse->SetDataFormat(&c_dfDIMouse);
	m_mouse->SetCooperativeLevel(Screen::Instance()->GetWindow(), DISCL_NONEXCLUSIVE | DISCL_NOWINKEY | DISCL_FOREGROUND);
	m_mouse->Acquire();

	DX_LOG("[INPUT] Input initialized successfully", DX_LOG_EMPTY, LOG_SUCCESS);
}


/*******************************************************************************************************************
	Function that checks for any input from the user every frame
*******************************************************************************************************************/
void InputManager::DetectInput()
{
	if (!ReadKeyboard())	{ DX_LOG("[INPUT] Problem reading keyboard", DX_LOG_EMPTY, LOG_ERROR); }
	if (!ReadMouse())		{ DX_LOG("[INPUT] Problem reading mouse", DX_LOG_EMPTY, LOG_ERROR); }
	
	UpdateMousePosition();
}


/*******************************************************************************************************************
	Function that reads the keyboard device and re-acquires keyboard if focus was lost
*******************************************************************************************************************/
bool InputManager::ReadKeyboard()
{
	HRESULT result = S_OK;

	//---------------------------------------------------------------- Read the keyboard device
	result = m_keyboard->GetDeviceState(sizeof(m_keyState), (LPVOID)&m_keyState);
	
	if (FAILED(result))
	{
		//---------------------------------------------------------------- If the keyboard lost focus or was not acquired then try to get control back
		if ((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED)) { m_keyboard->Acquire(); }
		else { return false; }
	}

	return true;
}


/*******************************************************************************************************************
	Function that reads the mouse device and re-acquires mouse if focus was lost
*******************************************************************************************************************/
bool InputManager::ReadMouse()
{
	HRESULT result = S_OK;

	//---------------------------------------------------------------- Read the mouse device
	result = m_mouse->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&m_mouseState);
	
	if (FAILED(result))
	{		
		//---------------------------------------------------------------- If the mouse lost focus or was not acquired then try to get control back
		if ((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED)) { m_mouse->Acquire(); }
		else { return false; }
	}

	return true;
}


/*******************************************************************************************************************
	Function that updates the mouse position every frame
*******************************************************************************************************************/
void InputManager::UpdateMousePosition()
{
	//---------------------------------------------------------------- Update the location of the mouse cursor based on the change of the mouse location during the frame
	m_mousePosition.x += m_mouseState.lX;
	m_mousePosition.y += m_mouseState.lY;

	//---------------------------------------------------------------- Ensure the mouse location doesn't exceed the screen width or height
	if (m_mousePosition.x > Screen::Instance()->GetWidth())		{ m_mousePosition.x = Screen::Instance()->GetWidth(); }
	if (m_mousePosition.y > Screen::Instance()->GetHeight())	{ m_mousePosition.y = Screen::Instance()->GetHeight(); }
	if (m_mousePosition.x < 0)									{ m_mousePosition.x = 0; }
	if (m_mousePosition.y < 0)									{ m_mousePosition.y = 0; }

}


/*******************************************************************************************************************
	Accessor Methods
*******************************************************************************************************************/
<<<<<<< HEAD
bool InputManager::IsKeyPressed(unsigned short key) const		{ return m_keyState[key] & 0x80; }
bool InputManager::IsButtonPressed(unsigned char button) const	{ return m_mouseState.rgbButtons[button] & 0x80; }
=======
int InputManager::IsKeyPressed(unsigned short key) const		{ return m_keyState[key] & 0x80; }
int InputManager::IsButtonPressed(unsigned char button) const	{ return m_mouseState.rgbButtons[button] & 0x80; }
>>>>>>> master

POINT InputManager::GetMousePosition() const	{ return m_mousePosition; }

long InputManager::GetMouseX() const			{ return m_mouseState.lX; }
long InputManager::GetMouseY() const			{ return m_mouseState.lY; }
long InputManager::GetMouseWheel() const		{ return m_mouseState.lZ; }

