#include "camera.h"

Camera::Camera()
{
	m_positionX = 0.0f;
	m_positionY = 0.0f;
	m_positionZ = 0.0f;

	m_rotationX = 0.0f;
	m_rotationY = 0.0f;
	m_rotationZ = 0.0f;

	m_lookSpeed = 4.0f;

}

Camera::~Camera()
{
}

void Camera::SetFrameTime(float t)
{
	m_frameTime = t;
}

void Camera::SetPosition(float x, float y, float z)
{
	m_positionX = x;
	m_positionY = y;
	m_positionZ = z;
}

void Camera::SetRotation(float x, float y, float z)
{
	m_rotationX = x;
	m_rotationY = y;
	m_rotationZ = z;
}

XMFLOAT3 Camera::GetPosition()
{ 
	XMFLOAT3 pos(m_positionX, m_positionY, m_positionZ);
	return pos;
}

XMVECTOR Camera::GetRotation()
{
	XMVECTOR rot;
	rot = XMVectorSet(m_rotationX, m_rotationY, m_rotationZ, 1.0f);
	return rot;
}

void Camera::Update()
{
	XMVECTOR up, position, lookAt;
	float yaw, pitch, roll;
	XMMATRIX rotationMatrix;


	// Setup the vector that points upwards.
	up = XMVectorSet(0.0f, 1.0, 0.0, 1.0f);

	// Setup the position of the camera in the world.
	position = XMVectorSet(m_positionX, m_positionY, m_positionZ, 1.0f);
	
	// Setup where the camera is looking by default.
	lookAt = XMVectorSet(0.0, 0.0, 1.0f, 1.0f);
	
	// Set the yaw (Y axis), pitch (X axis), and roll (Z axis) rotations in radians.
	pitch = m_rotationX * 0.0174532925f;
	yaw = m_rotationY * 0.0174532925f;
	roll = m_rotationZ * 0.0174532925f;

	// Create the rotation matrix from the yaw, pitch, and roll values.
	rotationMatrix = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

	// Transform the lookAt and up vector by the rotation matrix so the view is correctly rotated at the origin.
	lookAt = XMVector3TransformCoord(lookAt, rotationMatrix);
	up = XMVector3TransformCoord(up, rotationMatrix);
	

	// Translate the rotated camera position to the location of the viewer.
	lookAt = position + lookAt;

	// Finally create the view matrix from the three updated vectors.
	m_viewMatrix = XMMatrixLookAtLH(position, lookAt, up);
}


void Camera::GetViewMatrix(XMMATRIX& viewMatrix)
{
	viewMatrix = m_viewMatrix;
}

void Camera::GetBaseViewMatrix(XMMATRIX& baseMatrix)
{
	XMVECTOR up, position, lookAt;

	// Setup the vector that points upwards.
	up = XMVectorSet(0.0f, 1.0, 0.0, 1.0f);
	
	// Setup the position of the camera in the world.
	position = XMVectorSet(0.0f, 0.0, -10.0, 1.0f);
	
	// Setup where the camera is looking by default.
	lookAt = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0);
	
	// Finally create the view matrix from the three updated vectors.
	baseMatrix = XMMatrixLookAtLH(position, lookAt, up);
}

void Camera::MoveForward()
{
	float radians;

	// Update the forward movement based on the frame time
	m_speed = m_frameTime * 5.f;
	
	// Convert degrees to radians.
	radians = m_rotationY * 0.0174532925f;

	// Update the position.
	m_positionX += sinf(radians) * m_speed;
	m_positionZ += cosf(radians) * m_speed;
}


void Camera::MoveBackward()
{
	float radians;

	// Update the backward movement based on the frame time
	m_speed = m_frameTime * 5.f;// *0.5f;

	// Convert degrees to radians.
	radians = m_rotationY * 0.0174532925f;

	// Update the position.
	m_positionX -= sinf(radians) * m_speed;
	m_positionZ -= cosf(radians) * m_speed;
}


void Camera::MoveUpward()
{
	// Update the upward movement based on the frame time
	m_speed = m_frameTime * 5.f;// *0.5f;
	
	// Update the height position.
	m_positionY += m_speed;
}


void Camera::MoveDownward()
{
	// Update the downward movement based on the frame time
	m_speed = m_frameTime * 5.f;// *0.5f;

	// Update the height position.
	m_positionY -= m_speed;
}


void Camera::TurnLeft()
{
	// Update the left turn movement based on the frame time 
	m_speed = m_frameTime * 25.0f;
	
	// Update the rotation.
	m_rotationY -= m_speed;

	// Keep the rotation in the 0 to 360 range.
	if (m_rotationY < 0.0f)
	{
		m_rotationY += 360.0f;
	}
}


void Camera::TurnRight()
{
	// Update the right turn movement based on the frame time
	m_speed = m_frameTime * 25.0f;
	
	// Update the rotation.
	m_rotationY += m_speed;

	// Keep the rotation in the 0 to 360 range.
	if (m_rotationY > 360.0f)
	{
		m_rotationY -= 360.0f;
	}

}


void Camera::TurnUp()
{
	// Update the upward rotation movement based on the frame time
	m_speed = m_frameTime * 25.0f;
	
	// Update the rotation.
	m_rotationX -= m_speed;

	// Keep the rotation maximum 90 degrees.
	if (m_rotationX > 90.0f)
	{
		m_rotationX = 90.0f;
	}
}


void Camera::TurnDown()
{
	// Update the downward rotation movement based on the frame time
	m_speed = m_frameTime * 25.0f;

	// Update the rotation.
	m_rotationX += m_speed;

	// Keep the rotation maximum 90 degrees.
	if (m_rotationX < -90.0f)
	{
		m_rotationX = -90.0f;
	}
}


void Camera::Turn(int x, int y)
{

	// Update the rotation.
	m_rotationY += (float)x / m_lookSpeed;// m_speed * x;

	m_rotationX += (float)y / m_lookSpeed;// m_speed * y;
}

void Camera::StrafeRight()
{
	float radians;

	// Update the forward movement based on the frame time
	m_speed = m_frameTime * 5.f;

	// Convert degrees to radians.
	radians = m_rotationY * 0.0174532925f;

	// Update the position.
	m_positionZ -= sinf(radians) * m_speed;
	m_positionX += cosf(radians) * m_speed;

}

void Camera::StrafeLeft()
{
	float radians;

	// Update the forward movement based on the frame time
	m_speed = m_frameTime * 5.f;

	// Convert degrees to radians.
	radians = m_rotationY * 0.0174532925f;

	// Update the position.
	m_positionZ += sinf(radians) * m_speed;
	m_positionX -= cosf(radians) * m_speed;
}