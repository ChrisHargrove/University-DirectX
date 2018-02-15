#pragma once

/*******************************************************************************************************************
	Camera.h, Camera.cpp
	Created by Kim Kane
	Last updated: 27/01/2018

	A simple FPS camera.

*******************************************************************************************************************/
#include <d3d11.h>
#include <xnamath.h>

class Camera {

public:
	Camera(float x, float y, float z);
	~Camera();

public:
	void Move(const XMFLOAT3& direction, float amount);
	void Rotate(float pitch, float yaw, float roll);

public:
	XMFLOAT3 GetPosition() const;
	XMFLOAT3 GetRotation() const;

	XMFLOAT3 GetUp() const;
	XMFLOAT3 GetForward() const;
	XMFLOAT3 GetRight() const;

	XMMATRIX GetViewMatrix();

public:
	void SetPosition(float x, float y, float z);
	void SetRotation(float x, float y, float z);

private:
	XMFLOAT3 m_position;
	XMFLOAT3 m_rotation;

	XMVECTOR m_up;
	XMVECTOR m_forward;
	XMVECTOR m_right;

	XMVECTOR m_target;

	XMMATRIX m_viewMatrix;

	static XMVECTOR s_defaultUp;
	static XMVECTOR s_defaultForward;
	static XMVECTOR s_defaultRight;
};