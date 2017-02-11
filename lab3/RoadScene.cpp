#include "RoadScene.h"



RoadScene::RoadScene(string sceneName)
	: Scene(sceneName)

{
}


RoadScene::~RoadScene()
{
}
void RoadScene::Init(HWND hwnd, ID3D11Device * device, ID3D11DeviceContext * deviceContext)
{
	Scene::Init(hwnd, device, deviceContext);

	// Create Mesh object
 	colourShader = new ColourShader(device, hwnd); 
	sound = new Sound();

	sound->Init(L"../res/BlownAway.mp3");

	std::unordered_map<char, LShapedConstant> constants;

	std::unordered_map<char, std::string> d{
		{ 'A', "AB" },
		{ 'B', "A" }

	};
	lsystem.loadSystem(constants, d);

	std::string f = lsystem.recustionValues("A-B", 3);
		
	for (int i = 0;i < 10; i++)
	{
		 
			PointMesh * sm = new PointMesh(device, deviceContext, L"../res/bunny.png");
		points.push_back(sm);
	}

}

void RoadScene::Update(Timer * timer)
{
	static float time = 0;
	if (time > 10)
	{
		interations++;
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

	interations = 4;
		//	worldMatrix = XMMatrixScaling(0.1, 0.1, 0.1);

	std::string rule = lsystem.recustionValues("A", interations);
	int a = 0;
	int b = 0;
	for (auto itr : rule)
	{
 		if (itr == 'B')
		{
			b++;
			worldMatrix = XMMatrixTranslation(-100*b, 0, 0);

 		}
		if (itr == '-')
		{
			worldMatrix = XMMatrixTranslation(0, -100, 0);
		}
		if (itr == 'A')
		{				
			a++;

			points[a]->SendData(device->GetDeviceContext());
			colourShader->SetShaderParameters(device->GetDeviceContext(), worldMatrix, viewMatrix, projectionMatrix);
			colourShader->Render(device->GetDeviceContext(), points[a]->GetIndexCount());
		}

		if (a >= 10)
		{
			a = 9;
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
