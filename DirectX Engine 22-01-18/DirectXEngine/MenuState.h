#ifndef MENU_STATE_H_
#define MENU_STATE_H_

/*******************************************************************************************************************
	MenuState.h, MenuState.cpp
	Created by Kim Kane
	Last updated: 06/01/2018

	Initializes a menu state within the game.
	Objects are created here just for demonstration.

*******************************************************************************************************************/
#include "GameState.h"

#include "Mesh.h"
#include "GameObject.h"

#include "Terrain.h"

class MenuState : public GameState {

public:
	MenuState(GameState* previousState);
	virtual ~MenuState();

public:
	virtual bool Update() override;
	virtual bool Draw() override;
	
private:
	//---------------------------------------------------------------- Testing functions for demonstration only
	void InputTesting();

private:
	bool Initialize();
	void IsWindowClosed();

	//---------------------------------------------------------------- Example mesh objects
	Mesh meshLaraExample;
	Mesh meshCubeExample;
	Mesh meshFontExample;
	Mesh meshSphereExample;
	Mesh meshPyramidExample;

	//---------------------------------------------------------------- Example Game Objects
	GameObject* gameObjectLaraExample;
	GameObject* gameObjectCubeExample;
	GameObject* gameObjectFontExample;
	GameObject* gameObjectSphereExample;
	GameObject* gameObjectPyramidExample;

	GameObject* rootObjectExample;

	//---------------------------------------------------------------- Example terrain
	Terrain* terrainPlaneExample;
	Terrain* terrainGridExample;
};

#endif