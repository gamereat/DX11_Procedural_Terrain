#pragma once
 
#include "../DXFramework/OrthoMesh.h"

#include "../DXFramework/baseapplication.h"
#include "../DXFramework/RenderTexture.h"
#include "TextureShader.h"
#include "BoxBlurShader.h"

#include "../DXFramework/D3D.h"
#include "PostProccessBoxBlur.h"
#include "PostProccessingDownScale.h"
#include "PostProccessingUpScale.h"
#include "PostProcessingGaussianBlur.h"
#include "PostProcessingChromaticAberration.h" 
/*
	Adds any post processing effects to the scene 
	Can enable or disable any during run time using menu system

*/
class PostProcessing
{
public:
	 
	PostProcessing();
	~PostProcessing();

	/*
	Inilizes the post proccessing effects ready to be used 
	@param directX3D	The directx object being used on the scene 
	@param hwnd			The window pointer
	@param timer		Pointer to the window timer to get deltatime from
	*/
	void Init(D3D* directX3D, HWND hwnd, Timer* timer);


	/*
	Adds the activated post proccessing effects to the current screen.
	@param orthNormalSized		A orthMesh of the size of a full size screen
	@param lastRenderTexture	The last render texture shader.
	@param directX3D			Directx object used in scene 
	@param camera				Camera used in the scene 
	*/
	RenderTexture* ApplyPostProccessing(OrthoMesh*& orthNormalSized, RenderTexture* lastRenderTexture ,D3D* directX3D, Camera* camera);

	/*
	The post proccessing menu options for GUI
	*/
	void PostProccessingMenu();
 
private:

	/*
	General post processing effect menu for GUI
	@param isOpen	Is the menu been opened or not 
	*/
	void GeneralSettingMenu(bool* isOpen);
	 

	/////////////////////////////
	// Guassain Blur
	////////////////////////////

 
	/*
	Guassain blur effect
	*/
	PostProcessingGaussianBlur* gaussianBlur;

	/////////////////////////////////////////////
	
	
	/*
	Down scaling post processsing effect
	*/
	PostProccessingDownScale* downScale;

	/*
	Up scale post proccessing effect
	*/
	PostProccessingUpScale* UpScale;
	
	/*
	Box Blur post prosseing effect
	*/
	PostProccessBoxBlur* boxBlur;

	/*
	Adds a chromatic Aberration over the screen
	*/
	PostProcessingChromaticAberration* chromaticAberration;

	/*
	Orthomesh created after the image has been downscaled. Used for each post processsing effect
	*/
	OrthoMesh* orthoMeshDownScaled;
	/*
	How much the image will be downscalled by when doing post proccessing effects
	*/
	float downScaleAmmount;
	/*
	How much the image will be upscade back after the post proccesssing effects
	*/
	float upScaleAmmount;


	/*
	Time for the window
	*/
	Timer* timer;
	
};

