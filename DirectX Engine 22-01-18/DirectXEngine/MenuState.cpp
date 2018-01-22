#include "MenuState.h"
#include "Game.h"
#include "Log.h"
#include "Matrix4f.h"
#include "Constants.h"

/*******************************************************************************************************************
	Constructor with initializer list to set default values of data members
*******************************************************************************************************************/
MenuState::MenuState(GameState* previousState)	:	GameState(previousState),
													meshLaraExample("Assets/Models/Lara.obj", "Assets/Textures/Lara.png"),
													meshCubeExample("Assets/Models/Cube.obj", "Assets/textures/Cube.jpg"),
													meshFontExample("Assets/Models/Font.obj", "Assets/Textures/Font.jpg"),
													meshSphereExample("Assets/Models/Sphere.obj", "Assets/Textures/Sphere.jpg"),
													meshPyramidExample("Assets/Models/Pyramid.obj", "Assets/Textures/Pyramid.jpg")
{

	Log("[MENU STATE] MenuState constructor called", EMPTY, LOG_MESSAGE);
	
	Initialize();
}


/*******************************************************************************************************************
	Cleanup all memory usage, delete all objects and shut down all devices
*******************************************************************************************************************/
MenuState::~MenuState() {

	if (terrainGridExample)			{ delete terrainGridExample; terrainGridExample = nullptr; }
	if (terrainPlaneExample)		{ delete terrainPlaneExample; terrainPlaneExample = nullptr; }
	
	if (rootObjectExample)			{ delete rootObjectExample; rootObjectExample = nullptr; }
	
	//---------------------------------------------------------------- The deletion of child objects is done within the GameObject class.
	
	Log("[MENU STATE] MenuState destructor initialized", EMPTY, LOG_MESSAGE);
}


/*******************************************************************************************************************
	Initialize all start up procedures specific to the menu state
*******************************************************************************************************************/
bool MenuState::Initialize() {

	IsActive() = IsAlive() = true;
	
	/*******************************************************************************************************************
	[ Example ] Initialize the main camera component for the scene - Still Testing
	Last updated 12/01/2018

	I am planning on having a list of game components, that are part of every game state, rather than making them
	a game object. This will be useful for things like the interface- switching between 2D and 3D, changing the graphics
	settings depending on what mode the player is in, etc. All settings will be loaded in from a file (like the near view
	and far view values). This design seems better than just having a list of game objects, which is limited as not all
	game objects have a transform, some are temporarily in the scene, and some are unmovable and don't have a mesh or animation.
	Components effectively are to do with the setting up of the game, how it's viewed, played and what not, whereas the
	game objects I have will be everything that is IN the game, primarily having a mesh/model, a transform, and a shader.

	*******************************************************************************************************************/
	m_camera->InitializePerspectiveView(70.0f, Screen::Instance()->GetAspectRatio(), 0.1f, 1000.0f);

	/*******************************************************************************************************************
	[ Example ] Object creation using obj files and textures and adding them to the transform hierarchy - Still Testing
	Last updated 05/01/2018

	*******************************************************************************************************************/

	//---------------------------------------------------------------- Creating the root object (parent object of ALL the objects)
	rootObjectExample = new GameObject(Vector3f(-1.0f, -2.0f, 10.0f), meshFontExample, "ROOT");
	
	//---------------------------------------------------------------- Adding a child to the parent object
	rootObjectExample->AddChild(gameObjectLaraExample = new GameObject(Vector3f(-10.0f, 1.0f, 0.0f), meshLaraExample, "LARA"));
	
	//---------------------------------------------------------------- Adding a child to this child
	gameObjectLaraExample->AddChild(gameObjectCubeExample = new GameObject(Vector3f(-5.0f, -0.4f, 0.0f), meshCubeExample, "CUBE"));

	//---------------------------------------------------------------- Adding a child to the parent object
	rootObjectExample->AddChild(gameObjectSphereExample = new GameObject(Vector3f(30.0f, 2.0f, 10.0f), meshSphereExample, "SPHERE"));

	//---------------------------------------------------------------- Adding a child to this child
	gameObjectSphereExample->AddChild(gameObjectPyramidExample = new GameObject(Vector3f(5.0f, 0.2f, 5.0f), meshPyramidExample, "PYRAMID"));


	/*******************************************************************************************************************
	[ Example ] Terrain creation using user defined data - Still Testing
	Last updated 05/01/2018

	Have a play around with values passed in! I got it up to 100,000 before the fragment shader started acting weird :)
	
	If you want to switch to a grid, all you have to do is comment out the 3 lines that generate the plane,
	then call terrainExampleGrid->RenderArrays() in the draw funtion below, instead of terrainExamplePlane->RenderIndexed()

	*******************************************************************************************************************/
	terrainPlaneExample = new Terrain("Assets/textures/Cube.jpg");
	terrainPlaneExample->GeneratePlane(100, 100);
	terrainPlaneExample->FillBuffers(Terrain::TerrainType::PLANE);
	
	//terrainGridExample = new Terrain("Assets/Textures/Cube.jpg");
	//terrainGridExample->GenerateGrid(100);
	//terrainGridExample->FillBuffers(Terrain::TerrainType::GRID);

	return true;
}


/*******************************************************************************************************************
	Function that updates everything within the menu state
*******************************************************************************************************************/
bool MenuState::Update() {

	m_keys = Input::Instance()->GetKeyStates();

	IsWindowClosed();

	//---------------------------------------------------------------- If return is pressed, switch to Play state
	if (m_keys[SDL_SCANCODE_RETURN]) {
		MainGame::Instance()->PermanentState(new PlayState(this));
		IsActive() = IsAlive() = false;
	}

	//---------------------------------------------------------------- Update all input tests
	InputTesting();

	//---------------------------------------------------------------- Temporary variables to demonstrate movement and rotation
	static float temp = 0.0f;
	temp = SDL_GetTicks() / 1000.0f;
	float sinTemp = (float)(abs(sin(temp)));
	static float move = 0.0f;
	
	/*******************************************************************************************************************
		[EXAMPLE] Transform examples showing how transform parent-child hierarchy works so far.

		This class still needs a lot of work- primarily where optimization is concerned. Currently, all objects are
		updated every frame even if they don't change their orientation. This will be fixed in the final version so that
		only objects that move or rotate are updated.

		There is also a problem with the Z and X rotation of objects local transforms. Object's don't rotate around
		their own Z and X axis correctly (they become skewed, rather than rotating).
		Feel free to try this by changing the rotation axis below. I need to look in to this and fix it.

		Also, the function calls won't be messy like they are now - there will be a rotate and move type function
		within the transform class, that will encapsulate all the messy code!
		So it will look something like this: gameObject->GetTransform()->Rotate(glm::vec3(0,1,0), 90.0f)), or similar.

	*******************************************************************************************************************/
	
	//Sphere object rotates around its own Y axis, and pyramid object follows.
	gameObjectSphereExample->GetTransform()->SetRotation(Quaternion().InitializeRotation(Vector3f(0, 1, 0), temp * 180.0f));

	//---------------------------------------------------------------- [EXAMPLE] Pyramid object - child of sphere object
	
	//Pyramid object rotates around its own local Y axis.
	gameObjectPyramidExample->GetTransform()->SetRotation(Quaternion().InitializeRotation(Vector3f(0, 1, 0), temp * 180.0f));
	
	//Pyramid object increases and decreases its own Y position.
	gameObjectPyramidExample->GetTransform()->SetPosition(Vector3f(5.0f, sinTemp * 10.0f, 5.0f));

	//---------------------------------------------------------------- [EXAMPLE] Lara object - parent of cube object

	//Lara object moves along its own Z axis, and cube object follows (press the spacebar when in the game)
	if (m_keys[SDL_SCANCODE_SPACE]) { move += 0.03f; }
	gameObjectLaraExample->GetTransform()->SetPosition(Vector3f(-10.0, 1.0, move));

	//---------------------------------------------------------------- [EXAMPLE] Update the root parent object (the parent of ALL objects in the scene)
	rootObjectExample->Update();

	return true;
}


/*******************************************************************************************************************
	Function that renders all menu state graphics to the screen
*******************************************************************************************************************/
bool MenuState::Draw() {

	Screen::Instance()->SetBackgroundColor(glm::vec4(0.1f, 0.1f, 0.3f, 1.0f));

	//---------------------------------------------------------------- [EXAMPLE] Terrain being drawn
	terrainPlaneExample->RenderIndexed(m_camera);
	//terrainGridExample->RenderArrays(m_camera);
	
	//---------------------------------------------------------------- [EXAMPLE] Root object responsible for drawing ALL the game objects (this will probably be created in either the Game class or GameState class... haven't decided yet!
	rootObjectExample->Draw(m_camera);

	//To do: LightBox
	
	return true;
}


/*******************************************************************************************************************
	[TEMPORARY] Testing function to demonstrate input
*******************************************************************************************************************/
void MenuState::InputTesting()
{
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
	if (Input::Instance()->MouseButtonPressed(SDL_BUTTON_LEFT))		{ m_camera->RotateVertical(-InputConstants::RotateSpeed); }
	if (Input::Instance()->MouseButtonPressed(SDL_BUTTON_RIGHT))	{ m_camera->RotateVertical(InputConstants::RotateSpeed); }
	m_camera->RotateHorizontal(Input::Instance()->GetMouseWheel().y / 10.0f);


	/*******************************************************************************************************************
	[ Example ] Checking PS4 button presses - Still Testing
	Last updated 05/01/2018

	*******************************************************************************************************************/
	if (Input::Instance()->ControllerButtonPressed("buttonX"))			{ Log("X Button pressed", EMPTY, LOG_WARN); }
	if (Input::Instance()->ControllerButtonPressed("buttonSquare"))		{ Log("Square Button pressed", EMPTY, LOG_WARN); }
	if (Input::Instance()->ControllerButtonPressed("buttonCircle"))		{ Log("Circle Button pressed", EMPTY, LOG_WARN); }
	if (Input::Instance()->ControllerButtonPressed("buttonTriangle"))	{ Log("Triangle Button pressed", EMPTY, LOG_WARN); }
	if (Input::Instance()->ControllerButtonPressed("buttonL1"))			{ Log("L1 Button pressed", EMPTY, LOG_WARN); }
	if (Input::Instance()->ControllerButtonPressed("buttonR1"))			{ Log("R1 Button pressed", EMPTY, LOG_WARN); }
	if (Input::Instance()->ControllerButtonPressed("buttonL2"))			{ Log("L2 Button pressed", EMPTY, LOG_WARN); }
	if (Input::Instance()->ControllerButtonPressed("buttonR2"))			{ Log("R2 Button pressed", EMPTY, LOG_WARN); }
	if (Input::Instance()->ControllerButtonPressed("buttonL3"))			{ Log("L3 Button pressed", EMPTY, LOG_WARN); }
	if (Input::Instance()->ControllerButtonPressed("buttonR3"))			{ Log("R3 Button pressed", EMPTY, LOG_WARN); }
	if (Input::Instance()->ControllerButtonPressed("buttonShare"))		{ Log("Share Button pressed", EMPTY, LOG_WARN); }
	if (Input::Instance()->ControllerButtonPressed("buttonOptions"))	{ Log("Options Button pressed", EMPTY, LOG_WARN); }
	if (Input::Instance()->ControllerButtonPressed("buttonHome"))		{ Log("Home Button pressed", EMPTY, LOG_WARN); }
	if (Input::Instance()->ControllerButtonPressed("buttonTouchPad"))	{ Log("Touchpad Button pressed", EMPTY, LOG_WARN); }


	/*******************************************************************************************************************
	[ Example ] Camera movement and rotation with PS4 Controller Axis - Still Testing
	Last updated 05/01/2018

	*******************************************************************************************************************/
	m_camera->Move(m_camera->GetLeft(), -Input::Instance()->GetControllerLeftAxis().x);
	m_camera->Move(m_camera->GetForward(), -Input::Instance()->GetControllerLeftAxis().y);
	m_camera->RotateVertical(Input::Instance()->GetControllerRightAxis().x);
	m_camera->RotateHorizontal(Input::Instance()->GetControllerRightAxis().y);


	/*******************************************************************************************************************
	[ Example ] Camera rotation with PS4 Controller Directional Pad - Still Testing
	Last updated 05/01/2018

	*******************************************************************************************************************/
	m_camera->RotateHorizontal(Input::Instance()->GetDirectionalPad().y / 100.0f);
	m_camera->RotateVertical(Input::Instance()->GetDirectionalPad().x / 100.0f);
}


/*******************************************************************************************************************
	Function that checks if the player has closed the game window within the menu state
*******************************************************************************************************************/
void MenuState::IsWindowClosed() {

	if (Input::Instance()->IsWindowClosed())	{ IsActive() = IsAlive() = false; }
	if (m_keys[SDL_SCANCODE_ESCAPE])			{ IsActive() = IsAlive() = false; }
}
