#include "Camera.h"
#include "Tracker.h"
#include "Tools.h"
#include "Constants.h"
#include "Log.h"

/*******************************************************************************************************************
Constructor with initializer list to set default values of data members
*******************************************************************************************************************/
Camera::Camera()	:	m_position(0.0f, 0.0f, 0.0f),
						m_rotation(0.0f, 0.0f, 0.0f),
						m_up(XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)),
						m_forward(XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f)),
						m_right(XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f)),
						m_target(XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f))

{
	DX_LOG("[CAMERA] Camera constructor initialized", DX_LOG_EMPTY, LOG_MESSAGE);
}


/*******************************************************************************************************************
	Constructor with initializer list to set default values of data members
*******************************************************************************************************************/
Camera::Camera(float x, float y, float z) :	m_position(x, y, z),
											m_rotation(0.0f, 0.0f, 0.0f),
											m_up(XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)),
											m_forward(XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f)),
											m_right(XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f)),
											m_target(XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f))

{
	DX_LOG("[CAMERA] Camera constructor initialized", DX_LOG_EMPTY, LOG_MESSAGE);
}


/*******************************************************************************************************************
	Shut down all devices, destroy resources and clean up all memory
*******************************************************************************************************************/
Camera::~Camera()
{
	DX_LOG("[CAMERA] Camera destructor initialized", DX_LOG_EMPTY, LOG_MESSAGE);
}


/*******************************************************************************************************************
	Function that moves the camera in a specific direction, by an amount passed in
*******************************************************************************************************************/
void Camera::Move(const XMFLOAT3& direction, float amount)
{
	m_position.x += direction.x * amount;
	m_position.y += direction.y * amount;
	m_position.z += direction.z * amount;
}


/*******************************************************************************************************************
	Function that rotates the camera view by the pitch, yaw and roll values passed in
*******************************************************************************************************************/
void Camera::Rotate(float pitch, float yaw, float roll)
{
	m_rotation.x += pitch;
	m_rotation.y += yaw;
	m_rotation.z += roll;
}


/*******************************************************************************************************************
	Calculates and returns the final view matrix for the camera
*******************************************************************************************************************/
XMMATRIX Camera::GetViewMatrix()
{ 
	//---------------------------------------------------------------- Set the rotations in radians
	float pitch	= m_rotation.x * MathsConstants::Radians;
	float yaw	= m_rotation.y * MathsConstants::Radians;
	float roll	= m_rotation.z * MathsConstants::Radians;

	//---------------------------------------------------------------- Adjust the right, up and forward vectors when rotating around Y (yaw) axis, so we move in the correct direction
	m_right		= XMVector3TransformCoord(s_defaultRight, XMMatrixRotationY(yaw));
	m_up		= XMVector3TransformCoord(s_defaultUp, XMMatrixRotationY(yaw));
	m_forward	= XMVector3TransformCoord(s_defaultForward, XMMatrixRotationY(yaw));

	//---------------------------------------------------------------- Create the rotation matrix from the pitch (x), yaw (y), and roll (z) values
	XMMATRIX rotationMatrix = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

	//---------------------------------------------------------------- Transform the target by the final rotation matrix
	m_target = XMVector3TransformCoord(s_defaultForward, rotationMatrix);
	m_target = XMVector3Normalize(m_target);

	//---------------------------------------------------------------- Store the position in to an XMVECTOR and set the final target to the position plus the target
	XMVECTOR position = FloatToVector(m_position);
	m_target = position + m_target;

	//---------------------------------------------------------------- Finally create the view matrix from the final position, target and up vectors
	m_viewMatrix = XMMatrixLookAtLH(position, m_target, m_up);

	return m_viewMatrix;
}


/*******************************************************************************************************************
	Accessor methods
*******************************************************************************************************************/
XMFLOAT3 Camera::GetPosition() const { return m_position; }
XMFLOAT3 Camera::GetRotation() const { return m_rotation; }

XMFLOAT3 Camera::GetUp() const		{ return VectorToFloat(m_up); }
XMFLOAT3 Camera::GetForward() const { return VectorToFloat(m_forward); }
XMFLOAT3 Camera::GetRight() const	{ return VectorToFloat(m_right); }


/*******************************************************************************************************************
	Modifier methods
*******************************************************************************************************************/
void Camera::SetPosition(float x, float y, float z)
{
	m_position.x = x;
	m_position.y = y;
	m_position.z = z;
}
void Camera::SetRotation(float x, float y, float z)
{
	m_rotation.x = x;
	m_rotation.y = y;
	m_rotation.z = z;
}


/*******************************************************************************************************************
	Static variables and functions
*******************************************************************************************************************/
XMVECTOR Camera::s_defaultUp		= XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
XMVECTOR Camera::s_defaultForward	= XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
XMVECTOR Camera::s_defaultRight		= XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);