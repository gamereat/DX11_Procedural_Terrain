#include "PostProcessingGaussianBlur.h"

 
PostProcessingGaussianBlur::PostProcessingGaussianBlur(D3D * directX3D, HWND hwnd, float downscaleValue)
	: PostProcessingBase(directX3D, hwnd, downscaleValue)

{
	horizontalBlurShader = new HorizontalBlurShader(directX3D->GetDevice(), hwnd);
	this->downScaleAmmount = downscaleValue;


}


PostProcessingGaussianBlur::~PostProcessingGaussianBlur()
{
}

RenderTexture* PostProcessingGaussianBlur::Render(D3D* directX3D, Camera* camera, OrthoMesh* orthoMesh, RenderTexture* currentRenderTextureh)
{
	if (isEnabled)
	{
		XMMATRIX worldMatrix, baseViewMatrix, orthoMartix;

		PreRender(directX3D, camera, worldMatrix, baseViewMatrix, orthoMartix, orthoMesh);

		horizontalBlurShader->SetShaderParameters(directX3D->GetDeviceContext(), worldMatrix, baseViewMatrix, orthoMartix, currentRenderTextureh->GetShaderResourceView(), SCREEN_WIDTH / downScaleAmmount, neighboursToUse);
		horizontalBlurShader->Render(directX3D->GetDeviceContext(), orthoMesh->GetIndexCount());


	}

	PostRender(directX3D);


	if (isEnabled)
	{
		return renderTexture;
	}
	else
	{
		return currentRenderTextureh;

	}


}

void PostProcessingGaussianBlur::MenuOptions(bool * isOpen)
{
	if (*isOpen == true)
	{

		// Create the window
		if (!ImGui::Begin("Horizontal Blur settings ", isOpen, ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::End();
			return;
		}
		ImGui::Checkbox("Enable Horizontal Blur", &isEnabled);

		if (isEnabled)
		{

			if (ImGui::DragInt("Neightbours to check", &neighboursToUse, 0.05f, 0, 5))
			{
			}
		}
		ImGui::End();
	}
}
