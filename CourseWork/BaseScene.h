#pragma once
#include "Scene.h"
#include "../DXFramework/DoubleTriangleMesh.h"
#include "ColourShader.h"
class BaseScene :
	public Scene
{
public:

	BaseScene(std::string sceneName);
	~BaseScene();

	/*
	Initializes the given Scene

	@param hwnd		A handle to the window
	@param device	The directx11 device
	@param deviceContext	The directx11 device Context

	*/
	virtual void Init(HWND hwnd, ID3D11Device* device, ID3D11DeviceContext* deviceContext);

	/*
	Updates the current scene

	@param timer	 application timer that can be used to get total time and delta time for application
	*/
	virtual void Update(Timer* timer);

	/*
	Renders the given scene

	@param renderTexture	The given render texture that should be used for this scene
	@param device			The directx11 device
	@paran camera			the camera for the scene
	@param depthmap			The Render textures to be used for depths maps from the given lights
	@param lights			The lights within the scene
	*/
	virtual void Render(RenderTexture* renderTexture, D3D* device, Camera* camera, RenderTexture *depthMap[], Light* light[]) ;


	/*
	Will set lights back to correct position in the world
	Used when changing scene so lights so the scene in the best light :D and are light correctly

	@param lights The lights from within the world
	*/
	virtual void ResetLights(Light* lights[]) ;

	/*
	The given menu options for the scene
	*/
	virtual void MenuOptions() ;


protected:

	ColourShader* m_ColourShader;
	DoubleTriangleMesh* m_Mesh;

	/*
	Does a depth pass on the given scene

	@param device	The directx11 device
	@param camera	The camera within the scene
	@param DepthMap	The depth maps to be created on to
	@param lights   The lights within the scene
	*/
	virtual void GenerateDepthPass(D3D* device, Camera* camera, RenderTexture*depthMap[], Light* lights[]) ;



	/*
	Scene information pop up will tell the user about the current scene
	*/
	virtual void SceneInformationPopUp(bool* is_open) ;
};

