#ifndef _CAMERA_H_
#define _CAMERA_H_

#include <directxmath.h>

using namespace DirectX;

class Camera
{
public:
	void* operator new(size_t i)
	{
		return _mm_malloc(i, 16);
	}

	void operator delete(void* p)
	{
		_mm_free(p);
	}

	Camera();
	Camera(const Camera&);
	~Camera();

	void SetPosition(float, float, float);
	void SetRotation(float, float, float);

	XMFLOAT3 GetPosition();
	XMVECTOR GetRotation();

	void Update();
	void GetViewMatrix(XMMATRIX&);
	void GetBaseViewMatrix(XMMATRIX&);

	void SetFrameTime(float);

	void MoveForward();
	void MoveBackward();
	void MoveUpward();
	void MoveDownward();
	void TurnLeft();
	void TurnRight();
	void TurnUp();
	void TurnDown();
	void StrafeRight();
	void StrafeLeft();
	void Turn(int x, int y);

private:
	float m_positionX, m_positionY, m_positionZ;
	float m_rotationX, m_rotationY, m_rotationZ;
	XMMATRIX m_viewMatrix;
	float m_speed, m_frameTime;
	float m_lookSpeed;

};

#endif