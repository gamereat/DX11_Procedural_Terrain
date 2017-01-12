#pragma once
#include "PostProcessingBase.h"
#include "BoxBlurShader.h"

class PostProccessBoxBlur :
	public PostProcessingBase
{
public:

	RenderTexture* Render(D3D* directX3D, Camera* camera, OrthoMesh* orthoMesh, RenderTexture* currentRenderTexture);
	
	void MenuOptions(bool * isOpen);

	PostProccessBoxBlur(D3D * directX3D, HWND hwnd, float downscaleValue);
	~PostProccessBoxBlur();

private:
	/*
	Box blur render shader 
	Blurs the image using a simple bluring of neighour pixels 
	*/
	BoxBlurShader* boxBurShader;

};

