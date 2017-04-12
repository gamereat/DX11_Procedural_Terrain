#pragma once
#include "..\DXFramework\PlaneMesh.h"

#include <d3d11.h>
 #include <stdio.h>
#include "ShaderBuffers.h"

 
/*
* Terrain Mesh
*
* @author      Alan Yeats
*
* Creates a mesh that can be used to create prosedural terrain
*/
class Terrain :	public PlaneMesh
{ 
public:
	// TODO: delete
	bool particleDepositionRegeneate;



	Terrain(std::string name, ID3D11Device * device, ID3D11DeviceContext * deviceContext, WCHAR * textureFilename, int resolution = 100);
	~Terrain();
	bool InitializeTerrain(ID3D11Device*, int terrainWidth, int terrainHeight);
 	bool GenerateHeightMap(ID3D11Device * device, bool keydown, class Sound * sound);
	void InitBuffers(ID3D11Device * device);
	XMMATRIX SendData(ID3D11DeviceContext* deviceContext);


	/*
	GUI settings 
	@param  is_open			if the terrain is open
	@param  generation		The generation type to use 

	*/
	void GUISettings(bool * is_open, TerrainGeneration generation);

	/*
	Will generate a dimoand square terrain and set terrain to such
 	*/
	bool generateDiamondSquareTerrain;
	/*
	Will generate a Simplex Noise terrain and set terrain to such
	*/
	bool generateSimplexNoiseTerrain;

	/*
	Will generate a Fault line displaymentterrain and set terrain to such
	*/
	bool generateFaultLinelineDisplacement;
	
	/*
	Will generate a Fractional Brownian Noise terrain and set terrain to such
	*/
	bool genereateFractionalBrownainNoise;
 
	/*
	The fault line displacment settings to use 
	*/
	FaultLineDisplacementBufferType* faultLineSettings;

	/*
	Gets the maxium hight level of the terrain 
	@return the max hight of the terrain
	*/
	float getMaxHeight();
	/*
	Gets the miniumum hight level of the terrain
	@return the max hight of the terrain
	*/
	float getMinHight();

	/*
	Set Random Seed
	Sets the pointer to the random seed to be used
	*/
	void setRandomSeed(int* seed);

	FractionalBrowningNoiseBuffer* fBMSettings;
private:

	/*
	Caculates and sets normals for the terrain 
	*/
	bool CalculateNormals();
	
	 
	/*
	Will smooth the given terrain based off neibours 
	*/
	void SmoothTerrain();
	
	/*
	Caculates and applies dimond square algorithum
	*/
	void GenerateDimondSquare();
	/*
	Caculates and applies simplex noise
	*/
	void GenerateSimplexNoiseNoise();
	/*
	Caculate and applies frastical browning motion
	*/
	void GenerateFBmNoise();
	/*
	Caculates and applies fault line displaments
	*/
	void GenerateFaultLineDisplacement();

	/*
	Caculates and appliesparticles despisiton
	*/
	void GenereatParticleDeposition();



	/*
	Will work out the fault line displacement value for a given value 
	*/
	float FaultLineDisplacement(int x, int z);


	/*
	The scale of the number of neibour around point should create smoothing from
	*/
	float smoothingValue;

	/*
	if smoothing is in use
	*/
	bool isUsingSmoothing;

	/*
	Array that holds the terrain hight map
	*/
	HeightMapType* terrainArray;
	
	/*
	The number of iteration for particle displacment 
	*/
	int particleDepositionIterations;

	/*
	
	*/
	int numberOfHills;	
	int cellularAutomationIterations;
	int chanceOfNewHill;
	float chanceOfRoll;

	/*
	The range used for diamond square
	*/
	int diamondSquareRange;
 
	/*
	Resets the terriain
	*/
	bool resetTerrain;

	/*
	Frequance of perlin noise ( used for both x and z)
	*/
	float perlinNoiseFrequancy;
	
	/*
	Frequance of perlin noise ( used for both x and z)
	*/
	float perlinNoiseHeightRange;

	/*
	The points used told temparly hold diamond square values before applying to terrain
	*/
	float diamondSquarePoints[WIDTH_OF_TERRAIN * HEIGHT_OF_TERRAIN];

	/*
	If terrain will be affect by built in audio
	*/
	bool useMusicData;

	/*
	if the terrain needs to regenerated
	*/
 	bool doesTerrainNeedRegenerated;
	 
	/*
	The terrain width
	*/
	int terrainWidth;
	/*
	The terrain height
	*/
	int terrainHeight;
	
	/*
	Pointer to the seed for terrain generation
	*/
	int* randomSeed;
	  
};