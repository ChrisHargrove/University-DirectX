#ifndef DX_INPUT_H_
#define DX_INPUT_H_

#define DIRECTINPUT_VERSION 0x0800

#include <dinput.h>

const unsigned int TOTAL_KEYS = 256;
const unsigned int TOTAL_BUTTONS = 4;

enum ButtonType { LEFT = 0, RIGHT, MIDDLE};

class DXInput {

public:
	DXInput();
	~DXInput();

	void Initialize(HINSTANCE hInstance, HWND window);
	void DetectInput();
	
	bool IsKeyPressed(unsigned short key) const;
	bool IsButtonPressed(unsigned char button) const;

	long GetMouseX() const;
	long GetMouseY() const;
	long GetMouseWheel() const;

	void Shutdown();

private:
	LPDIRECTINPUT8 m_interface;
	LPDIRECTINPUTDEVICE8 m_keyboard;
	LPDIRECTINPUTDEVICE8 m_mouse;

	DIMOUSESTATE m_mouseState;
	unsigned char m_keyState[TOTAL_KEYS];
};


#endif