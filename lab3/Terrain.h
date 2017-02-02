#pragma once
#include "..\DXFramework\PlaneMesh.h"

#include <d3d11.h>
 #include <stdio.h>

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
		float x, y, z;
	};
public:
	Terrain(ID3D11Device* device, ID3D11DeviceContext* deviceContext, WCHAR* textureFilename, int resolution = 100);
	~Terrain();
	bool CalculateNormals();
	bool InitializeTerrain(ID3D11Device*, int terrainWidth, int terrainHeight);
	bool GenerateHeightMap(ID3D11Device* device, bool keydown);
	void InitBuffers(ID3D11Device * device);
 	void SendData(ID3D11DeviceContext* deviceContext);
	void Settings(bool* is_open);
private:
	bool LoadHeightMap(char*);
	void NormalizeHeightMap();

	void GenerateHieghtField(ID3D11Device * device);

	void GeneratePelinNoise(ID3D11Device * device);

   
private:

	bool usingWaves;
	bool usingPerlinNoise;
	float heightFieldMaxHight;
	bool usingHightField;
	bool terrainNeedReGeneration;
	bool generateTerrain;
	bool isTerrainGeneratedEnabled;
	int terrainWidth, terrainHeight;
	HeightMapType* heightMap;

	waveSettings xAxisWaveSettings, yAxisWaveSettings, zAxisWaveSettings;
};

