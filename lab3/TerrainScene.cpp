#include "TerrainScene.h"
#include "../imgui/imgui.h"
#include "../ApplicationSettings.h"
#include <random>

TerrainScene::TerrainScene(string sceneName)
	: Scene(sceneName)
{
	terrain = nullptr;
	colourShader = nullptr;

	regenerateFaultLines = true;
}


 
TerrainScene::~TerrainScene()
{

	// Release the Direct3D object.
	if (terrain)
	{
		delete terrain;
		terrain = 0;
	}

	if (colourShader)
	{
		delete colourShader;
		colourShader = 0;
	}


	
}

void TerrainScene::Init(HWND hwnd, ID3D11Device * device, ID3D11DeviceContext * deviceContext,Sound* sound)
{
	Scene::Init(hwnd, device, deviceContext, sound);

	// Create Mesh object
	terrain = new Terrain("Terrain",device, deviceContext, L"../res/bunny.png");
 	terrain->InitializeTerrain(device, 100, 100);
  	colourShader = new ColourShader(device, hwnd);
	textureShader = new TextureShader(device, hwnd);
 //	model = new Model(device, deviceContext, L"../res/bunny.png", L"../res/teapot.obj");
//	dbtShader = new DistanceBasedTesselation(device, hwnd);

	terrainShader = new TerrainShader(device,hwnd);
	//sound->Init(L"../res/BlownAway.mp3");
 

	depthShader = new DepthShader(device, hwnd);
	faultLineSettings = new FaultLineDisplacementBufferType();

	distanceTess = new DistanceBasedTesselation(device, hwnd);

	terrainTextureSettings = new TerrainSettingTextureType();
	tessSettings.maxDistance = 10;
	tessSettings.maxTesselationAmmount = 6;
	tessSettings.minDistance = 1;
	tessSettings.minTesselationAmmount = 1;


	faultLineSettings->numberOfIterations = 1;
	faultLineSettings->startingDisplacement = 0.5f;

	terrain->setTranslation(XMFLOAT3(-25, -25, 50));



	lowTexture = new Texture(device, deviceContext, L"../res/snow.png");
	mediumTexture = new Texture(device, deviceContext, L"../res/stone.png");
	hightTexture = new Texture(device, deviceContext, L"../res/rock_solid_rugged_01_M.png");



 }
void TerrainScene::Update(Timer * timer)
{



	if (faultLineSettings->enableFaultLineDisplacement && sound->getData(BASS_DATA_FFT256 | BASS_DATA_FLOAT, 128)[1] > 0.4f)
	{
 
		faultLineSettings->numberOfIterations++;
	}


	// If needing to genereat rnadom numbers 
	if (regenerateFaultLines)
	{
		regenerateFaultLines = false;
		for (int i = 0; i <  MAX_FAULTLINE_ITERATIONS; i++)
		{
			std::random_device rd;
			std::mt19937 gen(rd());
			std::uniform_real_distribution<> disWidth(0, 100);
			std::uniform_real_distribution<> disHeight(0, 100);

			faultLineSettings->interationsRandomPoints[i].x = disWidth(gen);
			faultLineSettings->interationsRandomPoints[i].z = disWidth(gen);

			faultLineSettings->interationsRandomPoints[i].y = disHeight(gen);
			faultLineSettings->interationsRandomPoints[i].w = disHeight(gen);
 
		}
	}
}
void TerrainScene::Render(RenderTexture * renderTexture, D3D * device, Camera * camera, RenderTexture * depthMap[], Light * light[])
{

	terrain->GenerateHeightMap(device->GetDevice(), false,sound);
//	GenerateDepthPass(device, camera, depthMap, light);

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
	worldMatrix = 	terrain->SendData(device->GetDeviceContext());
	//// Set shader parameters (matrices and texture)
	terrainShader->SetShaderParameters(device->GetDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, terrain->GetTexture(),
		faultLineSettings, terrainTextureSettings, light, depthMaps,
		lowTexture->GetTexture(), mediumTexture->GetTexture(), hightTexture->GetTexture());
	//// Render object (combination of mesh geometry and shader process
	terrainShader->Render(device->GetDeviceContext(), terrain->GetIndexCount());


	device->SetBackBufferRenderTarget();


}

void TerrainScene::MenuOptions()
{
	static bool terrainOptions;
	static bool soundOptions;
 	if (ImGui::BeginMenu("Terrain Options"))
	{
		if (ImGui::MenuItem("Terrain Settings "))
		{
			terrainOptions = terrainOptions ? false : true;

		}
		if (ImGui::MenuItem("Sound Details"))
		{
			soundOptions = soundOptions ? false : true;
		}
		if (ImGui::BeginMenu("Objects"))
		{
			if (ImGui::MenuItem(terrain->getName().c_str()))
			{
				terrain->ToggleMenu();
			}
			ImGui::EndMenu();

		}
		ImGui::EndMenu();

	}
	terrain->GuiSettings(&terrain->menuOpen);
	sound->GUI_Menu(&soundOptions);
	//terrain->Settings(&terrainOptions);
	TerrainSettings(&terrainOptions);

}

void TerrainScene::ResetLights(Light * lights[])
{
	lights[0]->SetDiffuseColour(1, 1, 1, 1);

	lights[0]->SetPosition(-68, 82, 177);

}

void TerrainScene::GenerateDepthPass(D3D * device, Camera * camera, RenderTexture * depthMap[], Light * lights[])
{
	// Loop though for each light to generate depth map for each
	for (int i = 0; i < NUM_LIGHTS; i++)
	{
		XMMATRIX worldMatrix;

		XMMATRIX lightViewMartix, lightProjectionMatrix;

		// Set the render target to be the render to texture.
		depthMap[i]->SetRenderTarget(device->GetDeviceContext());

		// Clear the render to texture.
		depthMap[i]->ClearRenderTarget(device->GetDeviceContext(), 0.0f, 0.0f, 1.0f, 1.0f);

		// Generate the view matrix based on the camera's position.
		camera->Update();

		// Get the world, view, and projection matrices from the camera and d3d objects.
		device->GetWorldMatrix(worldMatrix);

		lights[i]->GenerateViewMatrix();
		lightViewMartix = lights[i]->GetViewMatrix();

		lights[i]->GenerateProjectionMatrix(ApplicationSettings::sceenNear, ApplicationSettings::screenDepth);
		lightProjectionMatrix = lights[i]->GetProjectionMatrix();
		worldMatrix = XMMatrixScaling(0.1, 0.1, 0.1);


		////// Send geometry data (from mesh)


		//// Send geometry data (from mesh)
		worldMatrix = terrain->SendData(device->GetDeviceContext());
		//// Set shader parameters (matrices and texture)
		depthShader->SetShaderParameters(device->GetDeviceContext(), worldMatrix, lightViewMartix, lightProjectionMatrix);
		//// Render object (combination of mesh geometry and shader process
		depthShader->Render(device->GetDeviceContext(), terrain->GetIndexCount());






		// Reset the render target back to the original back buffer and not the render to texture anymore.
		device->SetBackBufferRenderTarget();
		device->ResetViewport();
	}
}

void TerrainScene::SceneInformationPopUp(bool * is_open)
{

}

void TerrainScene::TerrainSettings(bool * is_open)
{
	if (*is_open == true)
	{
		// Create the window
		if (!ImGui::Begin("Terrain Settings", is_open, ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::End();
			return;
		}
		if (ImGui::Checkbox("Enable Fault Line Displacement", &faultLineSettings->enableFaultLineDisplacement))
		{
		}
		if (faultLineSettings->enableFaultLineDisplacement)
		{

			if (ImGui::SliderInt("Number of Iterations", &faultLineSettings->numberOfIterations, 0, MAX_FAULTLINE_ITERATIONS))
			{
			}
			if (ImGui::SliderFloat("Displacement Value ", &faultLineSettings->startingDisplacement, 0, 1))
			{
			}
			if (ImGui::SliderFloat("Minimum displacement value ", &faultLineSettings->minimumDisplacement, 0, faultLineSettings->startingDisplacement))
			{
			}
			if (ImGui::SliderFloat("Smoothing Value ", &faultLineSettings->smoothingValue, 0, 1, "%.3f", 0.1f))
			{
			}

			if (ImGui::Checkbox("Display Normal map",&terrainTextureSettings->displayNormalMap))
			{
 			}
			if (ImGui::Button("Regenerate Fault Line Values"))
			{
				regenerateFaultLines = true;
			}
		}
		ImGui::End();

	}
}

 