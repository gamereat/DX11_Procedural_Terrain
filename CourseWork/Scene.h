#pragma once
#include "../DXFramework/Light.h"
#include "../DXFramework/RenderTexture.h"
#include "../DXFramework/D3D.h"
#include "../DXFramework/Camera.h"
#include "DepthShader.h"
#include "../DXFramework/Timer.h"
/*
A base scene interface to allow for different scene to be loaded
*/
class Scene
{
public:

	/*
	@param sceneName		Name for the given scene
	*/
	Scene(std::string sceneName);
	~Scene();
	
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
	virtual void Update(Timer* timer) = 0;

	/*
	Renders the given scene

	@param renderTexture	The given render texture that should be used for this scene 
	@param device			The directx11 device 
	@paran camera			the camera for the scene
	@param depthmap			The Render textures to be used for depths maps from the given lights 
	@param lights			The lights within the scene 
	*/
	virtual void Render(RenderTexture* renderTexture, D3D* device, Camera* camera, RenderTexture *depthMap[], Light* light[]) = 0;


	/*
	Will set lights back to correct position in the world
	Used when changing scene so lights so the scene in the best light :D and are light correctly 

	@param lights The lights from within the world 
	*/
	virtual void ResetLights(Light* lights[])  =0 ;

	/*
	The given menu options for the scene 
	*/
	virtual void MenuOptions() = 0;

	/*
	
	@returm The scene name 
	*/
	std::string getSceneName();
 
	/*
	If the scene is current viewable
	*/
	bool isEnbaled;


	
protected:

	/*
	Does a depth pass on the given scene 

	@param device	The directx11 device 
	@param camera	The camera within the scene
	@param DepthMap	The depth maps to be created on to
	@param lights   The lights within the scene 
	*/
	virtual void GenerateDepthPass(D3D* device, Camera* camera, RenderTexture*depthMap[], Light* lights[]) = 0;



	/*
	Scene information pop up will tell the user about the current scene 
	*/
	virtual void SceneInformationPopUp(bool* is_open) =0;

	/*
	Depth shader used to generate our depth maps
	*/
	DepthShader* depthShader;

	/*
	@deprecated 
	*/
	RenderTexture*  depthTexture;


	/*
	The name of the scene
	*/
	std::string sceneName;


	/*
	if the information pop up will be open
	*/
	bool isInformationPopupOpen;

};

