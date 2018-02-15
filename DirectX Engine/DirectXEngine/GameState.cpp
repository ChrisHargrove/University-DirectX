#include "GameState.h"
#include "Log.h"

/*******************************************************************************************************************
	Constructor with initializer list to set default values of data members
*******************************************************************************************************************/
GameState::GameState(GameState* previousState)	:	m_previousState(previousState),
													m_isActive(true),
													m_isAlive(true),
													m_camera(new Camera(50.0f, 20.0f, -7.0f))
{
	DX_LOG("[GAME STATE] GameState constructor initialized", DX_LOG_EMPTY, LOG_MESSAGE);
}


/*******************************************************************************************************************
	Cleanup all memory usage, delete all objects and shut down all devices
*******************************************************************************************************************/
GameState::~GameState()
{
	if (m_camera) { delete m_camera; m_camera = nullptr; }

	DX_LOG("[GAME STATE] GameState destructor initialized", DX_LOG_EMPTY, LOG_MESSAGE);
}


/*******************************************************************************************************************
	Defined copy constructor, so we can keep track of our copied objects (for optimization purposes only)
*******************************************************************************************************************/
GameState::GameState(const GameState & other)	:	m_previousState(other.m_previousState),
													m_isActive(other.m_isActive),
													m_isAlive(other.m_isAlive)
{
	DX_LOG("[GAME STATE] Game State object copied!", DX_LOG_EMPTY, LOG_COPY);
}


/*******************************************************************************************************************
	Accessor Methods
*******************************************************************************************************************/
bool& GameState::IsActive() { return m_isActive; }
bool& GameState::IsAlive()	{ return m_isAlive; }