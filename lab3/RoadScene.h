#pragma once
#include "Scene.h"
#include "Scene.h"
#include "../DXFramework/SphereMesh.h"
#include "../DXFramework/CubeMesh.h"
#include "../DXFramework/TessellationMesh.h"
#include "../DXFramework/QuadMesh.h"
#include "../DXFramework/PointMesh.h"
#include "../DXFramework/DoubleTriangleMesh.h"
#include "../DirectXTK/Inc/PrimitiveBatch.h"
#include "../DirectXTK/Inc/VertexTypes.h"
#include "../DirectXTK/Inc/Effects.h"
#include "../DirectXTK/Inc/CommonStates.h"
#include "../DirectXTK/Inc/DirectXHelpers.h"

#include "ColourShader.h"
#include "Terrain.h"
#include "../DXFramework/CubeMesh.h"
#include "TextureShader.h"
#include "DistanceBasedTesselation.h"
#include "../DXFramework/Model.h"
#include "Sound.h"
#include "LineMesh.h"
 #include "LSystem.h"
using namespace DirectX;

class RoadScene :
	public Scene
{
public:
	RoadScene(string sceneName);
	~RoadScene();


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

private:
	LineMesh* lineMesh;
	LineMesh* lineMesh2;
	 int a;
 	ColourShader* colourShader;
	std::string rule;
	CubeMesh* cube;
	int interations;
	std::unique_ptr<PrimitiveBatch< VertexPositionColor>> primitiveBatch;
	std::vector<PointMesh*> points;
};

