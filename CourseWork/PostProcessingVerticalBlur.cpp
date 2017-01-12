#include "PostProcessingVerticalBlur.h"


PostProcessingVerticalBlur::PostProcessingVerticalBlur(D3D * directX3D, HWND hwnd, float downscaleValue)
	: PostProcessingBase(directX3D, hwnd, downscaleValue)
{
	verticalBlurShader = new VerticalBlurShader(directX3D->GetDevice(), hwnd);
	this->downScaleAmmount = downscaleValue;
}

PostProcessingVerticalBlur::~PostProcessingVerticalBlur()
{
	if (verticalBlurShader)
	{
		delete verticalBlurShader;
		verticalBlurShader = nullptr;
	}
}



RenderTexture * PostProcessingVerticalBlur::Render(D3D * directX3D, Camera * camera, OrthoMesh * orthoMesh, RenderTexture * currentRenderTexture)
{
	if (isEnabled)
	{
		XMMATRIX worldMatrix, baseViewMatrix, orthoMartix;

		PreRender(directX3D, camera, worldMatrix, baseViewMatrix, orthoMartix, orthoMesh);

		verticalBlurShader->SetShaderParameters(directX3D->GetDeviceContext(), worldMatrix, baseViewMatrix, orthoMartix, currentRenderTexture->GetShaderResourceView(), SCREEN_HEIGHT / downScaleAmmount, neighboursToUse);
		verticalBlurShader->Render(directX3D->GetDeviceContext(), orthoMesh->GetIndexCount());


	}

	PostRender(directX3D);


	if (isEnabled)
	{
		return renderTexture;
	}
	else
	{
		return currentRenderTexture;

	}
}

void PostProcessingVerticalBlur::MenuOptions(bool * isOpen)
{
	if (*isOpen == true)
	{

		// Create the window
		if (!ImGui::Begin("Vertical Blur settings ", isOpen, ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::End();
			return;
		}
		ImGui::Checkbox("Enable Vertical Blur", &isEnabled);

		if (isEnabled)
		{

			if (ImGui::DragInt("Neightbours to check", &neighboursToUse, 0.05f, 0, 5))
			{
			}
		}
		ImGui::End();
	}
}
