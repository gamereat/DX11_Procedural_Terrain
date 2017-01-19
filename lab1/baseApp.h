#pragma once
#include "..\DXFramework\BaseApplication.h"
#include "Scene.h"
#include "../DXFramework/baseapplication.h"
#include "D3D.h"
#include "../DXFramework/PointMesh.h"

#include "../DXFramework/PlaneMesh.h"
#include "../DXFramework/SphereMesh.h"
#include "../DXFramework/Light.h"
#include "TextureShader.h"
#include "../DXFramework/OrthoMesh.h"
#include "../DXFramework/RenderTexture.h"
#include "../DXFramework/TessellationMesh.h"
#include "../DXFramework/QuadMesh.h"
#include "../DXFramework/Model.h"
#include "PostProcessing.h"
#include "EmptyScene.h"
#include "TerrainScene.h"
const int NUM_LIGHTS = 4;

class BaseApp :	public BaseApplication
{
public:
	BaseApp();
	~BaseApp();


	/*
	Initializes application

	*/
	void init(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input*);

	/*
	Update function used to update given scene

	@return is it was successful
	*/
	bool Frame();


protected:
	/*
	Renders the given scene
	@return is it was successful
	*/
	bool Render();

	/*
	Adds menu bar option to top of application to expose function to the user
	*/
	void CreateMainMenuBar();


private:

	TerrainScene* terrainScene;
	/*
	The current Scene that is viewed the world
	*/
	Scene* currentScene;

	/*
	Post processing object which creates and adds in post processing effects to scene
	*/
	PostProcessing postPro;

	/*
	Render
	*/
	void RenderToScreen();


	/*
	Lights used within the world all usable at any point
	*/
	Light* lights[NUM_LIGHTS];


	/*
	holds all the depth textures from each light
	*/
	RenderTexture* depthTextures[NUM_LIGHTS];

	/*
	Render texture used to send to a scene
	*/
	RenderTexture* sceneTexture;

	/*
	Texture that has been up scaled to correct size after post processing
	*/
	RenderTexture* upScaleTexture;


	/*
	OrthorMesh that is scaled to correct size
	*/
	OrthoMesh* orthoMeshNormalScaled;

	/*
	Texture shader that will add a texture to a object
	*/
	TextureShader* textureShader;

	/*
	Used to create a empty scene
	*/
	EmptyScene* emptyScene;
};

