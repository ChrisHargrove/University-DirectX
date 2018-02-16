#include "Actor.h"



Actor::Actor(XMFLOAT3 Position, Model* Model, Texture* texture) :
    GameObject(Position, Model, texture)
{
}


Actor::~Actor()
{
}

void Actor::Update()
{
    AddVelocity(GetAcceleration());
    SetAcceleration(XMVectorSet(0, 0, 0, 0));
    Translate(GetVelocity());

    if (XMVectorGetX(XMVector4Length(GetAcceleration())) < 0.1f) {
        //SetAcceleration(XMVectorSet(0, 0, 0, 0));
    }

   // deviceContext->UpdateSubresource(m_worldBuffer, 0, 0, &GetWorldMatrix(), 0, 0);
    //deviceContext->VSSetConstantBuffers(0, 1, &m_worldBuffer);
}
