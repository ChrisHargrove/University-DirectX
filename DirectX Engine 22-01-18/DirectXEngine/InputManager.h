#pragma once

/*******************************************************************************************************************
	InputManager.h, InputManager.cpp
	Create by Kim Kane
	Last updated: 27/01/2018

	A singleton class that handles all user input using direct input.

*******************************************************************************************************************/
#define DIRECTINPUT_VERSION 0x0800

#include <dinput.h>
#include "Singleton.h"
#include "Constants.h"

class InputManager {

public:
	friend class Singleton<InputManager>;

public:
	void Initialize();
	void DetectInput();
	void Shutdown();

public:
	bool IsKeyPressed(unsigned short key) const;
	bool IsButtonPressed(unsigned char button) const;

public:
	POINT GetMousePosition() const;

	long GetMouseX() const;
	long GetMouseY() const;
	long GetMouseWheel() const;

private:
	InputManager();
	InputManager(const InputManager&);
	InputManager& operator=(const InputManager&) {}

private:
	bool ReadKeyboard();
	bool ReadMouse();
	void UpdateMousePosition();

private:
	LPDIRECTINPUT8			m_interface;
	LPDIRECTINPUTDEVICE8	m_keyboard;
	LPDIRECTINPUTDEVICE8	m_mouse;

	DIMOUSESTATE			m_mouseState;
	unsigned char			m_keyState[InputConstants::TOTAL_KEYS];

	POINT					m_mousePosition;
};

typedef Singleton<InputManager> Input;