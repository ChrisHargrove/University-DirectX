#ifndef GAME_H_
#define GAME_H_

#include "DXApplication.h"
#include "GameObject.h"
#include "FirstPersonCamera.h"
#include "Model.h"

class Game : public DXApplication
{
public:
	Game(HINSTANCE hInstance, const char* title, int clientWidth, int clientHeight);
	~Game();

	//The override keyword throws error if it's not actually an overwritable method
	bool Initialize() override;
	void Shutdown() override;

protected:
	bool Update(float dt) override;
	void Render(float dt) override;

private:
	bool LoadResources();
	void ClearScreen();

	HRESULT SetupConstantBuffers();
	HRESULT SetupAlphaBlending();

	void AlphaBlendingState(bool state = false);
	void ZBufferState(bool state = false);

private:
	Model		LaraModel;
	Model		CubeModel;
	
	Texture		LaraTexture;
	Texture		CubeTexture;

	GameObject* LaraObject;
	GameObject* CubeObject;

	ID3D11Buffer* m_viewBuffer; //Camera class
	ID3D11Buffer* m_projectionBuffer;  //Camera class
	ID3D11Buffer* m_cameraPositionBuffer; //Camera class
	ID3D11Buffer* m_worldBuffer; //Transform hierarchy for game objects
	
	XMMATRIX m_projectionMatrix; //Camera class

	FirstPersonCamera m_firstPersonCamera;
};

#endif