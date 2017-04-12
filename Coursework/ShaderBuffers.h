#pragma once

#include "..\DXFramework\BaseShader.h"
#include "Scene.h"
#include "../DXFramework/Light.h"


/*
Used to hold buffer stucts used accross shader clasess

*/

const int WIDTH_OF_TERRAIN = 129;
const int HEIGHT_OF_TERRAIN = 129;

const int NUM_LIGHTS = 4;

const int MAX_FAULTLINE_ITERATIONS = 2500;
const int NUMBER_LSYSTEMS = 11;


//http://algorithmicbotany.org/papers/abop/abop-ch1.pdf
enum lSystemTypes
{
	DragonCurve,
	KochCurve,
	SierpinskisCarpet,
	HeighwayDragon,
	JoinedCrossCurves,
	modifiedKoch1,
	modifiedKoch2,
	modifiedKoch3,
	modifiedKoch4,
	modifiedKoch5,
	modifiedKoch6,
};




struct waveSettings
{
public:
	enum WaveType {
		sin,
		cos,
		tan
	};
	float period;
	float amplitude;

	WaveType waveType;


};


struct HeightMapType
{
	float x, y, z;
	float nx, ny, nz;
};

struct VectorType
{
	VectorType(float x, float y, float z)
	{
		this->x = x;
		this->y = y;
		this->z = z;

	}
	VectorType()
	{
		x = 0;
		y = 0;
		z = 0;
	}
	float x, y, z;
};
 
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
 
	bool useAbs;
	XMFLOAT3 fbmPadding;

	bool useRidged;


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
	ParticleDeposition


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
 


	float topHighPercentage;
	float midHighPercentage;

	float blendingPercentage;
	float minHightOfTerrain;

	int enableLighting;


	int textureTiling;

	int displayNormalMap;

	float minHeightOfWater;

	float medHeightOfWater;

	float maxHeightOfWater;

	float maxHightOfHill;

	float  padding;



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