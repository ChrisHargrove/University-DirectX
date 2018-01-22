#ifndef PLAY_STATE_H_
#define PLAY_STATE_H_

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
	virtual bool Update() override;
	virtual bool Draw() override;
	
private:
	bool Initialize();
	void IsWindowClosed();

	//For demonstration of an orthographic projection only
	Terrain* terrainPlaneExample;
};

#endif