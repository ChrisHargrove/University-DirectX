#include "Game.h"
#include "Log.h"
#include "ScreenManager.h"
#include "GraphicsManager.h"
#include "InputManager.h"

/*******************************************************************************************************************
	Constructor with initializer list to set default values of data members
*******************************************************************************************************************/
Game::Game()	:	m_endGame(false),
					m_state(nullptr)

{
	DX_LOG("[GAME] Game constructor initialized", DX_LOG_EMPTY, LOG_MESSAGE);
}


/*******************************************************************************************************************
	Cleanup all memory usage, delete all objects and shut down all devices
*******************************************************************************************************************/
void Game::Shutdown()
{
	Input::Instance()->Shutdown();
	Graphics::Instance()->Shutdown();
	Screen::Instance()->Shutdown();

	DX_LOG("[GAME] Game shutdown successfully", DX_LOG_EMPTY, LOG_SUCCESS);
}


/*******************************************************************************************************************
	Initialize all start up procedures specific to the game
*******************************************************************************************************************/
bool Game::Initialize(HINSTANCE instance, LPCSTR title, bool fullScreen, bool vSync)
{
	//---------------------------------------------------------------- Intialize the window and screen settings
	Screen::Instance()->Initialize(instance, title,
								   ScreenConstants::SCREEN_WIDTH,
								   ScreenConstants::SCREEN_HEIGHT,
								   fullScreen, vSync);
	
	//---------------------------------------------------------------- Initialize the Direct X graphics api
	if (!Graphics::Instance()->Initialize()) { return false; }
	
	//---------------------------------------------------------------- Initialize the direct input
	Input::Instance()->Initialize();

	//---------------------------------------------------------------- Initialize a new menu state
	m_gameStates.push_front(new MenuState(nullptr));

	return true;
}


/*******************************************************************************************************************
	The main game loop, integrated with Windows
*******************************************************************************************************************/
int Game::Run() {

	//---------------------------------------------------------------- Initialize all our trackers - Delta time, FPS, CPU
	Tracker::Initialize();

	MSG message = { 0 };

	//---------------------------------------------------------------- Loop until there is a quit message from the window or the user, or until the game state deque is empty
	while (!m_endGame)
	{
		//---------------------------------------------------------------- Get the state at the front of the deque
		m_state = m_gameStates.front();

		//---------------------------------------------------------------- Loop as long as atleast one game state is active
		while (m_state->IsActive()) {

			//---------------------------------------------------------------- Update tracker data every frame
			Tracker::Update();

			//---------------------------------------------------------------- Handle the windows messages
			if (PeekMessage(&message, nullptr, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&message);
				DispatchMessage(&message);
			}

			//---------------------------------------------------------------- If windows signals to end the application then exit out, making sure to kill the current game state so the end game flag is set
			if (message.message == WM_QUIT) { m_state->IsActive() = m_state->IsAlive() = false; }
			else
			{
				//---------------------------------------------------------------- Otherwise update and render the current active game state
				m_state->Update(Tracker::GetTime());
				m_state->Draw();
			}
		}

		//---------------------------------------------------------------- If a game state is not alive, delete it from heap memory and remove it from the game states deque
		if (!m_state->IsAlive())	{ RemoveState(); }

		//---------------------------------------------------------------- If no game states are alive or active, set end game to true and exit out of game loop
		if (m_gameStates.empty())	{ m_endGame = true; }
	}

	//---------------------------------------------------------------- Shutdown the trackers
	Tracker::Shutdown();

	//---------------------------------------------------------------- Return this part of the WM_QUIT message to Windows
	return (int)message.wParam;
}


/*******************************************************************************************************************
	Function that adds a temporary game state to the front of the deque (e.g. Pause)
*******************************************************************************************************************/
void Game::TemporaryState(GameState* state) { m_gameStates.push_front(state); }


/*******************************************************************************************************************
	Function that adds a permanent game state to the back of the deque (e.g. Play)
*******************************************************************************************************************/
void Game::PermanentState(GameState* state) { m_gameStates.push_back(state); }


/*******************************************************************************************************************
	Function that deletes the game state at the front of the deque and removes the game state from the deque
*******************************************************************************************************************/
void Game::RemoveState() { delete m_gameStates.front(); m_gameStates.pop_front(); }

