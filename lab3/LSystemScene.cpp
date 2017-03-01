#include "LSystemScene.h"
#include "../imgui/imgui.h"


LSystemScene::LSystemScene(string sceneName)
	: Scene(sceneName)
{
	colourShader = nullptr;
	lSystem = nullptr;
	textureShader= nullptr;


}


 
LSystemScene::~LSystemScene()
{

	// Release the Direct3D object.
	if (textureShader)
	{
		delete textureShader;
		textureShader = nullptr;
	}
	if (lSystem)
	{
		delete lSystem;
		lSystem = nullptr;
	}

	if (colourShader)
	{
		delete colourShader;
		colourShader = nullptr;
	}
	if (planeMesh)
	{
		delete planeMesh;
		planeMesh = nullptr;
	}

	
}

void LSystemScene::Init(HWND hwnd, ID3D11Device * device, ID3D11DeviceContext * deviceContext,Sound* sound)
{
	Scene::Init(hwnd, device, deviceContext, sound);

 
	// init Shaders

  	colourShader = new ColourShader(device, hwnd);
	textureShader = new TextureShader(device, hwnd);

	// init objects

	planeMesh = new PlaneMesh("Dynamic Texture",device, deviceContext, L"../res/bunny.png", 100);
	// Create L-System rules 
	std::unordered_map<char, std::string> rules{
	//	{ 'F',"F+F-F-F+F" },
		{ 'X',"X+YF+" },
		{ 'Y',"-FX-Y" },
	};
	// init L-System
	lSystem = new LSystem(device,14,3, rules);
	updateLSysteam = true;


//	lSystem->loadSystem(rules);


	// Set up base settings for plane to be diplayed on screen in correction line up
	// Will fill screen vetically positioned
	planeScale.x = 0.20;
	planeScale.z = 0.15;
	planeScale.y = 1;

	planeRotate.x = -1.55f;

	planeTranslation.x = -49;
	planeTranslation.y = -7;
	planeTranslation.z = -48;
	planeMesh->setScaleRotationTranslation(planeScale, planeRotate, planeTranslation);


}
void LSystemScene::Update(Timer * timer)
{
 
}
void LSystemScene::Render(RenderTexture * renderTexture, D3D * device, Camera * camera, RenderTexture * depthMap[], Light * light[])
{
  
	// Check if l-system is needing updated 
	if (updateLSysteam)
	{
		updateLSysteam = false;
		lSystem->GenterateUpdateTexture(device->GetDevice());
	}

	XMMATRIX worldMatrix, viewMatrix, projectionMatrix, baseViewMatrixs;

	// Set the render target to be the render to texture.
	renderTexture->SetRenderTarget(device->GetDeviceContext());

	// Clear the render to texture.
	renderTexture->ClearRenderTarget(device->GetDeviceContext(), 0.0f, 0.0f, 1.0f, 1.0f);


	camera->Update();

	device->GetWorldMatrix(worldMatrix);
	camera->GetViewMatrix(viewMatrix);
	device->GetProjectionMatrix(projectionMatrix);
	ID3D11ShaderResourceView* depthMaps[NUM_LIGHTS];

	for (int i = 0; i < NUM_LIGHTS; i++)
	{

		depthMaps[i] = depthMap[i]->GetShaderResourceView();

	}


	//// Send geometry data (from mesh)
	worldMatrix = planeMesh->SendData(device->GetDeviceContext());
	//// Set shader parameters (matrices and texture)
	textureShader->SetShaderParameters(device->GetDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, lSystem->texture);
	//// Render object (combination of mesh geometry and shader process
	textureShader->Render(device->GetDeviceContext(), planeMesh->GetIndexCount());

	device->SetBackBufferRenderTarget();


}

void LSystemScene::MenuOptions()
{
 	static bool lStsytemSettings;


	if (ImGui::BeginMenu("L-System Options"))
	{
		// L-System settings
		if (ImGui::MenuItem("L-System Settings"))
		{
			lStsytemSettings = lStsytemSettings ? false : true;

		}

		if (ImGui::BeginMenu("Objects"))
		{
			if (ImGui::MenuItem(planeMesh->getName().c_str()))
			{
				planeMesh->ToggleMenu();
			}
			ImGui::EndMenu();

		}
		if (ImGui::SmallButton("Refresh L-System"))
		{
			updateLSysteam = true;
		}


		ImGui::EndMenu();

	}
	planeMesh->GuiSettings(&planeMesh->menuOpen);
	lSystem->Gui_menu(&lStsytemSettings);

}

void LSystemScene::ResetLights(Light * lights[])
{
	// Set the light light to show white 
	lights[0]->SetDiffuseColour(1, 1, 1, 1);
}

void LSystemScene::GenerateDepthPass(D3D * device, Camera * camera, RenderTexture * depthMap[], Light * light[])
{
}

void LSystemScene::SceneInformationPopUp(bool * is_open)
{

}

 