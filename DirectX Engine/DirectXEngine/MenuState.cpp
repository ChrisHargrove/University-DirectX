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
    delete _Text;
    delete _FontTexture;

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

	m_terrain = new Terrain();
	if (!m_terrain->Initialize("Assets\\Terrain\\heightMap.bmp")) { return false; }

	if (!m_laraModel.Load("Assets\\Objects\\Lara.obj")) { return false; };
	if (!m_SphereModel.Load("Assets\\Objects\\Sphere.obj")) { return false; };

	if (!m_laraTexture.LoadTexture("Lara.png")) { return false; };
	if (!m_sphereTexture.LoadTexture("Sphere.jpg")) { return false; };

	m_laraObject = new Actor(XMFLOAT3(50.0, 10.5, 20.0), &m_laraModel, &m_laraTexture);
    m_laraObject->RotateY(XM_PIDIV2);

    for (int i = 0; i < 20; i++) {
        m_Sphere[i] = new GameObject(XMFLOAT3(100.0f / (1.5f * i), 100.0f / (1.5f * i), 100.0f / (1.5f * i)), &m_SphereModel, &m_sphereTexture);
    }

    _FontTexture = new Texture();
    _FontTexture->LoadTexture("Fonts\\oriental.png");
    _Text = new Text(_FontTexture, nullptr);
    _CullFrustum = new Frustum();

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

    m_camera->SetPosition(m_laraObject->GetPositionF().x, m_laraObject->GetPositionF().y + 5, m_laraObject->GetPositionF().z - 12);
    m_camera->SetRotation(15.0f, 0.0f, 0.0f);

    m_laraObject->SetMass(10.0f);

    if (Input::Instance()->IsKeyPressed(DIK_A)) { m_laraObject->ApplyForce(XMVectorSet(-0.25f, 0, 0, 0)); } //LEFT
    if (Input::Instance()->IsKeyPressed(DIK_D)) { m_laraObject->ApplyForce(XMVectorSet(0.25f, 0, 0, 0)); } //RIGHT
    if (Input::Instance()->IsKeyPressed(DIK_W)) { m_laraObject->ApplyForce(XMVectorSet(0, 0, 0.25f, 0)); } //FORWARD
    if (Input::Instance()->IsKeyPressed(DIK_S)) { m_laraObject->ApplyForce(XMVectorSet(0, 0, -0.25f, 0)); } //BACK
    if (Input::Instance()->IsKeyPressed(DIK_SPACE)) { m_laraObject->ApplyForce(XMVectorSet(0, 0.8f, 0, 0)); } //UP
    if (Input::Instance()->IsKeyPressed(DIK_LCONTROL)) { m_laraObject->ApplyForce(XMVectorSet(0, -0.8f, 0, 0)); } //DOWN

    m_laraObject->Update();
    m_laraObject->ApplyFriction(0.8f);

    _CullFrustum->Create(m_camera->GetViewMatrix());
}


/*******************************************************************************************************************
	Function that renders all menu state graphics to the screen
*******************************************************************************************************************/
void MenuState::Draw() {
	
	//---------------------------------------------------------------- Clear the screen
	Graphics::Instance()->BeginScene(0.2f, 0.2f, 0.4f, 1.0f);
	Graphics::Instance()->EnableDepthBuffer(true);
	Graphics::Instance()->EnableAlphaBlending(false);

	////////////////////////////////////////////////
	//  BEGIN 3D RENDERING
	////////////////////////////////////////////////
	
	m_terrain->Render(m_camera);

	m_laraObject->Render(m_camera);

    int renderCount = 0;
    for (auto s : m_Sphere) {
        XMFLOAT3 temp = s->GetPositionF();
        if (_CullFrustum->CheckSphere(temp.x, temp.y, temp.z, 1)) {
            s->Render(m_camera);
            renderCount++;
        }
    }

    ////////////////////////////////////////////////
    //  BEGIN 2D RENDERING
    ////////////////////////////////////////////////
    Graphics::Instance()->EnableDepthBuffer(false);
    Graphics::Instance()->EnableAlphaBlending(true);

    _Text->DrawString("FPS: " + std::to_string(Tracker::GetFps()), -0.9f, 0.83f, XMFLOAT3(1.0f, 0.0f, 0.0f));
    _Text->DrawString("Frame Time: " + std::to_string(Tracker::GetTime()), -0.9f, 0.75f);
    _Text->DrawString("CPU%: " + std::to_string(Tracker::GetCpuPercentage()), -0.9f, 0.67f);
    _Text->DrawString("Render Count: " + std::to_string(renderCount), -0.9f, 0.59f);

    _Text->DrawString("VelocityX: " + std::to_string(XMVectorGetX(m_laraObject->GetVelocity())), -0.9f, 0.51f, XMFLOAT3(0.0f, 0.0f, 1.0f));
    _Text->DrawString("AccelX: " + std::to_string(XMVectorGetX(m_laraObject->GetAcceleration())), -0.9f, 0.43f, XMFLOAT3(1.0f, 0.0f, 1.0f));

	
	//---------------------------------------------------------------- Present the rendered scene to the screen
	Graphics::Instance()->EndScene();

}


/*******************************************************************************************************************
	Function that checks if ESC key has been pressed during gameplay & if so, kills the current state
*******************************************************************************************************************/
void MenuState::IsWindowClosed() {

	if (Input::Instance()->IsKeyPressed(DIK_ESCAPE)) { IsActive() = IsAlive() = false; }
}