#include "TerrainScene.h"
#include "../imgui/imgui.h"
#include "../ApplicationSettings.h"
#include <random>

#include  <algorithm>
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
		terrain = nullptr;
	}

	if (colourShader)
	{
		delete colourShader;
		colourShader = nullptr;
	}

	if (textureShader)
	{
		delete textureShader;
		textureShader = nullptr;
	}

	if (terrainShader)
	{
		delete terrainShader;
		terrainShader = nullptr;
	}
	if (depthShader)
	{
		delete depthShader;
		depthShader = nullptr;
	}

	if (faultLineSettings)
	{
		delete faultLineSettings;
		faultLineSettings = nullptr;
	}
	

	if (terrainTextureSettings)
	{
		delete terrainTextureSettings;
		terrainTextureSettings = nullptr;
	}

	if (terrainGeneration)
	{
		delete terrainGeneration;
		terrainGeneration = nullptr;
	}


	if (fbnSettings)
	{
		delete fbnSettings;
		fbnSettings = nullptr;

	}

	if (lowTexture)
	{
		delete lowTexture;
		lowTexture = nullptr;
	}
	if (mediumTexture)
	{
		delete mediumTexture;
		mediumTexture = nullptr;
	}

	if (hightTexture)
	{
		delete hightTexture;
		hightTexture = nullptr;
	}

	if (waterMesh)
	{
		delete waterMesh;
		waterMesh = nullptr;
	}
	if (waveInfo)
	{
		delete waveInfo;
		waveInfo = nullptr;
	}
}


void TerrainScene::Init(HWND hwnd, ID3D11Device * device, ID3D11DeviceContext * deviceContext,Sound* sound)
{
	Scene::Init(hwnd, device, deviceContext, sound);

	// Create Mesh object
	terrain = new Terrain("Terrain",device, deviceContext, L"../res/bunny.png");
 	terrain->InitializeTerrain(device, 129, 129);
  	colourShader = new ColourShader(device, hwnd);
	textureShader = new TextureShader(device, hwnd);

	terrainShader = new TerrainShader(device,hwnd);

	depthShader = new DepthShader(device, hwnd);
	faultLineSettings = new FaultLineDisplacementBufferType();


	terrainTextureSettings = new TerrainSettingTextureType();
	tessSettings.maxDistance = 10;
	tessSettings.maxTesselationAmmount = 6;
	tessSettings.minDistance = 1;
	tessSettings.minTesselationAmmount = 1;


	faultLineSettings->enableFaultLineDisplacement = true;
	faultLineSettings->numberOfIterations = 250;
	faultLineSettings->startingDisplacement = 0.5f;

	terrain->setTranslation(XMFLOAT3(-25, -25, 50));

	terrainGeneration = new TerrainGenerationBufferType();
	diamondSquareSettings = new DimondSquareBuffer();
	fbnSettings = new FractionalBrowningNoiseBuffer();
	lowTexture = new Texture(device, deviceContext, L"../res/grassTex.png");
	mediumTexture = new Texture(device, deviceContext, L"../res/rock.png");
	hightTexture = new Texture(device, deviceContext, L"../res/snow.png");
	underWaterTexture = new Texture(device, deviceContext, L"../res/Sand2_S.jpg");
	hitByWaterTexture = new Texture(device, deviceContext, L"../res/Sand4_S.jpg");

	waveShader = new WaterShader(device, hwnd);

	waterMesh = new PlaneMesh("Water", device, deviceContext, L"../res/tex_Water.jpg", 129);

	waterMesh->setTranslation(XMFLOAT3(-25, -28, 50));

	diamondSquareSettings->widthOfGrid = 129;
	diamondSquareSettings->heightOfGrid = 129;
	fbnSettings->fbnOctaves = 8;
 	fbnSettings->fbnAmplitude = 1.0f;
	fbnSettings->fbnLacunarity = 0.5f;
	fbnSettings->fbnFrequancy = 8.2f;
	fbnSettings->fbnPelinNoiseFreqnacy = 0.55f;
	fbnSettings->fbnPersistence = 6.99f;
	fbnSettings->heightScale = 10.1f;

	waveInfo = new WavetBufferType();
	waveInfo->amplutude = 0.2;
	waveInfo->speed = 1.2;
	waveInfo->steepnesss = 0.2;
	waveInfo->freqancy = XMFLOAT3(0.25, 0.4, 0.1);

	terrainTextureSettings->textureTiling = 1; 
 
	std::random_device rd;
	seed = rd();

	terrain->setRandomSeed( &seed);

	terrainGeneration->enableGPUEffect = false;

	terrain->faultLineSettings = faultLineSettings;
	terrain->fBMSettings = fbnSettings;

	terrainTextureSettings->topHighPercentage = 90;

	terrainTextureSettings->midHighPercentage = 25;


	terrainTextureSettings->blendingPercentage = 15;


	waterAlphaValue = 0.8f;
  }

void TerrainScene::Update(Timer * timer)
{
	waveInfo->time = timer->GetTotalTimePast();

	terrainTextureSettings->medHeightOfWater = waterMesh->getTranslation().y;
	terrainTextureSettings->maxHeightOfWater = waterMesh->getTranslation().y + waveInfo->steepnesss;
	terrainTextureSettings->minHeightOfWater = waterMesh->getTranslation().y - waveInfo->steepnesss;
	if (faultLineSettings->enableFaultLineDisplacement && sound->getData(BASS_DATA_FFT256 | BASS_DATA_FLOAT, 128)[1] > 0.4f)
	{
 
		faultLineSettings->numberOfIterations++;
	}
 

	terrainTextureSettings->maxHightOfHill = terrain->getMaxHeight() + terrain->getTranslation().y;
	terrainTextureSettings->minHightOfTerrain = terrain->getMinHight() + terrain->getTranslation().y;

	if (terrainTextureSettings->minHightOfTerrain < terrainTextureSettings->maxHeightOfWater)
	{
		terrainTextureSettings->minHightOfTerrain = terrainTextureSettings->maxHeightOfWater;

	}
	// If needing to genereat rnadom numbers 
	if (regenerateFaultLines)
	{
		terrain->generateFaultLinelineDisplacement = true;

		regenerateFaultLines = false;
		for (int i = 0; i <  MAX_FAULTLINE_ITERATIONS; i++)
		{
 
			std::mt19937 gen(seed +i);
			std::mt19937 genSide(seed + seed + i);
			std::mt19937 genSideSecond(seed * seed + i);
			std::uniform_real_distribution<> disWidth(1, 128);
			std::uniform_real_distribution<> disWidth2(1, 128);
			std::uniform_real_distribution<> disHeight(1, 128);
			std::uniform_real_distribution<> disHeight2(1, 128);




			std::uniform_real_distribution<> sideToChoice(0, 3);
			int firstSide = sideToChoice(genSide);

			switch (firstSide)
			{
				// x = 0 ; y 0->129
			case 0:
			{
				faultLineSettings->interationsRandomPoints[i].x = 1;
				faultLineSettings->interationsRandomPoints[i].y = disHeight(gen);
				int firstSide = 0;
				do {
					firstSide = sideToChoice(genSideSecond);
				} while (firstSide ==0);
				switch (firstSide)
				{
					// x = 129 ; y 0->129
				case 1:
				{
					faultLineSettings->interationsRandomPoints[i].z = 128;
					faultLineSettings->interationsRandomPoints[i].w = disHeight2(gen);
					break;
				}
				// x = 0->129 ; y 0
				case 2:
				{
					faultLineSettings->interationsRandomPoints[i].z = disWidth2(gen);;
					faultLineSettings->interationsRandomPoints[i].w = 1;
					break;
				}
				// x = 0->129 ; y 129
				case 3:
				{
					faultLineSettings->interationsRandomPoints[i].z = disWidth2(gen);;
					faultLineSettings->interationsRandomPoints[i].w = 128;
					break;
				}
				}

				break;
			}
			// x = 129 ; y 0->129
			case 1:
			{
				faultLineSettings->interationsRandomPoints[i].x = 128;
				faultLineSettings->interationsRandomPoints[i].y = disHeight(gen);
				int firstSide = 1;
				do {
					firstSide = sideToChoice(genSideSecond);
				} while (firstSide == 1);
				switch (firstSide)
				{
					// x = 0 ; y 0->129
				case 0:
				{
					faultLineSettings->interationsRandomPoints[i].z = 1;
					faultLineSettings->interationsRandomPoints[i].w = disHeight2(gen);
					break;
				}
				// x = 0->129 ; y 0
				case 2:
				{
					faultLineSettings->interationsRandomPoints[i].z = disWidth2(gen);;
					faultLineSettings->interationsRandomPoints[i].w = 1;
					break;
				}
				// x = 0->129 ; y 129
				case 3:
				{
					faultLineSettings->interationsRandomPoints[i].z = disWidth2(gen);;
					faultLineSettings->interationsRandomPoints[i].w = 128;
					break;
				}
				}

				break;
			}
			// x = 0->129 ; y 0
			case 2:
			{
				faultLineSettings->interationsRandomPoints[i].x = disWidth(gen);
				faultLineSettings->interationsRandomPoints[i].y = 1;
				int firstSide = 2;
				do {
					firstSide = sideToChoice(genSideSecond);
				} while (firstSide == 2);
				switch (firstSide)
				{
					// x = 0 ; y 0->129
				case 0:
				{
					faultLineSettings->interationsRandomPoints[i].z = 1;
					faultLineSettings->interationsRandomPoints[i].w = disHeight2(gen);
					break;
				}
				// x = 0 ; y 0->129
				case 1:
				{
					faultLineSettings->interationsRandomPoints[i].z = 128;
					faultLineSettings->interationsRandomPoints[i].w = disHeight2(gen);
					break;
				}
				// x = 0->129 ; y 129
				case 3:
				{
					faultLineSettings->interationsRandomPoints[i].z = disWidth2(gen);;
					faultLineSettings->interationsRandomPoints[i].w = 128;
					break;
				}
				}

				break;
 			}
			// x = 0->129 ; y 129
			case 3:
			{
				faultLineSettings->interationsRandomPoints[i].x = disWidth(gen);
				faultLineSettings->interationsRandomPoints[i].y = 128;
				int firstSide = 3;
				do {
					firstSide = sideToChoice(genSideSecond);
				} while (firstSide == 3);
				switch (firstSide)
				{
					// x = 0 ; y 0->129
				case 0:
				{
					faultLineSettings->interationsRandomPoints[i].z = 1;
					faultLineSettings->interationsRandomPoints[i].w = disHeight2(gen);
					break;
				}
				// x = 0 ; y 0->129
				case 1:
				{
					faultLineSettings->interationsRandomPoints[i].z = 129;
					faultLineSettings->interationsRandomPoints[i].w = disHeight2(gen);
					break;
				}
				// x = 0->129 ; y 0
				case 2:
				{
					faultLineSettings->interationsRandomPoints[i].z = disWidth2(gen);;
					faultLineSettings->interationsRandomPoints[i].w = 1;
					break;
				}
				}

				break;
			}
			default:
				break;
			}

			//faultLineSettings->interationsRandomPoints[i].x = disWidth(gen);
			//faultLineSettings->interationsRandomPoints[i].z = disWidth2(gen);

			//faultLineSettings->interationsRandomPoints[i].y = disHeight(gen);
			//faultLineSettings->interationsRandomPoints[i].w = disHeight2(gen);
 
		}
	}
	else if (regenerateDiamondSquare)
	{
		regenerateDiamondSquare = false;
		terrain->generateDiamondSquareTerrain = true;
		std::mt19937 gen(seed);
		std::uniform_real_distribution<> dis(0, 1);


		diamondSquareSettings->bottomLeftRandomNumber = dis(gen);
		diamondSquareSettings->bottomRightRandomNumber = dis(gen);
		diamondSquareSettings->topLeftRandomNumber = dis(gen);
		diamondSquareSettings->topRightRandomNumber = dis(gen);
	}
	else if (regenerateSimplexNoise)
	{
		regenerateSimplexNoise = false;
		terrain->generateSimplexNoiseTerrain = true;

	}
	if (regenerateCellularAutomata)
	{
		regenerateCellularAutomata = false;
		terrain->generateSimplexNoiseTerrain = true;

	}

	if (regenerateParticleDeposition)
	{
		regenerateParticleDeposition = false;
		terrain->particleDepositionRegeneate = true;

	}

	if (regenerateFBM)
	{
		regenerateFBM = false;
		terrain->genereateFractionalBrownainNoise = true;
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




	// Send geometry data (from mesh)
	worldMatrix = 	terrain->SendData(device->GetDeviceContext());
	// Set shader parameters (matrices and texture)
	terrainShader->SetShaderParameters(device->GetDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, terrain->GetTexture(),
		terrainGeneration,faultLineSettings, terrainTextureSettings,fbnSettings, light, depthMaps,
		lowTexture->GetTexture(), mediumTexture->GetTexture(), hightTexture->GetTexture(), underWaterTexture->GetTexture(), hitByWaterTexture->GetTexture());
	// Render object (combination of mesh geometry and shader process
	terrainShader->Render(device->GetDeviceContext(), terrain->GetIndexCount());


	device->TurnOnAlphaBlending();	


	worldMatrix = waterMesh->SendData(device->GetDeviceContext());

	waveShader->SetShaderParameters(device->GetDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, waterMesh->GetTexture(), waveInfo, waterAlphaValue);

	waveShader->Render(device->GetDeviceContext(), waterMesh->GetIndexCount());
	device->TurnOffAlphaBlending();
	

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
		if (ImGui::MenuItem("Water Settings "))
		{
			waterMenuOpen = waterMenuOpen ? false : true;

		}
	/*	if (ImGui::MenuItem("Sound Details"))
		{
			soundOptions = soundOptions ? false : true;
		}*/
		if (ImGui::BeginMenu("Objects"))
		{
			if (ImGui::MenuItem(terrain->getName().c_str()))
			{
				terrain->ToggleMenu();

			}
			if (ImGui::MenuItem(waterMesh->getName().c_str()))
			{
				waterMesh->ToggleMenu();

			}
			ImGui::EndMenu();

		}
		ImGui::EndMenu();

	}
	waterMesh->GuiSettings(&waterMesh->menuOpen);
	terrain->GuiSettings(&terrain->menuOpen);
	sound->GUI_Menu(&soundOptions);
	//terrain->Settings(&terrainOptions);
	TerrainSettings(&terrainOptions);
	WaterSettings(&waterMenuOpen);
}

void TerrainScene::ResetLights(Light * lights[])
{
	lights[0]->SetDiffuseColour(1, 1, 1, 1);

	lights[0]->SetPosition(-61, -47, -7);

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

		ImGui::Text("Terrain Generation Settings - will effect how terrain looks");
 
		/////////////////////////////////////////////////////////////////
		// GENERAL TERRAIN SETTINGS
		/////////////////////////////////////////////////////////////////
		if (terrainGeneration->terrainGenerationType != TerrainGeneration::DiamondSquare)
		{	
			ImGui::Separator();

			ImGui::Text("Will make caculations in real time on the GPU rather than pre caculated on the CPU if enabled");
			if (ImGui::Checkbox("Enable GPU Generation", &terrainGeneration->enableGPUEffect))
			{
				regenerateFaultLines = true;

			}

			ImGui::Text("WARNING: This may be force On or Off for some techniques");
			ImGui::Text("WARNING: Results on GPU may not be identical to ones made of CPU");

		}
 


		/////////////////////////////////////////////////////////////////
		// SEED SETTINGS
		/////////////////////////////////////////////////////////////////
		ImGui::Separator();

		ImGui::Text("Seed used to generate any pure random numbers (NOTE: Using Mersenne Twister 19937 generator)");
		if (ImGui::InputInt("Terrain Seed", &seed))
		{
			switch (terrainGeneration->terrainGenerationType)
			{
			case TerrainGeneration::DiamondSquare:
			{
				regenerateDiamondSquare = true;
				break;
			}
			case TerrainGeneration::SimplexNoise:
			{
				regenerateSimplexNoise = true;
				break;
			}
			case TerrainGeneration::FractionalBrowningNoise:
			{
				regenerateFBM = true;
				break;
			}
			case TerrainGeneration::FaultLineDisplacement:
			{
				terrain->generateFaultLinelineDisplacement = true;
				regenerateFaultLines = true;

				break;
			}

			case TerrainGeneration::ParticleDeposition:
			{
				terrain->particleDepositionRegeneate = true;
				regenerateParticleDeposition = true;
				break;
			}
			default:
				break;
			}
		}
 

		/////////////////////////////////////////////////////////////////
		// PICKING TECHNIQUE SETTINGS
		/////////////////////////////////////////////////////////////////
		ImGui::Separator();

		ImGui::Text("Type of proccedural Techniques to use");
		int currnetGen = terrainGeneration->terrainGenerationType;
	
		if (ImGui::Combo("Terrain Generation Type", &currnetGen, TerrainGeneration_str, IM_ARRAYSIZE(TerrainGeneration_str)))
		{

			terrainGeneration->terrainGenerationType = (TerrainGeneration)currnetGen;

		switch (terrainGeneration->terrainGenerationType)
		{
		case TerrainGeneration::DiamondSquare:
		{
			terrainGeneration->enableGPUEffect = false;

			regenerateDiamondSquare = true;
			break;
		}
		case TerrainGeneration::SimplexNoise:
		{
			terrainGeneration->enableGPUEffect = false;

			regenerateSimplexNoise = true;
			break;
		}
		case TerrainGeneration::FractionalBrowningNoise:
		{
			terrainGeneration->enableGPUEffect = false;
			regenerateFBM = true;
 

			break;
		}
		case TerrainGeneration::FaultLineDisplacement:
		{
			terrain->generateFaultLinelineDisplacement = true;
			terrainGeneration->enableGPUEffect = false;
			regenerateFaultLines = true;

			break;
		}
		//case TerrainGeneration::CellularAutomata:
		//{
		//	terrain->cellularAutomataRegenerate = true;
		//	terrainGeneration->enableGPUEffect = false;
		//	regenerateFaultLines = true;
		//	break;
		//}
		case TerrainGeneration::ParticleDeposition:
		{
			terrain->particleDepositionRegeneate = true;
			terrainGeneration->enableGPUEffect = false;
			regenerateParticleDeposition = true;
			break;
		}
		default:
			break;
		}

		}






		/////////////////////////////////////////////////////////////////
		// INDVIDURAL TECHIQUE SETTINGS
		/////////////////////////////////////////////////////////////////
		ImGui::Separator();

		ImGui::Text("Given Techique settings - Settings for selected  techique ");

		if (terrainGeneration->terrainGenerationType == TerrainGeneration::FaultLineDisplacement)
		{



			if (ImGui::SliderInt("Number of Iterations", &faultLineSettings->numberOfIterations, 2, MAX_FAULTLINE_ITERATIONS))
			{
				regenerateFaultLines = true;

			}

			if (ImGui::SliderFloat("Displacement Value ", &faultLineSettings->startingDisplacement, 0, 5))
			{
				regenerateFaultLines = true;

			}


			if (ImGui::Button("Regenerate Fault Line Values"))
			{
				std::random_device rd;
				seed = rd();
				regenerateFaultLines = true;
			}

		}

		else if (terrainGeneration->terrainGenerationType == TerrainGeneration::FractionalBrowningNoise)
		{

			if	(ImGui::DragFloat("Fbn Octaves", &fbnSettings->fbnOctaves, 1, 1, 16))
			{
				regenerateFBM = true;
			}
			if (ImGui::DragFloat("Fbn Amplitude", &fbnSettings->fbnAmplitude, 0.05f, 0.0f, 2))
			{
			regenerateFBM = true;
			}
			if (ImGui::DragFloat("Fbn Lacunarity", &fbnSettings->fbnLacunarity, 0.05f, 0.0f, 2))
			{
			regenerateFBM = true;
			}
			if (ImGui::DragFloat("Fbn Frequancy", &fbnSettings->fbnFrequancy, 0.05f, 0.0f, 15))
			{
			regenerateFBM = true;
			}
			if (ImGui::DragFloat("Fbn Pelin Noise Freqancy", &fbnSettings->fbnPelinNoiseFreqnacy, 0.05f, 0.0f, 10))
			{
			regenerateFBM = true;
			}
			if (ImGui::DragFloat("Fbn Persistence", &fbnSettings->fbnPersistence, 0.05f, 0.0f, 10))
			{
			regenerateFBM = true;
			}
			if (ImGui::DragFloat("Fbn height Scale", &fbnSettings->heightScale,0.05f, 0.0f, 15))
			{
			regenerateFBM = true;
			}

			if (!fbnSettings->useRidged)
			{
				if (ImGui::Checkbox("Use Billow", &fbnSettings->useAbs))
				{
					regenerateFBM = true;
				}
			}

			if (!fbnSettings->useAbs)
			{

			
				if (ImGui::Checkbox("Use Riggid", &fbnSettings->useRidged))
				{
					regenerateFBM = true;
				}
			}
			if (ImGui::Button("Regeneraete FBM "))
			{
				std::random_device rd;
				seed = rd();
				regenerateFBM = true;
			}
 		}
		else if (terrainGeneration->terrainGenerationType == TerrainGeneration::DiamondSquare)
		{

			if (ImGui::Button("Regenerate DiamondSquare Alorigumth "))
			{
				std::random_device rd;
				seed = rd();
				regenerateDiamondSquare = true;
			}
		}
		else if (terrainGeneration->terrainGenerationType == TerrainGeneration::SimplexNoise)
		{

			if (ImGui::Button("Regenerate Simplex Noise "))
			{
				std::random_device rd;
				seed = rd();
				regenerateSimplexNoise = true;
			}
		}


		if (!terrainGeneration->enableGPUEffect)
		{
			bool f = true;
			terrain->GUISettings(&f, terrainGeneration->terrainGenerationType);
		}




			/////////////////////////////////////////////////////////////////
			// TEXTURE GUI
			/////////////////////////////////////////////////////////////////
			ImGui::Separator();



			ImGui::Text("Terrain Texture Settings");

			ImGui::DragFloat("Water Alpha Blending Values", &waterAlphaValue, 0.1f, 0.0f, 1.0f);

			ImGui::Text("How many times the texture will repeat");
			if (ImGui::DragInt("TerrainTexturing", &terrainTextureSettings->textureTiling))
			{
			}

			ImGui::InputFloat("Min Height of Wave", &terrainTextureSettings->minHeightOfWater);
			ImGui::InputFloat("mid Height of Wave", &terrainTextureSettings->medHeightOfWater);
			ImGui::InputFloat("max Height of Wave", &terrainTextureSettings->maxHeightOfWater);



			ImGui::SliderFloat("Hight of tip of hill starts ", &terrainTextureSettings->topHighPercentage, 0, 100);
			ImGui::SliderFloat("HIght of start of hill starts ", &terrainTextureSettings->midHighPercentage, 0, 100);
			ImGui::SliderFloat("Blending ammount of textures", &terrainTextureSettings->blendingPercentage, 0, 100);
			ImGui::Text("Max Hight Of hill %f", terrainTextureSettings->maxHightOfHill);
			ImGui::Text("Min Hight Of hill %f", terrainTextureSettings->minHightOfTerrain);


			/////////////////////////////////////////////////////////////////
			// DEBUG GUI
			/////////////////////////////////////////////////////////////////
			ImGui::Separator();

			ImGui::Text("Debug Settings");
			ImGui::Text("Enables and disables lights - useful when terrain get larger than range of lights");
			ImGui::Text("Allows to Visulize normals to show if working corrently");
			bool enableLighting = terrainTextureSettings->enableLighting;

			if (ImGui::Checkbox("Enable Lighting", &enableLighting))
			{
			}
			terrainTextureSettings->enableLighting = enableLighting;

 			ImGui::SameLine();
			bool dispalyNormalMap = terrainTextureSettings->displayNormalMap;

			if (ImGui::Checkbox("Display Normal map", &dispalyNormalMap))
			{
			}


			terrainTextureSettings->displayNormalMap = dispalyNormalMap;

		ImGui::End();

	}
}

void TerrainScene::WaterSettings(bool * is_open)
{
	if (*is_open == true)
	{
		// Create the window
		if (!ImGui::Begin("Water Settings", is_open, ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::End();
			return;
		}

		ImGui::DragFloat("Wave Aplitude", &waveInfo->amplutude);
		ImGui::DragFloat3("Wave Freqancy", &waveInfo->freqancy.x);
		ImGui::DragFloat("Wave Speed", &waveInfo->speed);
		ImGui::DragFloat("Wave Steepness", &waveInfo->steepnesss);
		
		ImGui::End();
	};
}

 