#pragma once
#include "PostProcessingBase.h"
#include "TextureShader.h"

class PostProccessingUpScale :
	public PostProcessingBase
{
public:

	RenderTexture* Render(D3D* directX3D, Camera* camera, OrthoMesh* orthoMesh, RenderTexture* currentRenderTexture);
	void MenuOptions(bool * isOpen);

	PostProccessingUpScale(D3D * directX3D, HWND hwnd, float downscaleValue);
	~PostProccessingUpScale();



private:
	TextureShader* textureShader;
};

