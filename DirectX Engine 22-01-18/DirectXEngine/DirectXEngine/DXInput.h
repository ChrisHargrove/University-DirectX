#ifndef DX_INPUT_H_
#define DX_INPUT_H_

const int TOTAL_KEYS = 256;

class DXInput
{
public:
	DXInput();
	~DXInput();

	void Initialize();

	void KeyDown(unsigned int key);
	void KeyUp(unsigned int key);

	bool IsKeyDown(unsigned int key);

private:
	bool m_keys[TOTAL_KEYS];
};


#endif
