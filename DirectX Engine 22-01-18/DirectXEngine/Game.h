#pragma once

/*******************************************************************************************************************
	Game.h, Game.cpp
	Created by Kim Kane
	Last updated: 27/01/2018

	A singleton class that sets up and initializes the main game.

*******************************************************************************************************************/

#include <deque>

#include "Singleton.h"
#include "Tracker.h"
#include "GameState.h"
#include "MenuState.h"
#include "PlayState.h"

/* NOTE: Any new game state .h files must be included here */

class Game {

public:
	friend class Singleton<Game>;

public:
	bool Initialize(HINSTANCE instance, LPCSTR title, bool fullScreen, bool vSync);
	void Shutdown();
<<<<<<< HEAD
	bool Run();
=======
	int Run();
>>>>>>> master

public:
	void TemporaryState(GameState* state);
	void PermanentState(GameState* state);

private:
	Game();
	Game(const Game&);
	Game& operator=(const Game&) {}

private:
	void RemoveState();

private:
	bool		m_endGame;
	GameState*	m_state;

private:
	std::deque<GameState*> m_gameStates;
};

typedef Singleton<Game> MainGame;