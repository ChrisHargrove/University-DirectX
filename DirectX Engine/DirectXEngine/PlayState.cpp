#include "PlayState.h"
#include "GameManager.h"
#include "Log.h"
#include "ScreenManager.h"
#include "GraphicsManager.h"
#include "InputManager.h"

/*******************************************************************************************************************
	Constructor with initializer list to set default values of data members
*******************************************************************************************************************/
PlayState::PlayState(GameState* previousState) : GameState(previousState) 
{
	DX_LOG("[PLAY STATE] PlayState constructor initialized", DX_LOG_EMPTY, LOG_MESSAGE);
	
	if (!Initialize()) { DX_LOG("[PLAY STATE] Problem initializing play state", DX_LOG_EMPTY, LOG_ERROR); }
}


/*******************************************************************************************************************
	Cleanup all memory usage, delete all objects and shut down all devices
*******************************************************************************************************************/
PlayState::~PlayState() 
{
	DX_LOG("[PLAY STATE] PlayState destructor initialized", DX_LOG_EMPTY, LOG_MESSAGE);
}


/*******************************************************************************************************************
	Initialize all start up procedures specific to the play state
*******************************************************************************************************************/
bool PlayState::Initialize()
{
	//---------------------------------------------------------------- Set this game state to current active game state, & it is alive (running)
	IsActive() = IsAlive() = true;

	//---------------------------------------------------------------- Set the projection to 3D
	Screen::Instance()->Enable3DView(true);

	/////////////////////////////////////////////////////////
	//  BEGIN CREATION & INITALIZATION OF OBJECTS TESTING
	/////////////////////////////////////////////////////////




	/////////////////////////////////////////////////////////
	//  END OF CREATION & INITALIZATION OF OBJECTS TESTING
	/////////////////////////////////////////////////////////

	return true;
}


/*******************************************************************************************************************
	Function that updates everything within the play state
*******************************************************************************************************************/
void PlayState::Update(float deltaTime) {

	//---------------------------------------------------------------- Detect user input every frame
	Input::Instance()->DetectInput();
	IsWindowClosed();

	//---------------------------------------------------------------- If space pressed, delete this state and create a new menu state
	if (Input::Instance()->IsKeyPressed(DIK_SPACE)) {
		Game::Instance()->PermanentState(new MenuState(this));
		IsActive() = IsAlive() = false;
	}
}


/*******************************************************************************************************************
	Function that renders all play state graphics to the screen
*******************************************************************************************************************/
void PlayState::Draw() {

	//---------------------------------------------------------------- Clear the screen
	Graphics::Instance()->BeginScene(0.2f, 0.2f, 0.4f, 1.0f);

	Graphics::Instance()->EnableDepthBuffer(false);
	Graphics::Instance()->EnableAlphaBlending(true);

	////////////////////////////////////////////////
	//  BEGIN 2D RENDERING
	////////////////////////////////////////////////





	////////////////////////////////////////////////
	// END OF 2D RENDERING
	////////////////////////////////////////////////

	Graphics::Instance()->EnableDepthBuffer(true);
	Graphics::Instance()->EnableAlphaBlending(false);

	////////////////////////////////////////////////
	//  BEGIN 3D RENDERING
	////////////////////////////////////////////////





	////////////////////////////////////////////////
	// END OF 3D RENDERING
	////////////////////////////////////////////////

	//---------------------------------------------------------------- Present the rendered scene to the screen
	Graphics::Instance()->EndScene();
}


/*******************************************************************************************************************
	Function that checks if ESC key has been pressed during gameplay & if so, kills the current state
*******************************************************************************************************************/
void PlayState::IsWindowClosed() {
	
	if (Input::Instance()->IsKeyPressed(DIK_ESCAPE)) { IsActive() = IsAlive() = false; }
}