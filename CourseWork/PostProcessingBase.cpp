#include "PostProcessingBase.h"



PostProcessingBase::PostProcessingBase(D3D * directX3D, HWND hwnd, float downscaleValue)
{	
	renderTexture = new RenderTexture(directX3D->GetDevice(), SCREEN_WIDTH / downscaleValue, SCREEN_HEIGHT / downscaleValue, SCREEN_NEAR, SCREEN_DEPTH);

}


PostProcessingBase::~PostProcessingBase()
{
}

void PostProcessingBase::ToggleMenu()
{
	isMenuOpen = isMenuOpen ? false : true;
}

void PostProcessingBase::PreRender(D3D * directX3D, Camera * camera, XMMATRIX & worldMatrix, XMMATRIX & baseViewMatrix, XMMATRIX & orthoMartix, OrthoMesh* orthoMesh)
{

	XMMATRIX viewMatrix, projectionMatrix ;

	// Set the render target to be the render to texture.
	renderTexture->SetRenderTarget(directX3D->GetDeviceContext());

	// Clear the render to texture.
	renderTexture->ClearRenderTarget(directX3D->GetDeviceContext(), 0.0f, 0.0f, 0.0f, 1.0f);

	directX3D->GetWorldMatrix(worldMatrix);
	camera->GetViewMatrix(viewMatrix);
	directX3D->GetProjectionMatrix(projectionMatrix);

	// To render ortho mesh
	// Turn off the Z buffer to begin all 2D rendering.
	directX3D->TurnZBufferOff();

	orthoMartix = renderTexture->GetOrthoMatrix();// ortho matrix for 2D rendering
	camera->GetBaseViewMatrix(baseViewMatrix);

	orthoMesh->SendData(directX3D->GetDeviceContext());



}

void PostProcessingBase::PostRender(D3D * directX3D)
{

	directX3D->TurnZBufferOn();

	directX3D->SetBackBufferRenderTarget();

	MenuOptions(&isMenuOpen);
}
 