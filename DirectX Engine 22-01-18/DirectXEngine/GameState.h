#ifndef GAME_STATE_H_
#define GAME_STATE_H_

/*******************************************************************************************************************
	GameState.h, GameState.cpp
	Created by Kim Kane
	Last updated: 06/01/2018

	Base class for all our game states.

*******************************************************************************************************************/
#include <SDL.h>
#include "Camera.h"

class GameState {

public:
	GameState(GameState* previousState);
	virtual ~GameState() = 0;
	GameState(const GameState& other);

public:
	virtual bool Update()	= 0;
	virtual bool Draw()		= 0;

public:
	bool& IsActive();
	bool& IsAlive();

protected:
	const Uint8*	m_keys;
	GameState*		m_previousState;

	//---------------------------------------------------------------- Decided to create the camera here, as I want it to be more of a component than a game object. 
	//---------------------------------------------------------------- Now the game states don't have to create a new camera every time they are created
	//---------------------------------------------------------------- but rather they just specify what kind of projection the camera should have. Eventually I will have a list of game components as well as game objects
	Camera*			m_camera;

private:
	bool			m_isActive;
	bool			m_isAlive;
};

#endif