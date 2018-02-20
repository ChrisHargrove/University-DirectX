//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Headers
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#include "GameObject.h"
#include "GraphicsManager.h"

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
GameObject::GameObject() 
{
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
GameObject::GameObject(const XMFLOAT3& Position, Model* Model, Texture* texture)
{
    _Position = XMLoadFloat3(&Position);
	_Direction = DEFAULT_FORWARD;
	_RotationMatrix = XMMatrixIdentity();
	_WorldMatrix = XMMatrixIdentity();
	_ObjectModel = Model;
	_ObjectTexture = texture;
    _Rotation = XMVectorSet(0,0,0,0);
    
    UpdateWorldMatrix();

	//Shouldn't be done here - either a rendering engine or create shaders in gamestates and pass in to
	//render function of game objects
	m_basicShader.LoadShader(L"basicShader.vs", L"basicShader.ps");
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
GameObject::~GameObject()
{
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
const XMVECTOR& GameObject::GetPosition() const
{
	return _Position;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
const XMFLOAT3 GameObject::GetPositionF() const
{
    XMFLOAT3 temp;
    XMStoreFloat3(&temp, _Position);
    return temp;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
const XMMATRIX& GameObject::GetRotationY()
{
	return _RotationMatrix;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
const XMMATRIX& GameObject::GetWorldMatrix()
{
	return _WorldMatrix;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Model* GameObject::GetModel() const
{
	return _ObjectModel;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Texture* GameObject::GetTexture() const
{
	return _ObjectTexture;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void GameObject::RotateX(float angle)
{
    _Rotation = XMVectorSet(XMVectorGetX(_Rotation) + angle, XMVectorGetY(_Rotation), XMVectorGetZ(_Rotation), XMVectorGetW(_Rotation));
    CalculateRotationMatrix();
    UpdateWorldMatrix();
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void GameObject::RotateY(float angle)
{
    _Rotation = XMVectorSet(XMVectorGetX(_Rotation), XMVectorGetY(_Rotation) + angle, XMVectorGetZ(_Rotation), XMVectorGetW(_Rotation));
    CalculateRotationMatrix();
	UpdateWorldMatrix();
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void GameObject::RotateZ(float angle)
{
    _Rotation = XMVectorSet(XMVectorGetX(_Rotation), XMVectorGetY(_Rotation), XMVectorGetZ(_Rotation) + angle, XMVectorGetW(_Rotation));
    CalculateRotationMatrix();
    UpdateWorldMatrix();
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void GameObject::TranslateX(float distance)
{
	XMMATRIX translation = XMMatrixTranslation(distance, 0, 0);
	_Position = XMVector3Transform(_Position, translation);
	UpdateWorldMatrix();
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void GameObject::TranslateY(float distance)
{
	XMMATRIX translation = XMMatrixTranslation(0, distance, 0);
	_Position = XMVector3Transform(_Position, translation);
	UpdateWorldMatrix();
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void GameObject::TranslateZ(float distance)
{
	XMMATRIX translation = XMMatrixTranslation(0, 0, distance);
	_Position = XMVector3Transform(_Position, translation);
	UpdateWorldMatrix();
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void GameObject::Translate(const XMVECTOR & translation)
{
    _Position += translation;
    UpdateWorldMatrix();
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void GameObject::UpdateWorldMatrix()
{
	XMMATRIX translation = XMMatrixTranslation(XMVectorGetX(_Position), XMVectorGetY(_Position), XMVectorGetZ(_Position));
	_WorldMatrix = _RotationMatrix*translation;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void GameObject::CalculateRotationMatrix()
{
    _RotationMatrix = XMMatrixRotationRollPitchYaw(XMVectorGetX(_Rotation), XMVectorGetY(_Rotation), XMVectorGetZ(_Rotation));
    _Direction = XMVector3Transform(_Direction, _RotationMatrix);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void GameObject::Update()
{

}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void GameObject::Render(Camera* camera) {
	
	m_basicShader.Bind(_WorldMatrix, camera, _ObjectTexture);

	_ObjectModel->Render();
}

void GameObject::SetModel(Model * Model)
{
    _ObjectModel = Model;
}
