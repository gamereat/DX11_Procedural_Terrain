#pragma once

#include "..\DXFramework\BaseShader.h"
#include "Scene.h"
#include "../DXFramework/Light.h"


/*
Used to hold buffer stucts used accross shader clasess

*/


const int NUM_LIGHTS = 4;







struct TessellationBufferType
{


	float maxDistance;

	float minDistance;

	float minTesselationAmmount;
	float maxTesselationAmmount;
	XMFLOAT4 camPos;

};
struct WavetBufferType
{



	float amplutude;
	float time;
	float speed;
	float steepnesss;


	XMFLOAT3 freqancy;
	float padding;

};

struct MatrixBufferType2
{
	XMMATRIX world;
	XMMATRIX view;
	XMMATRIX projection;
	XMMATRIX lightView[NUM_LIGHTS];
	XMMATRIX lightProjection[NUM_LIGHTS];
};
struct CammeraBufferType
{
	XMFLOAT3 cammeraPostion;
	float padding;
};
struct LightBufferType
{
	//XMFLOAT4 ambient;
	//XMFLOAT4 diffuse; 

	XMFLOAT4 diffuseColour[NUM_LIGHTS];
	XMFLOAT4 lightDirection[NUM_LIGHTS];
	XMFLOAT4 ambientColour[NUM_LIGHTS];
	XMFLOAT4 specularColour[NUM_LIGHTS];
	XMFLOAT4 position[NUM_LIGHTS];

	XMFLOAT4 specularPower[NUM_LIGHTS];

	// 0 is constant
	// 1 linear
	// 2 quadratic
	// 3 range
	XMFLOAT4 attenuationValues[NUM_LIGHTS];

	//0 - directional
	//1- spot
	//2- point
	XMINT4 lightType[NUM_LIGHTS];

	int isSpecular[NUM_LIGHTS];

	int willGenerateShadows[NUM_LIGHTS];

};

struct LightBufferType2
{
	XMFLOAT4 position[NUM_LIGHTS];
	//float padding;
};