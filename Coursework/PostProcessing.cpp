#include "PostProcessing.h"
/* See the file "LICENSE" for the full license governing this code. */

PostProcessing::PostProcessing()
{

}


PostProcessing::~PostProcessing()
{
	// Tidy up memory 


	if (orthoMeshDownScaled)
	{
		delete orthoMeshDownScaled;
		orthoMeshDownScaled = nullptr;
	}
 
	if (downScale)
	{
		delete downScale;
		downScale = nullptr;
	}
	if (UpScale)
	{
		delete UpScale;
		UpScale = nullptr;
	}

	if (chromoAberration)
	{
		delete chromoAberration;
		chromoAberration = nullptr;
	}
}

void PostProcessing::Init(D3D* directX3D, HWND hwnd, Timer* timer)
{

	this->timer = timer;

	// Init var 
	// Set the down scale ammount to half the size of the screen
	downScaleAmmount = 2;
	// Set the upscale back the full size of the screen
	upScaleAmmount = 1;


	// Create a downscaled OrthoMesh to be used for each post prcoessing effect
	orthoMeshDownScaled = new OrthoMesh("Down Scale Orth Mesh",directX3D->GetDevice(), directX3D->GetDeviceContext(), ApplicationSettings::screenWidth / downScaleAmmount, ApplicationSettings::screenHeight / downScaleAmmount, 0, 0);

	// Init all the post proccessing effects 

	gaussainBlur = new PostProcessingGaussianBlur(directX3D, hwnd, downScaleAmmount);
	chromoAberration = new PostProcessingChromaticAberration(directX3D, hwnd, downScaleAmmount);
	downScale = new PostProccessingDownScale(directX3D, hwnd, downScaleAmmount);

	UpScale = new PostProccessingUpScale(directX3D, hwnd, upScaleAmmount);

 

}

RenderTexture* PostProcessing::ApplyPostProccessing(OrthoMesh*& orthNormalSized, RenderTexture* lastRenderTexture, D3D* directX3D, Camera* camera)
{
 

	// Create a pointer to the current render texture being used for post processing 
	RenderTexture*  currentRenderTexture = nullptr;
	// Used to keep track of the down scale render texture 
	RenderTexture* downScaleTexture = nullptr;

	// Down scale the scene and keep track of the downscaled texture what is returned 
	downScaleTexture = currentRenderTexture = downScale->Render(directX3D, camera, orthoMeshDownScaled, lastRenderTexture);
 

	currentRenderTexture = chromoAberration->Render(directX3D, camera, orthoMeshDownScaled, downScaleTexture, timer->GetTotalTimePast());
	
	currentRenderTexture = gaussainBlur->Render(directX3D, camera, orthoMeshDownScaled, currentRenderTexture);

 	// Upscale the scene again 
	// Return the render texture so i can be rendered to the scene 
	 return UpScale->Render(directX3D, camera, orthNormalSized, currentRenderTexture);

}


void PostProcessing::PostProccessingMenu()
{

	// Create a menu bar option 
	// each effect will have it own pop out menu which will allow the user to change settings for that effect  
	static bool isGeneralSetting;
 

	if (ImGui::BeginMenu("Post Proccessing Settings"))
	{
		if (ImGui::MenuItem("General Settings"))
		{
			isGeneralSetting = isGeneralSetting ? false : true;
		}
		if (ImGui::MenuItem("chromatic Aberration "))
		{
			chromoAberration->ToggleMenu();
		}

		if (ImGui::MenuItem("Guasaiun blur"))
		{
			gaussainBlur->ToggleMenu();
		}
		ImGui::EndMenu();

	}

	GeneralSettingMenu(&isGeneralSetting);
}


void PostProcessing::GeneralSettingMenu(bool* isOpen)
{
	if (*isOpen == true)
	{
		if (!ImGui::Begin("General PostProsessing effects", isOpen, ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::End();
			return;
		}
			ImGui::Text("General Settting for the post processing effects");


			ImGui::End();

		
	}
}