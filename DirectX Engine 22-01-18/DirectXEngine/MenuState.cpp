#include "MenuState.h"
#include "Game.h"
#include "ScreenManager.h"
#include "GraphicsManager.h"
#include "InputManager.h"
#include "Log.h"

/*******************************************************************************************************************
	Constructor with initializer list to set default values of data members
*******************************************************************************************************************/
MenuState::MenuState(GameState* previousState)	:	GameState(previousState)
{

	DX_LOG("[MENU STATE] MenuState constructor initialized", DX_LOG_EMPTY, LOG_MESSAGE);
	
	if (!Initialize()) { DX_LOG("[MENU STATE] Problem initializing menu state", DX_LOG_EMPTY, LOG_ERROR); }
}


/*******************************************************************************************************************
	Cleanup all memory usage, delete all objects and shut down all devices
*******************************************************************************************************************/
MenuState::~MenuState() {
	
	if (m_terrain) { delete m_terrain; m_terrain = nullptr; }

	DX_LOG("[MENU STATE] MenuState destructor initialized", DX_LOG_EMPTY, LOG_MESSAGE);
}


/*******************************************************************************************************************
	Initialize all start up procedures specific to the menu state
*******************************************************************************************************************/
bool MenuState::Initialize() {

	//---------------------------------------------------------------- Set this game state to current active game state, & it is alive (running)
	IsActive() = IsAlive() = true;

	//---------------------------------------------------------------- Set the projection to 3D
	Screen::Instance()->Enable3DView(true);

	/////////////////////////////////////////////////////////
	//  BEGIN CREATION & INITALIZATION OF OBJECTS TESTING
	/////////////////////////////////////////////////////////
	m_terrain = new Terrain();
	if (!m_terrain->Initialize("Assets\\Terrain\\heightMap.bmp")) { return false; }




	if (!m_laraModel.Load("Assets\\Objects\\Lara.obj")) { return false; };
	if (!m_cubeModel.Load("Assets\\Objects\\Cube.obj")) { return false; };

	if (!m_laraTexture.LoadTexture("Assets\\Textures\\Lara.png")) { return false; };
	if (!m_cubeTexture.LoadTexture("Assets\\Textures\\Cube.jpg")) { return false; };

	m_laraObject = new GameObject(XMFLOAT3(50.0, 10.5, 20.0), &m_laraModel, &m_laraTexture);
	m_cubeObject = new GameObject(XMFLOAT3(10.0, 1.5, 20.0), &m_cubeModel, &m_cubeTexture);


	/////////////////////////////////////////////////////////
	//  END OF CREATION & INITALIZATION OF OBJECTS TESTING
	/////////////////////////////////////////////////////////

	return true;
}


/*******************************************************************************************************************
	Function that updates everything within the menu state
*******************************************************************************************************************/
void MenuState::Update(float deltaTime) {
	
	//---------------------------------------------------------------- Detect user input every frame
	Input::Instance()->DetectInput();
	IsWindowClosed();

	//---------------------------------------------------------------- If return pressed, delete this state and create a new play state
	if (Input::Instance()->IsKeyPressed(DIK_RETURN)) {
		MainGame::Instance()->PermanentState(new PlayState(this));
		IsActive() = IsAlive() = false;
	}

	/////////////////////////////////////////////////////////
	//  BEGIN INPUT TESTING
	/////////////////////////////////////////////////////////

	//---------------------------------------------------------------- Keyboard Testing
	if (Input::Instance()->IsKeyPressed(DIK_LEFT))	{ m_camera->Move(m_camera->GetRight(),   -InputConstants::Speed * deltaTime); }
	if (Input::Instance()->IsKeyPressed(DIK_RIGHT)) { m_camera->Move(m_camera->GetRight(),	  InputConstants::Speed * deltaTime); }
	if (Input::Instance()->IsKeyPressed(DIK_DOWN))	{ m_camera->Move(m_camera->GetForward(), -InputConstants::Speed * deltaTime); }
	if (Input::Instance()->IsKeyPressed(DIK_UP))	{ m_camera->Move(m_camera->GetForward(),  InputConstants::Speed * deltaTime); }

	//---------------------------------------------------------------- Mouse Testing
	if (Input::Instance()->IsButtonPressed(InputConstants::LEFT))	{ m_camera->Rotate(0.0f, -InputConstants::RotateSpeed * deltaTime, 0.0f); }
	if (Input::Instance()->IsButtonPressed(InputConstants::RIGHT))	{ m_camera->Rotate(0.0f,  InputConstants::RotateSpeed * deltaTime, 0.0f); }
	m_camera->Rotate(Input::Instance()->GetMouseWheel() / 1000.0f * deltaTime, 0.0f, 0.0f);
	



	float move = 0.01f * deltaTime;
	//TESTING--DONT TOUCH!...................Seems to work :)

	//X AXIS
	if (Input::Instance()->IsKeyPressed(DIK_A)) { m_laraObject->TranslateX(-move); } //LEFT
	if (Input::Instance()->IsKeyPressed(DIK_D)) { m_laraObject->TranslateX(move); } //RIGHT

	//Z AXIS
	if (Input::Instance()->IsKeyPressed(DIK_W)) { m_laraObject->TranslateZ(move); } //FORWARD
	if (Input::Instance()->IsKeyPressed(DIK_S)) { m_laraObject->TranslateZ(-move); } //BACK
	
	//Y AXIS
	if (Input::Instance()->IsKeyPressed(DIK_T)) { m_laraObject->TranslateY(move); } //UP
	if (Input::Instance()->IsKeyPressed(DIK_B)) { m_laraObject->TranslateY(-move); } //DOWN





	/////////////////////////////////////////////////////////
	//  END OF INPUT TESTING
	/////////////////////////////////////////////////////////

	//std::cout << "FPS: " << Tracker::GetFps() << std::endl;
	//std::cout << "TIME: " << Tracker::GetTime() << std::endl;
	//std::cout << "CPU: " << Tracker::GetCpuPercentage() << std::endl;
}


/*******************************************************************************************************************
	Function that renders all menu state graphics to the screen
*******************************************************************************************************************/
void MenuState::Draw() {
	
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
	
	m_terrain->Render(m_camera);


	m_laraObject->Render(m_camera);
	m_cubeObject->Render(m_camera);


	////////////////////////////////////////////////
	// END OF 3D RENDERING
	////////////////////////////////////////////////

	//---------------------------------------------------------------- Present the rendered scene to the screen
	Graphics::Instance()->EndScene();

}


/*******************************************************************************************************************
	Function that checks if ESC key has been pressed during gameplay & if so, kills the current state
*******************************************************************************************************************/
void MenuState::IsWindowClosed() {

	if (Input::Instance()->IsKeyPressed(DIK_ESCAPE)) { IsActive() = IsAlive() = false; }
}