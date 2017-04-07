#include "baseApp.h"



BaseApp::BaseApp()
{

	// Make sure all object start a null pointers
	currentScene = nullptr; 
	orthoMeshNormalScaled = nullptr;
	for (int i = 0; i < NUM_LIGHTS; i++)
	{
		lights[i] = nullptr;
		depthTextures[i] = nullptr;
	}
	sceneTexture = nullptr;
	upScaleTexture = nullptr;
	textureShader = nullptr;
	emptyScene = nullptr;
	lSystemScene = nullptr;
	terrainScene = nullptr;
}


BaseApp::~BaseApp()
{
	BaseApplication::~BaseApplication();
 
	if (lSystemScene)
	{
		delete lSystemScene;
		lSystemScene = nullptr;
	}
	if (emptyScene)
	{
		delete emptyScene;
		emptyScene = nullptr;
	}
	//clean up memory
	if (currentScene)
	{
		delete currentScene;
		currentScene = nullptr;
	}

	if (terrainScene)
	{
		
		delete terrainScene;
		terrainScene = nullptr;
	}
 

	if (orthoMeshNormalScaled)
	{
		delete orthoMeshNormalScaled;
		orthoMeshNormalScaled = nullptr;
	}
	for (int i = 0; i < NUM_LIGHTS; i++)
	{
		if (lights[i])
		{
			delete lights[i];
			lights[i] = nullptr;

		}
		if (depthTextures[i])
		{
			delete depthTextures[i];
			depthTextures[i] = nullptr;
		}
	}

	if (sceneTexture)
	{
		delete sceneTexture;
		sceneTexture = nullptr;
	}

	if (upScaleTexture)
	{
		delete upScaleTexture;
		upScaleTexture = nullptr;
	}


	if (textureShader)
	{
		delete textureShader;
		textureShader = nullptr;
	}

	if (sound)
	{
		delete sound;
		sound = nullptr;
	}


}

void BaseApp::init(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input *in)
{
	// Call super init function (required!)
	BaseApplication::init(hinstance, hwnd, screenWidth, screenHeight, in);
 
	// Init sound information 
	sound = new Sound();
	sound->Init(L"../res/DiscoMedusae.mp3");
	sound->setPause(true);
 	
	// Load in Scenes 
	terrainScene = new TerrainScene("Terrain Scene");
	terrainScene->Init(hwnd, m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), sound);

	emptyScene = new EmptyScene("Empty Scene");
	emptyScene->Init(hwnd, m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), sound);


	lSystemScene = new LSystemScene("L-System Scene");
	lSystemScene->Init(hwnd, m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), sound);



	// Initialize lights and depth texture 
	for (int i = 0; i < NUM_LIGHTS; i++)
	{
		lights[i] = new Light();

		lights[i]->SetDirection(0, 0, 0);
		lights[i]->SetDiffuseColour(0, 0, 0, 0);
		lights[i]->SetLookAt(0, 0, 0);
		lights[i]->SetPosition(0, 0, -4);
		lights[i]->SetRange(255);

		depthTextures[i] = new RenderTexture(m_Direct3D->GetDevice(), screenWidth, screenHeight, sceenNear, screenDepth);
	}

	// Load in textures
	orthoMeshNormalScaled = new OrthoMesh("Normal OrthoMesh",m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), screenWidth, screenHeight, 0, 0);
	upScaleTexture = new  RenderTexture(m_Direct3D->GetDevice(), screenWidth, screenHeight, sceenNear, screenDepth);
	sceneTexture = new  RenderTexture(m_Direct3D->GetDevice(), screenWidth, screenHeight, sceenNear, screenDepth);


	// Load in shaders
	textureShader = new TextureShader(m_Direct3D->GetDevice(), hwnd);



	postPro.Init(m_Direct3D, hwnd, m_Timer);
	// set starting scene to be terrain scene
	currentScene = lSystemScene;

	// set my lights up correcntly for current scene
	currentScene->ResetLights(lights);
}

bool BaseApp::Frame()
{
	bool result;

	result = BaseApplication::Frame();
	if (!result)
	{
		return false;
	}
	currentScene->Update(m_Timer);
	// Render the graphics.
	result = Render();
	if (!result)
	{
		return false;
	}
	return true;

}
bool BaseApp::Render()
{



	// Render current scene  
	currentScene->Render(sceneTexture, m_Direct3D, m_Camera, depthTextures, lights);



	// disable wireframe mode for the post processing effects
	if (m_Direct3D->getWireFrameMode())
	{
		m_Direct3D->TurnOffWireframe();
	}

	// Apply any post processing effecst 
	upScaleTexture = postPro.ApplyPostProccessing(orthoMeshNormalScaled, sceneTexture, m_Direct3D, m_Camera);

	// Render the scene to the screen
	RenderToScreen();



	// disable wireframe mode for the post processing effects
	if (m_Direct3D->getWireFrameMode())
	{
		m_Direct3D->TurnOnWireframe();
	}

	return true;
}
void BaseApp::RenderToScreen()
{

	XMMATRIX worldMatrix, viewMatrix, projectionMatrix, baseViewMatrix, orthoMartix;

	//// Clear the scene. (default blue colour)
	m_Direct3D->BeginScene(0.39f, 0.58f, 0.92f, 1.0f);

	//// Generate the view matrix based on the camera's position.
	m_Camera->Update();

	// Reset the world martix back to starting point
	m_Direct3D->GetWorldMatrix(worldMatrix);


	// To render ortho mesh
	// Turn off the Z buffer to begin all 2D rendering.
	m_Direct3D->TurnZBufferOff();

	m_Direct3D->GetOrthoMatrix(orthoMartix);// ortho matrix for 2D rendering
	m_Camera->GetBaseViewMatrix(baseViewMatrix);

	orthoMeshNormalScaled->SendData(m_Direct3D->GetDeviceContext());



	textureShader->SetShaderParameters(m_Direct3D->GetDeviceContext(), worldMatrix, baseViewMatrix, orthoMartix, upScaleTexture->GetShaderResourceView());
	textureShader->Render(m_Direct3D->GetDeviceContext(), orthoMeshNormalScaled->GetIndexCount());


	m_Direct3D->TurnZBufferOn();

	// Present the rendered scene to the screen.
	m_Direct3D->EndScene();


}



void BaseApp::CreateMainMenuBar()
{
	static bool show_light_option[4];

	postPro.PostProccessingMenu();
	if (ImGui::BeginMenu("Scenes"))
	{
		if (ImGui::MenuItem(terrainScene->getSceneName().c_str()))
		{
			currentScene = terrainScene;
			currentScene->ResetLights(lights);
			terrainScene->isEnbaled = true;
			lSystemScene->isEnbaled = false;
		}
		if (ImGui::MenuItem(lSystemScene->getSceneName().c_str()))
		{
			currentScene = lSystemScene;
			currentScene->ResetLights(lights);
			lSystemScene->isEnbaled = true;
			terrainScene->isEnbaled = false;
		}

		ImGui::EndMenu();
	}
	if (ImGui::BeginMenu("Lights"))
	{
		for (int i = 0; i < 4; i++)
		{
			std::string lightName;
			lightName.append("Light ");
			lightName.append(std::to_string(i + 1));
			if (ImGui::MenuItem(lightName.c_str(), NULL, show_light_option[i]))
			{
				show_light_option[i] = show_light_option[i] ? false : true;
			}
		}

		ImGui::EndMenu();

	}
	currentScene->MenuOptions();
	// display light settings
	for (int i = 0; i < 4; i++)
	{

		lights[i]->DisplayGUIEditor(std::to_string(i), &show_light_option[i]);
	}

}


