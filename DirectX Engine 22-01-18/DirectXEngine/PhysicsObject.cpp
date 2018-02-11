//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Headers
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#include "PhysicsObject.h"

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
PhysicsObject::PhysicsObject() :
    _Velocity(XMVectorSet(0, 0, 0, 0)),
    _Desired(XMVectorSet(0, 0, 0, 0)),
    _Acceleration(XMVectorSet(0, 0, 0, 0)),
    _Mass(1.0f),
    _MaxForce(1.0f),
    _MaxSpeed(1.0f)
{
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
PhysicsObject::~PhysicsObject()
{
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void PhysicsObject::SetVelocity(const XMVECTOR Velocity)
{
    _Velocity = Velocity;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void PhysicsObject::SetDesiredVelocity(const XMVECTOR Velocity)
{
    _Desired = Velocity;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void PhysicsObject::SetAcceleration(const XMVECTOR Acceleration)
{
    _Acceleration = Acceleration;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void PhysicsObject::SetMass(float Mass)
{
    _Mass = Mass;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void PhysicsObject::SetMaxSpeed(float MaxSpeed)
{
    _MaxSpeed = MaxSpeed;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void PhysicsObject::SetMaxForce(float MaxForce)
{
    _MaxForce = MaxForce;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void PhysicsObject::AddVelocity(const XMVECTOR Velocity)
{
    _Velocity += Velocity;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
const XMVECTOR& PhysicsObject::GetVelocity() const
{
    return _Velocity;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
const XMVECTOR & PhysicsObject::GetDesiredVelocity() const
{
    return _Desired;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
const XMVECTOR & PhysicsObject::GetAcceleration() const
{
    return _Acceleration;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
float PhysicsObject::GetMass() const
{
    return _Mass;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
float PhysicsObject::GetMaxSpeed() const
{
    return _MaxSpeed;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
float PhysicsObject::GetMaxForce() const
{
    return _MaxForce;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void PhysicsObject::ApplyForce(const XMVECTOR Force)
{
    //Newtons 2nd Law: Force = Mass * Acceleration
    //Therefore:
    _Acceleration += (Force / _Mass);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void PhysicsObject::ApplyFriction(float Coeficient)
{
    //Law of Dry Friction:
    //Force = -1 * Coeficient of Friction * Normal Force * Unit Length Velocity.

    XMVECTOR force = _Velocity;
    force = XMVector4Normalize(force);   //Get Unit Length of current velocity.
    force *= Coeficient;                 //Multiply by friction coeficient. Assume normal force is 1(flat surface).
    force *= -1.0f;                      //Negate friction so it acts in opposite direction to movement.
    ApplyForce(force);                   //Apply friction force.
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void PhysicsObject::ApplyDrag(float Coeficient)
{
    //Law of drag:
    //Force = -1/2 * Fluid Density * (Magnitude of Velocity)^2 * Surface Area of Object * Coeficient of Drag * Unit Length Velocity.
    XMVECTOR force = _Velocity;
    float magSq = XMVectorGetX(XMVector4LengthSq(force)); //Gets the Magnitude of the velocity squared.
    force = XMVector4Normalize(force);                    //Get Unit Length of current Velocity.
    float negCo = -Coeficient;                            //Negate coeficient to reverse direction of force.
    force *= negCo;                                       //Finish Equation.
    force *= magSq;                                       //Assume Density and Surface Area = 1 for simplicity.
    ApplyForce(force);                                    //Apply drag force.

}
