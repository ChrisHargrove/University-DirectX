#ifndef GAME_OBJECT_H_
#define GAME_OBJECT_H_

#include<d3d11.h>
#include<d3dx11.h>
#include<DxErr.h>
#include<xnamath.h>

#include "AlignedAllocationPolicy.h"
#include "Model.h"
#include "Texture.h"
#include "BasicShader.h"

class GameObject : public AlignedAllocationPolicy<BYTE_16>
{
public:
	GameObject();
	GameObject(XMFLOAT3 Position, Model* Model, Texture* texture);
	~GameObject();

	XMVECTOR GetPosition() const;
	XMMATRIX GetRotationY();
	XMMATRIX GetWorldMatrix();
	Model* GetModel() const;
	Texture* GetTexture() const;

	void RotateY(float angle);
	void TranslateX(float distance);
	void TranslateY(float distance);
	void TranslateZ(float distance);

	void UpdateWorldMatrix();

	void Update();
	void Render(Camera* camera);

protected:
	Model*			_ObjectModel;

private:
	const XMVECTOR DEFAULT_UP = XMVectorSet( 0.0f, 1.0f, 0.0f , 0.0f);
	const XMVECTOR DEFAULT_FORWARD = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	const XMVECTOR DEFAULT_RIGHT = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);

	XMVECTOR _Position;
	XMVECTOR _Direction;
	XMMATRIX _RotationY;
	XMMATRIX _WorldMatrix;

	Texture*		m_objectTexture;
	ID3D11Buffer*	m_worldBuffer;

	BasicShader		m_basicShader;
};

#endif