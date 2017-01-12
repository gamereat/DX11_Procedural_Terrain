#pragma once
#include "PostProcessingBase.h"
#include "HorizontalBlurShader.h"
#include "VerticalBlurShader.h"

class PostProcessingGaussianBlur :
	public PostProcessingBase
{
public:
	RenderTexture* Render(D3D* directX3D, Camera* camera, OrthoMesh* orthoMesh, RenderTexture* currentRenderTexture);
	void MenuOptions(bool * isOpen);

	PostProcessingGaussianBlur(D3D * directX3D, HWND hwnd, float downscaleValue);
	~PostProcessingGaussianBlur();



private:

	RenderTexture* RenderHoizontalBlur(D3D* directX3D, Camera* camera, OrthoMesh* orthoMesh, RenderTexture* currentRenderTexture);
	RenderTexture* RenderVerticalBlur(D3D* directX3D, Camera* camera, OrthoMesh* orthoMesh, RenderTexture* currentRenderTexture);

	HorizontalBlurShader* horizontalBlurShader;
	VerticalBlurShader* VerticallBlurShader;

	bool isHoizontalEnabled;
	bool isVerticalEnabled;

	float downScaleAmmount;

	int neighboursToUse;

	/*
	Rendering Texture used for the post Proccessing effect
	*/
	RenderTexture* verticalTexture;

	/*
	Rendering Texture used for the post Proccessing effect
	*/
	RenderTexture* hoizontalTexture;
};

