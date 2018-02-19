#pragma once

#include "GameObject.h"
#include "PhysicsObject.h"

class Actor : public GameObject, public PhysicsObject
{
public:
    Actor(XMFLOAT3 Position, Model* Model, Texture* texture);
    ~Actor();


    virtual void Update() override;
};

