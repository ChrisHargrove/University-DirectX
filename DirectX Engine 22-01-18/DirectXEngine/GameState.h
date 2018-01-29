#pragma once

/*******************************************************************************************************************
	GameState.h, GameState.cpp
	Created by Kim Kane
	Last updated: 06/01/2018

	Base class for all our game states.

*******************************************************************************************************************/
#include "Camera.h"

class GameState {

public:
	GameState(GameState* previousState);
	virtual ~GameState() = 0;
	GameState(const GameState& other);

public:
	virtual void Update(float deltaTime)	= 0;
	virtual void Draw()						= 0;

public:
	bool& IsActive();
	bool& IsAlive();

protected:
	GameState*		m_previousState;

	Camera*			m_camera;

private:
	bool			m_isActive;
	bool			m_isAlive;
};