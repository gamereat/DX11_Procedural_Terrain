#pragma once
#include "PostProcessingBase.h"
#include "ChromaticAberrationsShader.h"

class PostProcessingChromaticAberration :
	public PostProcessingBase
{
public:
	PostProcessingChromaticAberration(D3D * directX3D, HWND hwnd, float downscaleValue);
 	~PostProcessingChromaticAberration();

	RenderTexture* Render(D3D* directX3D, Camera* camera, OrthoMesh* orthoMesh, RenderTexture* currentRenderTexture);
	RenderTexture* Render(D3D* directX3D, Camera* camera, OrthoMesh* orthoMesh, RenderTexture* currentRenderTexture,float deltaTime);
	void MenuOptions(bool * isOpen);

private:
	ChromaticAberrationsShader* chromaticAberrationsShader;

	ChromaticAberrationsShader::ChromaticAberrationsBufferType ChromaticAberrationsShaderInfo;
};

