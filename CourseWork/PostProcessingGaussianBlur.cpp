#include "PostProcessingGaussianBlur.h"

 
PostProcessingGaussianBlur::PostProcessingGaussianBlur(D3D * directX3D, HWND hwnd, float downscaleValue)
	: PostProcessingBase(directX3D, hwnd, downscaleValue)

{
	horizontalBlurShader = new HorizontalBlurShader(directX3D->GetDevice(), hwnd);
	VerticallBlurShader = new VerticalBlurShader(directX3D->GetDevice(), hwnd);
	this->downScaleAmmount = downscaleValue;

	verticalTexture = new RenderTexture(directX3D->GetDevice(), SCREEN_WIDTH / downscaleValue, SCREEN_HEIGHT / downscaleValue, SCREEN_NEAR, SCREEN_DEPTH);
	
	hoizontalTexture = new RenderTexture(directX3D->GetDevice(), SCREEN_WIDTH / downscaleValue, SCREEN_HEIGHT / downscaleValue, SCREEN_NEAR, SCREEN_DEPTH);
		isHoizontalEnabled = true;
	isVerticalEnabled = true;
}


PostProcessingGaussianBlur::~PostProcessingGaussianBlur()
{
}

RenderTexture * PostProcessingGaussianBlur::RenderHoizontalBlur(D3D * directX3D, Camera * camera, OrthoMesh * orthoMesh, RenderTexture * currentRenderTexture)
{
	if (isHoizontalEnabled)
	{
		XMMATRIX worldMatrix, baseViewMatrix, orthoMartix;


		XMMATRIX viewMatrix, projectionMatrix;

		// Set the render target to be the render to texture.
		hoizontalTexture->SetRenderTarget(directX3D->GetDeviceContext());

		// Clear the render to texture.
		hoizontalTexture->ClearRenderTarget(directX3D->GetDeviceContext(), 0.0f, 0.0f, 0.0f, 1.0f);

		directX3D->GetWorldMatrix(worldMatrix);
		camera->GetViewMatrix(viewMatrix);
		directX3D->GetProjectionMatrix(projectionMatrix);

		// To render ortho mesh
		// Turn off the Z buffer to begin all 2D rendering.
		directX3D->TurnZBufferOff();

		orthoMartix = hoizontalTexture->GetOrthoMatrix();// ortho matrix for 2D rendering
		camera->GetBaseViewMatrix(baseViewMatrix);

		orthoMesh->SendData(directX3D->GetDeviceContext());

		horizontalBlurShader->SetShaderParameters(directX3D->GetDeviceContext(), worldMatrix, baseViewMatrix, orthoMartix, currentRenderTexture->GetShaderResourceView(), SCREEN_WIDTH / downScaleAmmount, neighboursToUse);
		horizontalBlurShader->Render(directX3D->GetDeviceContext(), orthoMesh->GetIndexCount());


	}


	directX3D->TurnZBufferOn();

	directX3D->SetBackBufferRenderTarget();

	if (isHoizontalEnabled)
	{
		return hoizontalTexture;
	}
	else
	{
		return currentRenderTexture;

	}
}

RenderTexture * PostProcessingGaussianBlur::RenderVerticalBlur(D3D * directX3D, Camera * camera, OrthoMesh * orthoMesh, RenderTexture * currentRenderTexture)
{
	if (isVerticalEnabled)
	{
		XMMATRIX worldMatrix, baseViewMatrix, orthoMartix;

		XMMATRIX viewMatrix, projectionMatrix;

		// Set the render target to be the render to texture.
		verticalTexture->SetRenderTarget(directX3D->GetDeviceContext());

		// Clear the render to texture.
		verticalTexture->ClearRenderTarget(directX3D->GetDeviceContext(), 0.0f, 0.0f, 0.0f, 1.0f);

		directX3D->GetWorldMatrix(worldMatrix);
		camera->GetViewMatrix(viewMatrix);
		directX3D->GetProjectionMatrix(projectionMatrix);

		// To render ortho mesh
		// Turn off the Z buffer to begin all 2D rendering.
		directX3D->TurnZBufferOff();

		orthoMartix = verticalTexture->GetOrthoMatrix();// ortho matrix for 2D rendering
		camera->GetBaseViewMatrix(baseViewMatrix);

		orthoMesh->SendData(directX3D->GetDeviceContext());

		VerticallBlurShader->SetShaderParameters(directX3D->GetDeviceContext(), worldMatrix, baseViewMatrix, orthoMartix, currentRenderTexture->GetShaderResourceView(), SCREEN_WIDTH / downScaleAmmount, neighboursToUse);
		VerticallBlurShader->Render(directX3D->GetDeviceContext(), orthoMesh->GetIndexCount());


	}


	directX3D->TurnZBufferOn();

	directX3D->SetBackBufferRenderTarget();

	if (isVerticalEnabled)
	{
		return verticalTexture;
	}
	else
	{
		return currentRenderTexture;

	}
}

RenderTexture* PostProcessingGaussianBlur::Render(D3D* directX3D, Camera* camera, OrthoMesh* orthoMesh, RenderTexture* currentRenderTexture)
{
	if (isEnabled)
	{
		currentRenderTexture =	RenderVerticalBlur(directX3D, camera, orthoMesh, currentRenderTexture);
		renderTexture = RenderHoizontalBlur(directX3D, camera, orthoMesh, currentRenderTexture);
	}


	MenuOptions(&isMenuOpen);

	if (isEnabled)
	{
		return renderTexture;
	}
	else
	{
		return currentRenderTexture;
	}

}

void PostProcessingGaussianBlur::MenuOptions(bool * isOpen)
{
	if (*isOpen == true)
	{

		// Create the window
		if (!ImGui::Begin("Gaussian Blur settings ", isOpen, ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::End();
			return;
		}
		ImGui::Checkbox("Enable Gaussian Blur", &isEnabled);

		if (isEnabled)
		{
			ImGui::Checkbox("Enable Horizontal Blur", &isHoizontalEnabled);
			ImGui::Checkbox("Enable Vertical Blur", &isVerticalEnabled);

		 
		}
		ImGui::End();
	}
}
