#pragma once
#include "..\DXFramework\BaseApplication.h"
#include "D3D.h"
#include "../DXFramework/Light.h"
#include "../DXFramework/OrthoMesh.h"
#include "../DXFramework/RenderTexture.h"
#include "PostProcessing.h"
#include "EmptyScene.h"
#include "TerrainScene.h"
#include "LSystemScene.h"
#include "../DXFramework/Model.h"
#include "ShaderBuffers.h"
#include "Sound.h"
#include "TextureShader.h"
#include "../OpenVR/openvr.h"
/*
*/

/*
* Base app 
*
* @author      Alan Yeats
*
* Base application used to host given scenes
*/
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
	
	vr::IVRSystem *pHMD;
	vr::IVRRenderModels * pRenderModels;

	vr::TrackedDevicePose_t rTrackedDevicePose[vr::k_unMaxTrackedDeviceCount];
	char rDevClassChar[vr::k_unMaxTrackedDeviceCount];   // for each device, a character representing its class
	XMMATRIX rmat4DevicePose[vr::k_unMaxTrackedDeviceCount];
	XMMATRIX mat4HMDPose;
	int iValidPoseCount;
	std::string strPoseClasses;                            // what classes we saw poses for this frame

	RenderTexture leftEye;
	RenderTexture rightEye;



	/*
	L-System Scene used to show a 2D l-system on a dynamic texture
	*/
	LSystemScene* lSystemScene;
	
	/*
	Terrain scene used to show procedural generated terrain
	*/
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
	Render to the screen
	*/
	void RenderToScreen();

	void UpdateHMDMatrixPose();
	
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

	/*
	Sound system used across the application
	*/
	Sound* sound;
	
	XMMATRIX ConvertSteamVRMatrixToXMMartix(const vr::HmdMatrix34_t & matPose);

};

