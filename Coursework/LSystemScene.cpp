#include "LSystemScene.h"
#include "../imgui/imgui.h"


LSystemScene::LSystemScene(string sceneName)
	: Scene(sceneName)
{
	colourShader = nullptr;
	for (int i = 0; i < NUMBER_LSYSTEMS; i++)
	{
		lSystem[i] = nullptr;

	}
	textureShader = nullptr;


}


 
LSystemScene::~LSystemScene()
{

	// Release the Direct3D object.
	if (textureShader)
	{
		delete textureShader;
		textureShader = nullptr;
	}
	for (int i = 0; i < NUMBER_LSYSTEMS; i++)
	{
		if (lSystem[i])
		{
			delete 	lSystem[i];
			lSystem[i] = nullptr;

		}

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
	lSystemShader = new LSystemShader(device, hwnd);
	// init objects

	planeMesh = new PlaneMesh("Dynamic Texture",device, deviceContext, L"../res/bunny.png", 100);
	cube = new CubeMesh("Dynamic Cube texture", device, deviceContext, L"../res/bunny.png", 100);

	// Create L-System rules 
	std::unordered_map<char, std::string> drangonCurveRules{
		{ 'X',"X+YF+" },
		{ 'Y',"-FX-Y" },
	};

	std::unordered_map<char, std::string> kochCurveRules{
		{ 'F',"F+F-F-F+F" },

	};


	std::unordered_map<char, std::string> sierpinskisCarpetRules{
		{ 'F',"F+F-F-F-G+F+F+F-F" },
		{ 'G',"GGG" },

	};
	std::unordered_map<char, std::string> drangonHighwayRules{
		{ 'X',"X+YF" },
		{ 'Y',"FX-Y" },

	};
	std::unordered_map<char, std::string> SpaceFillingCurveRules{
		{ 'X',"-YF+XFX+FY-" },
		{ 'Y',"+XF-YFY-FX+" },

	};

	std::unordered_map<char, std::string> JoinedCrossCurvesRules{
		{ 'X',"FX+FX+FXFY-FY-" },
		{ 'Y',"+FX+FXFY-FY-FY" },
		{ 'F',"" },

	};
	std::unordered_map<char, std::string> modKochCurveRules1{
		{ 'F',"FF-F-F-F-F-F+F" },

	};
	std::unordered_map<char, std::string> modKochCurveRules2{
		{ 'F',"FF-F-F-F-FF" },

	};
	std::unordered_map<char, std::string> modKochCurveRules3{
		{ 'F',"FF-F+F-F-FF" },

	};
	std::unordered_map<char, std::string> modKochCurveRules4{
		{ 'F',"FF-F--F-F" },

	};
	std::unordered_map<char, std::string> modKochCurveRules5{
		{ 'F',"F-FF--F-F" },

	};
	std::unordered_map<char, std::string> modKochCurveRules6{
		{ 'F',"F-F+F-F-F" },

	};

	// init L-System
	lSystem[currentLSystem] = new LSystem(device,14,3, drangonCurveRules,"FX");
	lSystem[currentLSystem + 1] = new LSystem(device, 5, 4, kochCurveRules, "F", XMINT2(10, 350));
	lSystem[currentLSystem + 2] = new LSystem(device, 4, 3, sierpinskisCarpetRules, "F", XMINT2(100, 450));
	lSystem[currentLSystem + 3] = new LSystem(device, 12, 8, drangonHighwayRules, "FX", XMINT2(400, 250));
	lSystem[currentLSystem + 4] = new LSystem(device, 3, 5, JoinedCrossCurvesRules, "XYXYXYX+XYXYXYX+XYXYXYX+XYXYXYX", XMINT2(500, 500));
	lSystem[currentLSystem + 5] = new LSystem(device, 3, 8, modKochCurveRules1, "F-F-F-F", XMINT2(500, 350));
	lSystem[currentLSystem + 6] = new LSystem(device, 4, 6, modKochCurveRules2, "F-F-F-F", XMINT2(500, 435));
	lSystem[currentLSystem + 7] = new LSystem(device, 5, 5, modKochCurveRules3, "F-F-F-F", XMINT2(500, 500));
	lSystem[currentLSystem + 8] = new LSystem(device, 4, 5, modKochCurveRules4, "F-F-F-F", XMINT2(500, 500));
	lSystem[currentLSystem + 9] = new LSystem(device, 4, 14, modKochCurveRules5, "F-F-F-F", XMINT2(500, 500));
	lSystem[currentLSystem + 10] = new LSystem(device, 5,6, modKochCurveRules6, "F-F-F-F", XMINT2(500, 500));

	updateLSysteam = true;


	lSystem[currentLSystem]->GenterateUpdateTexture(device);
	lSystem[currentLSystem + 1]->GenterateUpdateTexture(device);
	lSystem[currentLSystem + 2]->GenterateUpdateTexture(device);
	lSystem[currentLSystem + 3]->GenterateUpdateTexture(device);
	lSystem[currentLSystem + 4]->GenterateUpdateTexture(device);
	lSystem[currentLSystem + 5]->GenterateUpdateTexture(device);
	lSystem[currentLSystem + 6]->GenterateUpdateTexture(device);
	lSystem[currentLSystem + 7]->GenterateUpdateTexture(device);
	lSystem[currentLSystem + 8]->GenterateUpdateTexture(device);
	lSystem[currentLSystem + 9]->GenterateUpdateTexture(device);
	lSystem[currentLSystem + 10]->GenterateUpdateTexture(device);



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


	cube->setScaleRotationTranslation(XMFLOAT3(3.35,2.5, 0.010000), XMFLOAT3(0,0,0), XMFLOAT3(0,0,200));

}
void LSystemScene::Update(Timer * timer)
{
	lSystem[currentLSystem]->Update();
}
void LSystemScene::Render(RenderTexture * renderTexture, D3D * device, Camera * camera, RenderTexture * depthMap[], Light * light[])
{
  
	// Check if l-system is needing updated 
	if (updateLSysteam)
	{
		updateLSysteam = false;
		lSystem[currentLSystem]->GenterateUpdateTexture(device->GetDevice());
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
	worldMatrix = cube->SendData(device->GetDeviceContext());
	//// Set shader parameters (matrices and texture)
	lSystemShader->SetShaderParameters(device->GetDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, lSystem[currentLSystem]->texture);
	//// Render object (combination of mesh geometry and shader process
	lSystemShader->Render(device->GetDeviceContext(), cube->GetIndexCount());

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
			if (ImGui::MenuItem(cube->getName().c_str()))
			{
				cube->ToggleMenu();
			}
			ImGui::EndMenu();

		}



		ImGui::EndMenu();

	}
	cube->GuiSettings(&cube->menuOpen);
	planeMesh->GuiSettings(&planeMesh->menuOpen);
	LSystemMenu(&lStsytemSettings);
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

void LSystemScene::LSystemMenu(bool * is_open)
{

	if (*is_open)
	{
		// Create the window
		if (!ImGui::Begin("L-System ", is_open, ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::End();
			return;
		}
		ImGui::Text("L-System options");
		ImGui::Text("Will Effect how the l-system will show. ");
 
		if (ImGui::Button("Refresh L-System"))
		{
			updateLSysteam = true;
		}

		int currentLS = currentLSystem;



		if (ImGui::Combo("Terrain Generation Type", &currentLS, lSystemTypes_str, IM_ARRAYSIZE(lSystemTypes_str)))
		{
			currentLSystem = (lSystemTypes)currentLS;
			lSystem[currentLSystem]->textureNeedingUpdated = true;
			updateLSysteam = true;

		}
		
		lSystem[currentLSystem]->Gui_menu(is_open);


		ImGui::End();
	}

}