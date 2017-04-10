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
	void Settings(bool * is_open, TerrainGeneration generation);

	/*
	Will generate a dimoand square terrain and set terrain to such
 	*/
	bool generateDiamondSquareTerrain;
	/*
	Will generate a Simplex Noise terrain and set terrain to such
	*/
	bool generateSimplexNoise;

	/*
	Will generate a Fault line displaymentterrain and set terrain to such
	*/
	bool generateFaultLinelineDisplacement;
	
	/*
	Will generate a Fractional Brownian Noise terrain and set terrain to such
	*/
	bool genereateFractionalBrownainNoise;
 

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
private:

	/*
	Caculates and sets normals for the terrain 
	*/
	bool CalculateNormals();

	/*
	Array that holds the terrain hight map
	*/
	HeightMapType* terrainArray;



	bool LoadHeightMap(char*);
	void NormalizeHeightMap();
	float smoothingValue;
	bool enableSmoothing;

 
	/*
	Caculates and applies dimond square algorithum
	*/
	void GenerateDimondSquare();
	void GenerateSimplexNoiseNoise();
 	void GenerateFBmNoise();
	void GenerateFaultLineDisplacement();
	void GenerateCellularAutomata();
	void GenereatParticleDeposition();


	int particleDepositionIterations;
	int numberOfHills;	
	float CellularAutomataRule(float neibours[8], float center);
	int cellularAutomationIterations;
	int chanceOfNewHill;
	float chanceOfRoll;

	int diamondSquareRange;
	bool resetTerrain; 

	double* perinNoiseValues;
	float perlinNoiseFrequancy;
	float perlinNoiseHeightRange;

	float diamondSquarePoints[129 * 129];

	bool useMusicData;
 	bool terrainNeedReGeneration;
	bool generateTerrain;
	int terrainWidth, terrainHeight;
	

	/*
	Pointer to the seed for terrain generation
	*/
	int* randomSeed;


	float FaultLineDisplacement(int x, int z);
	HeightMapType* startingHeightmap;
	waveSettings xAxisWaveSettings, yAxisWaveSettings, zAxisWaveSettings;
};

