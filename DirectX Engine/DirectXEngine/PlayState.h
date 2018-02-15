#pragma once

/*******************************************************************************************************************
	PlayState.h, PlayState.cpp
	Created by Kim Kane
	Last updated: 06/01/2018

	Initializes a play state within the game.

*******************************************************************************************************************/
#include "GameState.h"
#include "Terrain.h"

class PlayState : public GameState {

public:
	PlayState(GameState* previousState);
	virtual ~PlayState();

public:
	virtual void Update(float deltaTime) override;
	virtual void Draw() override;
	
private:
	bool Initialize();
	void IsWindowClosed();
};