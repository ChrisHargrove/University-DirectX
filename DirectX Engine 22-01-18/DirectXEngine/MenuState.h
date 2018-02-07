#pragma once

/*******************************************************************************************************************
	MenuState.h, MenuState.cpp
	Created by Kim Kane
	Last updated: 06/01/2018

	Initializes a menu state within the game.

*******************************************************************************************************************/
#include "GameState.h"

#include "Terrain.h"

#include "GameObject.h"
#include "Model.h"
#include "Texture.h"

class MenuState : public GameState {

public:
	MenuState(GameState* previousState);
	virtual ~MenuState();

public:
	virtual void Update(float deltaTime) override;
	virtual void Draw() override;
	
private:
	bool Initialize();
	void IsWindowClosed();

private:
	Terrain* m_terrain;

	Model m_laraModel;
	Model m_cubeModel;

	Texture m_laraTexture;
	Texture m_cubeTexture;

	GameObject* m_laraObject;
	GameObject* m_cubeObject;
};