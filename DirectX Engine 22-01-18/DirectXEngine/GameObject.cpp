#include "GameObject.h"

GameObject::GameObject() 
{
}

GameObject::GameObject(XMVECTOR Position, Model* Model, Texture* texture)
{
	_Position = Position;
	_Direction = DEFAULT_FORWARD;
	_RotationY = XMMatrixIdentity();
	_WorldMatrix = XMMatrixIdentity();
	_ObjectModel = Model;
	m_objectTexture = texture;
}

GameObject::~GameObject()
{
}

XMVECTOR GameObject::GetPosition() const
{
	return _Position;
}

XMMATRIX GameObject::GetRotationY()
{
	XMVECTOR directionInvertX = XMVectorSet(-1.0f*XMVectorGetX(_Direction), XMVectorGetY(_Direction), XMVectorGetZ(_Direction), 0.0f);
	XMVECTOR XAxis = XMVector3Cross(DEFAULT_UP, directionInvertX);
	XAxis = XMVector3Normalize(XAxis);

	_RotationY = XMMatrixSet
	(XMVectorGetX(XAxis), XMVectorGetX(DEFAULT_UP), XMVectorGetX(directionInvertX), 0.0f,
		XMVectorGetY(XAxis), XMVectorGetY(DEFAULT_UP), XMVectorGetY(directionInvertX), 0.0f,
		XMVectorGetZ(XAxis), XMVectorGetZ(DEFAULT_UP), XMVectorGetZ(directionInvertX), 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);
	return _RotationY;
}

XMMATRIX GameObject::GetWorldMatrix()
{
	return _WorldMatrix;
}

Model* GameObject::GetModel() const
{
	return _ObjectModel;
}

Texture* GameObject::GetTexture() const
{
	return m_objectTexture;
}

void GameObject::RotateY(float angle)
{
	XMMATRIX temp = XMMatrixRotationY(angle);
	_Direction = XMVector3Transform(_Direction, temp);
	GetRotationY();
	UpdateWorldMatrix();
}

void GameObject::TranslateX(float distance)
{
	XMMATRIX translation = XMMatrixTranslation(distance, 0, 0);
	_Position = XMVector3Transform(_Position, translation);
	UpdateWorldMatrix();
}

void GameObject::TranslateY(float distance)
{
	XMMATRIX translation = XMMatrixTranslation(0, distance, 0);
	_Position = XMVector3Transform(_Position, translation);
	UpdateWorldMatrix();
}

void GameObject::UpdateWorldMatrix()
{
	XMMATRIX translation = XMMatrixTranslation(XMVectorGetX(_Position), XMVectorGetY(_Position), XMVectorGetZ(_Position));

	_WorldMatrix = _RotationY*translation;

	_WorldMatrix = XMMatrixTranspose(_WorldMatrix);
}


void GameObject::Update(ID3D11DeviceContext* deviceContext)
{
	deviceContext->UpdateSubresource(m_worldBuffer, 0, 0, &GetWorldMatrix(), 0, 0);
	deviceContext->VSSetConstantBuffers(0, 1, &m_worldBuffer);
}

void GameObject::Render(ID3D11DeviceContext* deviceContext) {
	
	deviceContext->PSSetShaderResources(0, 1, m_objectTexture->GetTexture());
	deviceContext->PSSetSamplers(0, 1, m_objectTexture->GetSampler());

	_ObjectModel->Render(deviceContext);
}