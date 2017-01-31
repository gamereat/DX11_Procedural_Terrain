#include "PostProccessingUpScale.h"



PostProccessingUpScale::PostProccessingUpScale(D3D * directX3D, HWND hwnd, float downscaleValue)
	: PostProcessingBase(directX3D, hwnd, downscaleValue)

{
	textureShader = new TextureShader(directX3D->GetDevice(), hwnd);
	isEnabled = true;
}


PostProccessingUpScale::~PostProccessingUpScale()
{
}

RenderTexture* PostProccessingUpScale::Render(D3D* directX3D, Camera* camera, OrthoMesh* orthoMesh, RenderTexture* currentRenderTextureh)
{
	if (isEnabled)
	{
		XMMATRIX worldMatrix, baseViewMatrix, orthoMartix;

		PreRender(directX3D, camera, worldMatrix, baseViewMatrix, orthoMartix, orthoMesh);

		textureShader->SetShaderParameters(directX3D->GetDeviceContext(), worldMatrix, baseViewMatrix, orthoMartix, currentRenderTextureh->GetShaderResourceView());
		textureShader->Render(directX3D->GetDeviceContext(), orthoMesh->GetIndexCount());

	}

	PostRender(directX3D);


	return renderTexture;
}

void PostProccessingUpScale::MenuOptions(bool * isOpen)
{
	if (*isOpen == true)
	{

		// Create the window
		if (!ImGui::Begin("up Scale Settings ", isOpen, ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::End();
			return;
		}

		ImGui::Text("ff");

		ImGui::End();
	}
}
