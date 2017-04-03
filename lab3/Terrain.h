#pragma once
#include "..\DXFramework\PlaneMesh.h"

#include <d3d11.h>
 #include <stdio.h>
#include "ShaderBuffers.h"

/*
Terrain Mesh

@Author Alan Yeats

Creates a mesh that can be used to create prosedural terrain
*/
class Terrain :	public PlaneMesh
{

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

private:
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
public:

	Terrain(std::string name, ID3D11Device * device, ID3D11DeviceContext * deviceContext, WCHAR * textureFilename, int resolution = 100);
	~Terrain();
	bool CalculateNormals();
	bool InitializeTerrain(ID3D11Device*, int terrainWidth, int terrainHeight);
 	bool GenerateHeightMap(ID3D11Device * device, bool keydown, class Sound * sound);
	void InitBuffers(ID3D11Device * device);
	XMMATRIX SendData(ID3D11DeviceContext* deviceContext);
	void Settings(bool * is_open, TerrainGeneration generation);

	bool diamondSquareNeedRegenerated;
	bool simplexNoiseRegenerated;
	bool faultLineDisplacementRegenerated;
	bool fbmregenereed;;
	bool cellularAutomataRegenerate;
	bool particleDepositionRegeneate;
	int* seed;
	FaultLineDisplacementBufferType* faultLineSettings;

	float getMaxHeight();
	float getMinHight();
private:


	bool LoadHeightMap(char*);
	void NormalizeHeightMap();
	float smoothingValue;
	bool enableSmoothing;

 
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
	HeightMapType* heightMap;
	


	float FaultLineDisplacement(int x, int z);
	HeightMapType* startingHeightmap;
	waveSettings xAxisWaveSettings, yAxisWaveSettings, zAxisWaveSettings;
};

