#include "PostProcessingChromaticAberration.h"



PostProcessingChromaticAberration::PostProcessingChromaticAberration(D3D * directX3D, HWND hwnd, float downscaleValue)
	: PostProcessingBase(directX3D, hwnd, downscaleValue)
{
 	chromaticAberrationsShader = new ChromaticAberrationsShader(directX3D->GetDevice(), hwnd);

	ChromaticAberrationsShaderInfo.freqancy = XMFLOAT3(0.55f, 0.3f, 0.0f);
	ChromaticAberrationsShaderInfo.rgbDistortLevel = XMFLOAT3(.21, .14, 0);
 }


PostProcessingChromaticAberration::~PostProcessingChromaticAberration()
{
}


RenderTexture * PostProcessingChromaticAberration::Render(D3D * directX3D, Camera * camera, OrthoMesh * orthoMesh, RenderTexture * currentRenderTexture)
{
	return nullptr;
}

RenderTexture * PostProcessingChromaticAberration::Render(D3D * directX3D, Camera * camera, OrthoMesh * orthoMesh, RenderTexture * currentRenderTexture,float deltaTime)
{
	// If the effect has been enabled 
	if (isEnabled)
	{
		XMMATRIX worldMatrix, baseViewMatrix, orthoMartix;

		PreRender(directX3D, camera, worldMatrix, baseViewMatrix, orthoMartix, orthoMesh);

		// Apply shaders 
		ChromaticAberrationsShaderInfo.totalTime = deltaTime;
		chromaticAberrationsShader->SetShaderParameters(directX3D->GetDeviceContext(), worldMatrix, baseViewMatrix, orthoMartix, currentRenderTexture->GetShaderResourceView(), ChromaticAberrationsShaderInfo);
		chromaticAberrationsShader->Render(directX3D->GetDeviceContext(), orthoMesh->GetIndexCount());

	}


	PostRender(directX3D);

	// if the effect is being used then return the used render texture if not pass though the last texture used 
	if (isEnabled)
	{
		return renderTexture;
	}
	else
	{
		return currentRenderTexture;

	}
}

void PostProcessingChromaticAberration::MenuOptions(bool * isOpen)
{
	if (*isOpen == true)
	{

		// Create the window
		if (!ImGui::Begin("Chromatic Aberration Settings ", isOpen, ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::End();
			return;
		}
		ImGui::Checkbox("Enable Chromatic Aberration ", &isEnabled);

		if (isEnabled)
		{ 
			ImGui::DragFloat3("Disort Frecensy", &ChromaticAberrationsShaderInfo.freqancy.x, 0.05f, 0.0f, 10.0f);
			ImGui::ColorEdit3("Colour Disort ", &ChromaticAberrationsShaderInfo.rgbDistortLevel.x);
		
		}
		ImGui::End();
	}
}