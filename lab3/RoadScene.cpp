#include "RoadScene.h"



RoadScene::RoadScene(string sceneName)
	: Scene(sceneName)

{
}

RoadScene::~RoadScene()
{
}
void RoadScene::Init(HWND hwnd, ID3D11Device * device, ID3D11DeviceContext * deviceContext, Sound * sound)
{
	Scene::Init(hwnd, device, deviceContext, sound);

	// Create Mesh object
 	colourShader = new ColourShader(device, hwnd); 
 
	std::unordered_map<char, LShapedConstant> constants;

	std::unordered_map<char, std::string> d{
		{ 'F', "F+F-F-F+F" },
 
	};
 
	interations = 5;
 
	 lineMesh = new LineMesh("Line Mesh",device, deviceContext, L"../res/bunny.png");
 	cube = new CubeMesh("Set Cube",device, deviceContext, L"../res/bunny.png");
}



void RoadScene::Update(Timer * timer)
{


 }

void RoadScene::Render(RenderTexture * renderTexture, D3D * device, Camera * camera, RenderTexture * depthMap[], Light * light[])
{
 
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix, baseViewMatrixs;

	// Set the render target to be the render to texture.
	renderTexture->SetRenderTarget(device->GetDeviceContext());

	// Clear the render to texture.
	renderTexture->ClearRenderTarget(device->GetDeviceContext(), 0.0f, 0.0f, 1.0f, 1.0f);


	camera->Update();

	device->GetWorldMatrix(worldMatrix);
	camera->GetViewMatrix(viewMatrix);
	device->GetProjectionMatrix(projectionMatrix);
 
 
	 
	int g = 0;
	  int b = 0;
 
	  lineMesh->SendData(device->GetDeviceContext());
	  colourShader->SetShaderParameters(device->GetDeviceContext(), worldMatrix, viewMatrix, projectionMatrix);
	  colourShader->Render(device->GetDeviceContext(), lineMesh->GetIndexCount());

 
 

	device->SetBackBufferRenderTarget();

}

void RoadScene::MenuOptions()
{
	static bool terrainOptions, soundMenuOpen;

	if (ImGui::BeginMenu("Road options"))
	{
		if (ImGui::MenuItem("Sound Settings"))
		{
			soundMenuOpen = soundMenuOpen ? false : true;

		}
		ImGui::EndMenu();

	}

	sound->GUI_Menu(&soundMenuOpen);

}

void RoadScene::ResetLights(Light * lights[])
{
}

void RoadScene::GenerateDepthPass(D3D * device, Camera * camera, RenderTexture * depthMap[], Light * light[])
{
}

void RoadScene::SceneInformationPopUp(bool * is_open)
{
}
