#pragma once
#include "PostProcessingBase.h"
#include "TextureShader.h"

/*
Post Proccessing upscaling scaling

@Author Alan Yeats

upscales texture used after post processing effects
*/
class PostProccessingUpScale :
	public PostProcessingBase
{
public:

	/*
	Renderes the current scene

	@param directX3D				Directx Object used for given window
	@param camera					Camera used in scene
	@param orthoMesh				Orthomesh used for post Processing effect
	@param currentRenderTexture		The current render texture being used
	*/
	RenderTexture* Render(D3D* directX3D, Camera* camera, OrthoMesh* orthoMesh, RenderTexture* currentRenderTexture);

	/*
	Menu options for given effect

	@param isOpen	if the option menu is open or not
	*/
	void MenuOptions(bool * isOpen);

	PostProccessingUpScale(D3D * directX3D, HWND hwnd, float downscaleValue);
	~PostProccessingUpScale();



private:
	/*
	Texture shader
	*/
	TextureShader* textureShader;
};

