#ifndef _LIGHT_H_
#define _LIGHT_H_

#include <directxmath.h>
#include <string>
using namespace DirectX;

class Light
{


public:
	Light();
	enum class lightType
	{
		directional,
		point,
		spot
	};
	void* operator new(size_t i)
	{
		return _mm_malloc(i, 16);
	}

	void operator delete(void* p)
	{
		_mm_free(p);
	}

 
	void GenerateViewMatrix();
	void GenerateProjectionMatrix(float, float);
	void generateOrthoMatrix(float screenWidth, float screenHeight, float near, float far);

	void DisplayGUIEditor(std::string lightNum, bool* is_open);


	// Setters
	void SetAmbientColour(float, float, float, float);
	void SetDiffuseColour(float, float, float, float);
	void SetDirection(float, float, float);
	void SetSpecularColour(float, float, float, float);
	void SetSpecularPower(float);
	void SetPosition(float, float, float);
	void SetLookAt(float, float, float);
	void SetRange(float);

	void SetAttenuationContantFactor(float);
	void SetAttenuationLinearFactor(float);
	void SetAttenuationQuadraticFactor(float);


	void SetWillGenerateShadows(bool);

	// Getters
	XMFLOAT4 GetAmbientColour();
	XMFLOAT4 GetDiffuseColour();
	XMFLOAT3 GetDirection();
	XMFLOAT4 GetSpecularColour();
	float GetSpecularPower();
	XMFLOAT3 GetPosition();
	XMMATRIX GetViewMatrix();
	XMMATRIX GetProjectionMatrix();
	XMMATRIX GetOrthoMatrix();

	float GetRange();
	float GetAttenuationContantFactor();
	float GetAttenuationLinearFactor();
	float GetAttenuationQuadraticFactor();
	lightType GetLightType();
	bool GetMakesSpecular();

	bool GetWillGenerateShadows();


protected:
	XMFLOAT4 m_ambientColour;
	XMFLOAT4 m_diffuseColour;
	XMFLOAT3 m_direction;
	XMFLOAT4 m_specularColour;
	float m_specularPower;
	XMVECTOR m_position;
	XMMATRIX m_viewMatrix;
	XMMATRIX m_projectionMatrix;
	XMMATRIX m_orthoMatrix;
	XMVECTOR m_lookAt; 
	float m_range;
	float m_attenuationConstantFactor;
	float m_attenuationLinearFactor;
	float m_attenuationQuadraticFactor;

	bool m_makeSpecular;
	lightType m_lightType;

	/*
	if the light will display show shadows
	*/
	bool m_willGenereateShadows;
};

#endif