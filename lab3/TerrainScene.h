#pragma once
#include "Scene.h"
#include "../DXFramework/SphereMesh.h"
#include "../DXFramework/CubeMesh.h"
#include "../DXFramework/QuadMesh.h"
#include "../DXFramework/DoubleTriangleMesh.h"
#include "DepthShader.h"
#include "ColourShader.h"
#include "Terrain.h"
#include "../DXFramework/CubeMesh.h"
#include "TextureShader.h"
#include "DistanceBasedTesselation.h"
#include "../DXFramework/Model.h"
#include "Sound.h"
#include "TerrainShader.h"
#include "LSystem.h"
#include <string>
#include "WaterShader.h"
#include <random>

/*
Terrain Scene 
@Author Alan Yeats

Used to show proseduraly generated terrain
*/
 class TerrainScene :
	public Scene
{
	 const char* TerrainGeneration_str[5] =
	 {
		 "FaultLineDisplacement",
		 "PerlinNoise",
		 "FractionalBrowningNoise",
		 "SimplexNoise",
		 "DiamondSquare",
	 };
public:
	TerrainScene(string sceneName);
	~TerrainScene();

	/*
	Initializes the given Scene

	@param hwnd		A handle to the window
	@param device	The directx11 device
	@param deviceContext	The directx11 device Context

	*/
	virtual void Init(HWND hwnd, ID3D11Device* device, ID3D11DeviceContext* deviceContext, Sound* sound);

	/*
	Updates the current scene

	@param timer	 application timer that can be used to get total time and delta time for application
	*/
	void Update(Timer* timer);
	/*
	Renders the given scene

	@param renderTexture	The given render texture that should be used for this scene
	@param device			The directx11 device
	@paran camera			the camera for the scene
	@param depthmap			The Render textures to be used for depths maps from the given lights
	@param lights			The lights within the scene
	*/
	void Render(RenderTexture* renderTexture, D3D* device, Camera* camera, RenderTexture *depthMap[], Light* light[]);

	/*
	The given menu options for the scene
	*/
	void MenuOptions();


	/*
	Will set lights back to correct position in the world
	Used when changing scene so lights so the scene in the best light :D and are light correctly

	@param lights The lights from within the world
	*/
	virtual void ResetLights(Light* lights[]);

	/*
	Does a depth pass on the given scene

	@param device	The directx11 device
	@param camera	The camera within the scene
	@param DepthMap	The depth maps to be created on to
	@param lights   The lights within the scene
	*/
	void GenerateDepthPass(D3D* device, Camera* camera, RenderTexture*depthMap[], Light* light[]);

	/*
	Scene information pop up will tell the user about the current scene
  	*/
	virtual void SceneInformationPopUp(bool* is_open);


	void TerrainSettings(bool *is_open);

	void WaterSettings(bool *is_open);
private:


	bool regenerateSimplexNoise;

	int seed;

	bool waterMenuOpen;

	WavetBufferType* waveInfo;

	WaterShader* waveShader;

	PlaneMesh* waterMesh;

	bool regenerateDiamondSquare;

	DimondSquareBuffer* diamondSquareSettings;

	FractionalBrowningNoiseBuffer* fbnSettings;

	TerrainGenerationBufferType* terrainGeneration;

	TerrainGeneration generationMethod;
	/*
	
	*/
	Texture* lowTexture;

	/*

	*/
	Texture* mediumTexture;

	/*

	*/
	Texture* hightTexture;

	/*
	Depth texture using to create depth map
	*/
	DepthShader* depthShader;

	/*
	Settings used to texture the terrains
	*/
	TerrainSettingTextureType* terrainTextureSettings;

	/*
	Fault line settings
	*/
	FaultLineDisplacementBufferType* faultLineSettings;

	/*
	Regenerate random fault lines
	*/
	bool regenerateFaultLines;

	DistanceBasedTesselation* distanceTess;
	/*
	Terraom Shader
	*/
	TerrainShader* terrainShader;
	/*
	Tesselation settings
	*/
 	TessellationBufferType tessSettings;

	/*
	Model used for debugging purposes
	*/
 	Model* model;
	/*
	Texture shaders
	*/
	TextureShader* textureShader;
	/*
	Basic colour Shader
	*/
	ColourShader* colourShader;
	/*
	Terrain mesh
	*/
	Terrain* terrain;
};

