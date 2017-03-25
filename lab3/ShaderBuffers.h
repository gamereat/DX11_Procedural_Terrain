#pragma once

#include "..\DXFramework\BaseShader.h"
#include "Scene.h"
#include "../DXFramework/Light.h"


/*
Used to hold buffer stucts used accross shader clasess

*/


const int NUM_LIGHTS = 4;

const int MAX_FAULTLINE_ITERATIONS = 2500;

 
struct DimondSquareBuffer  
{
	int widthOfGrid;
	int heightOfGrid;

	XMINT2 padding;

	float topRightRandomNumber;
	float topLeftRandomNumber;
	float bottomRightRandomNumber;
	float bottomLeftRandomNumber;
};

struct FractionalBrowningNoiseBuffer
{
	float fbnFrequancy;
	float fbnOctaves;
 	float fbnAmplitude;
	float fbnLacunarity;
	float fbnPersistence;
	float fbnPelinNoiseFreqnacy;
	float heightScale;
	float fbnPadding;

};
/*
The Current Terrain Shader being Used
*/
enum TerrainGeneration
{
	FaultLineDisplacement,
	FractionalBrowningNoise,
	SimplexNoise,
	DiamondSquare,

};


struct TerrainGenerationBufferType
{
	/*
	The Type of generation used. NOTE this should be cast from TerrainGeneration enum
	*/
	TerrainGeneration terrainGenerationType;

	bool enableGPUEffect;

	float  highScale;
	int  padding;
};


struct TerrainSettingTextureType
{
	

	int enableLighting;
	int padding;
	int textureTiling;

	int displayNormalMap;
};
 
struct FaultLineDisplacementBufferType
{
	/*
	Number of iterations to complete
	*/
	int numberOfIterations;
	/*
	The starting displacment value
	*/
	float startingDisplacement;
	/*
	The minimum value the last displacement will set to be
	*/
	float minimumDisplacement;
	
	/*
	SmoothingValue
	*/
	float smoothingValue;
	/*

	2D array holding the diffrent random points on the surface which should be sued to complete fault line displ


	Using heavy packing on varrible.
	x and y being the first 2 random points on the surface,
	z and w beingthe second random point on the surface
	Note 1000 will be the maxium number of iterations

	*/
	XMINT4 interationsRandomPoints[MAX_FAULTLINE_ITERATIONS];

	/*
	IF fault line displacement is active
	*/
	bool enableFaultLineDisplacement;

	XMFLOAT3 padding;
};

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