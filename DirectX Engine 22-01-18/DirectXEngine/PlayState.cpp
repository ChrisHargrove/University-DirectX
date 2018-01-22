#include "PlayState.h"
#include "Game.h"
#include "Log.h"

/*******************************************************************************************************************
	Constructor with initializer list to set default values of data members
*******************************************************************************************************************/
PlayState::PlayState(GameState* previousState) : GameState(previousState) 
{
	Initialize();
	
	Log("[PLAY STATE] PlayState constructor initialized", EMPTY, LOG_MESSAGE);
}


/*******************************************************************************************************************
	Cleanup all memory usage, delete all objects and shut down all devices
*******************************************************************************************************************/
PlayState::~PlayState() 
{
	if (terrainPlaneExample) { delete terrainPlaneExample; terrainPlaneExample = nullptr; }

	Log("[PLAY STATE] PlayState destructor initialized", EMPTY, LOG_MESSAGE);
}


/*******************************************************************************************************************
	Initialize all start up procedures specific to the play state
*******************************************************************************************************************/
bool PlayState::Initialize()
{
	IsActive() = IsAlive() = true;
	
	//Does work but needs some adjustments, have to rotate camera to see terrain. Needs some work!
	m_camera->InitializeOrthoView(800.0f, 600.0f, 0.1f, 1000.0f);
	m_camera->RotateHorizontal(1.0f);

	//Example terrain to demonstrate orthographic view
	terrainPlaneExample = new Terrain("Assets/textures/Cube.jpg");
	terrainPlaneExample->GeneratePlane(10, 10);
	terrainPlaneExample->FillBuffers(Terrain::TerrainType::PLANE);

	return true;
}


/*******************************************************************************************************************
	Function that updates everything within the play state
*******************************************************************************************************************/
bool PlayState::Update() {

	m_keys = Input::Instance()->GetKeyStates();

	IsWindowClosed();

	if (m_keys[SDL_SCANCODE_BACKSPACE]) {
		MainGame::Instance()->PermanentState(new MenuState(this));
		IsActive() = IsAlive() = false;
	}
	

	/*******************************************************************************************************************
	[ Example ] Camera movement with keyboard - Still Testing
	Last updated 05/01/2018

	*******************************************************************************************************************/
	if (m_keys[SDL_SCANCODE_UP])	{ m_camera->Move(m_camera->GetForward(), InputConstants::Speed); }
	if (m_keys[SDL_SCANCODE_DOWN])	{ m_camera->Move(m_camera->GetForward(), -InputConstants::Speed); }
	if (m_keys[SDL_SCANCODE_LEFT])	{ m_camera->Move(m_camera->GetLeft(), InputConstants::Speed); }
	if (m_keys[SDL_SCANCODE_RIGHT]) { m_camera->Move(m_camera->GetRight(), InputConstants::Speed); }


	/*******************************************************************************************************************
	[ Example ] Camera rotation with mouse - Still Testing
	Last updated 05/01/2018

	*******************************************************************************************************************/
	if (Input::Instance()->MouseButtonPressed(SDL_BUTTON_LEFT)) { m_camera->RotateVertical(-InputConstants::RotateSpeed); }
	if (Input::Instance()->MouseButtonPressed(SDL_BUTTON_RIGHT)) { m_camera->RotateVertical(InputConstants::RotateSpeed); }
	m_camera->RotateHorizontal(Input::Instance()->GetMouseWheel().y / 10.0f);

	return true;
}


/*******************************************************************************************************************
	Function that renders all play state graphics to the screen
*******************************************************************************************************************/
bool PlayState::Draw() {

	static float temp = 0.0f;
	temp = SDL_GetTicks() / 500.0f;
	float sinTemp = (float)(abs(sin(temp)));

	Screen::Instance()->SetBackgroundColor(glm::vec4(0.4f + sinTemp,0.3f + sinTemp, sinTemp, 1.0f));

	//Example terrain being drawn
	terrainPlaneExample->GetTransform()->SetPosition(Vector3f(400.0f, 300.0, 0.0f));
	terrainPlaneExample->GetTransform()->SetRotation(Quaternion().InitializeRotation(Vector3f(0, 1, 0), -temp * 180));
	terrainPlaneExample->RenderIndexed(m_camera);

	return true;
}


/*******************************************************************************************************************
	Function that checks if the player has closed the game window within the play state
*******************************************************************************************************************/
void PlayState::IsWindowClosed() {

	if (Input::Instance()->IsWindowClosed())	{ IsActive() = IsAlive() = false; }
	if (m_keys[SDL_SCANCODE_ESCAPE])			{ IsActive() = IsAlive() = false; }
}