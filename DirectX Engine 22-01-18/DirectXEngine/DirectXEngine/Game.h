#ifndef GAME_H_
#define GAME_H_

#include "DXApplication.h"

using namespace DirectX;

class Game : public DXApplication
{
public:
	Game(HINSTANCE hInstance, const char* title, int clientWidth, int clientHeight);
	~Game();

	//The override keyword throws error if it's not actually an overwritable method
	bool Initialize() override;
	bool Update(float dt) override;
	bool Render(float dt) override;


protected:


private:



};

#endif