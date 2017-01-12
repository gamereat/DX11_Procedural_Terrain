#pragma once
#include "PostProcessingBase.h"
#include "TextureShader.h"
class PostProccessingDownScale :
	public PostProcessingBase
{
public:


	RenderTexture* Render(D3D* directX3D, Camera* camera, OrthoMesh* orthoMesh, RenderTexture* currentRenderTexture);
	void MenuOptions(bool * isOpen);


	PostProccessingDownScale(D3D * directX3D, HWND hwnd, float downscaleValue);
	~PostProccessingDownScale();

private:
	TextureShader* textureShader;

};

