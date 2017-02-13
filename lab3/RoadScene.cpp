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
	lsystem.loadSystem(constants, d);

	interations = 5;
	 rule = lsystem.recustionValues("F", interations);

 	cube = new CubeMesh(device, deviceContext, L"../res/bunny.png");
}



void RoadScene::Update(Timer * timer)
{
	static float time = 0;
	if (time > 1)
	{
		time = 0;
		//interations++;
	}
	else
	{
		time += timer->GetTime();
	}
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
 	for (auto itr : rule)
	{
 		if (itr == '+')
		{
			a++;
 		//	worldMatrix = worldMatrix *XMMatrixTranslation(0, 10 * a, 0);

 		}
		if (itr == '-')
		{
			a--;

			//worldMatrix = worldMatrix * XMMatrixTranslation(0,10 *a, 0);
		}
		if (itr == 'F')
		{

			float radians =0;



			// Convert degrees to radians.
			radians = 90*a * 0.0174532925f;
			 
			if (g > a)
			{
			worldMatrix =  XMMatrixScaling(0.1, 0.1, 0.1) *  XMMatrixRotationX(90 * a)* XMMatrixTranslation(1 , 0, 0);

			}
			else
			{
				worldMatrix = XMMatrixScaling(0.1, 0.1, 0.1) *  XMMatrixRotationX(90 * a)* XMMatrixTranslation(-1, 0, 0);

			}
		

			//// Send geometry data (from mesh)
			cube->SendData(device->GetDeviceContext());
			//// Set shader parameters (matrices and texture)
			colourShader->SetShaderParameters(device->GetDeviceContext(), worldMatrix, viewMatrix, projectionMatrix);
			//// Render object (combination of mesh geometry and shader process
			colourShader->Render(device->GetDeviceContext(), cube->GetIndexCount());

			g = a;
			b++;
		}
	 }
 
 

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
