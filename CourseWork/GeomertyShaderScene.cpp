#include "GeomertyShaderScene.h"
#include "VertexShader.h"


#include "../DXFramework/BaseApplication.h"

 
GeomertyShaderScene::GeomertyShaderScene(std::string sceneName)
	: Scene(sceneName)

{
	firstTimeExplotion = false;
	geomentryShader = nullptr;
	teaTop = nullptr;
}

GeomertyShaderScene::~GeomertyShaderScene()
{
 	if (geomentryShader)
	{
		delete geomentryShader;
		geomentryShader = nullptr;
	}
	if (teaTop)
	{
		delete teaTop;
		teaTop = nullptr;
	}
}

void GeomertyShaderScene::Init(HWND hwnd, ID3D11Device * device, ID3D11DeviceContext * deviceContext)
{
	Scene::Init(hwnd, device, deviceContext);

	geomentryShader = new GeomentryShader(device, hwnd);

 
 	teaTop = new Model(device, deviceContext, L"I don't want a texture please ", L"../res/teapot.obj");

 
	// set default values for explotion
	expSettings.explosiveAcceleration = 1;
	expSettings.explosiveAmmount = 1;
	expSettings.gravity = -9.8;
	expSettings.maxTime = 5;
}

void GeomertyShaderScene::Update(Timer* timer)
{
	// if user hasn't exploeded tea for the first time the don't add to time

	if (firstTimeExplotion)
	{
		expSettings.time += timer->GetTime();
	}
}

void GeomertyShaderScene::Render(RenderTexture * renderTexture, D3D * device, Camera * camera, RenderTexture * depthMap[], Light * light[])
{
 
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix, baseViewMatrix, orthoMartix;

	//m_Direct3D->BeginScene(0.39f, 0.58f, 0.92f, 1.0f);
	renderTexture->SetRenderTarget(device->GetDeviceContext());

	renderTexture->ClearRenderTarget(device->GetDeviceContext(), 0.0f, 0.0f, 1.0f, 1.0f);

	//// Generate the view matrix based on the camera's position.
	camera->Update();

	//// Get the world, view, projection, and ortho matrices from the camera and Direct3D objects.
	device->GetWorldMatrix(worldMatrix);
	camera->GetViewMatrix(viewMatrix);
	device->GetProjectionMatrix(projectionMatrix);

	worldMatrix = XMMatrixScaling(0.1, 0.1, 0.1);
	 
	// Send geometry data (from mesh)
	teaTop->SendData(device->GetDeviceContext());

	geomentryShader->SetShaderParameters(device->GetDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, teaTop->GetTexture(), expSettings);

	geomentryShader->Render(device->GetDeviceContext(), teaTop->GetIndexCount());


	worldMatrix = XMMatrixTranslation(-50, -10, -50);
	  
	device->SetBackBufferRenderTarget();
}

void GeomertyShaderScene::ResetLights(Light * lights[])
{ 
}

void GeomertyShaderScene::MenuOptions()
{
	std::string menuName;
	menuName.append(sceneName);
	menuName.append(" Settings");
	if (ImGui::BeginMenu(menuName.c_str()))
	{
		if (ImGui::MenuItem("Scene Information "))
		{
			isInformationPopupOpen = isInformationPopupOpen ? false : true;
		}
		if (ImGui::MenuItem("Explosion effects "))
		{
			isExplotionMenuOpen = isExplotionMenuOpen ? false : true;
		}
 



		ImGui::EndMenu();
	}


	SceneInformationPopUp(&isInformationPopupOpen);
	explotionMenu(&isExplotionMenuOpen);


}

void GeomertyShaderScene::GenerateDepthPass(D3D * device, Camera * camera, RenderTexture * depthMap[], Light * lights[])
{
	
	// NO DEPTH PASS NEEDED AS NO SHADOWS WITHIN THIS SCENE 
}

void GeomertyShaderScene::explotionMenu(bool * is_open)
{
	if (*is_open == true)
	{
		if (!ImGui::Begin("Explosion ", is_open, ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::End();
			return;
		}
		 
		ImGui::Text("Change Explosive setting bellow \nNote explotion will not loop as a feature and will stop at the end of the max time\n to complete a new set of settings press button again\nIt will then simulate explotion again ");
		ImGui::SliderFloat("Explosion Velocity", &expSettings.explosiveAmmount,0,15);
		ImGui::SliderFloat("Maximum explosion time", &expSettings.maxTime,0,25);
		ImGui::SliderFloat("Gravity scale", &expSettings.gravity,-15,15);
		ImGui::SliderFloat("Explosive acceleration", &expSettings.explosiveAcceleration, 0, 15);
 
		if (ImGui::Button("Explode Teapot"))
		{
			firstTimeExplotion = true;
			expSettings.time = 0;
		}
		ImGui::End();
	}
}

void GeomertyShaderScene::SceneInformationPopUp(bool * is_open)
{
}
