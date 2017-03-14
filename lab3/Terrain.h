#pragma once
#include "..\DXFramework\PlaneMesh.h"

#include <d3d11.h>
 #include <stdio.h>

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
	void Settings(bool* is_open);	
	bool diamondSquareNeedRegenerated;

private:
	bool LoadHeightMap(char*);
	void NormalizeHeightMap();

 
	void GenerateDimondSquare();
 
   
private:

	float diamondSquareRange;
	bool resetTerrain; 

	float diamondSquarePoints[129 * 129];

	bool useMusicData;
	bool usingWaves;
	bool terrainNeedReGeneration;
	bool generateTerrain;
	int terrainWidth, terrainHeight;
	HeightMapType* heightMap;
	



	HeightMapType* startingHeightmap;
	waveSettings xAxisWaveSettings, yAxisWaveSettings, zAxisWaveSettings;
};

