// Lab1.cpp
// Lab 1 example, simple textured quad
#include "App1.h"
#include <string>

 
App1::App1()
{
 
	// Make sure all object start a null pointers
	currentScene = nullptr;



	orthoMeshNormalScaled = nullptr;
	 
	for (int i = 0; i < 4; i++)
	{
		lights[i] = nullptr;
		depthTextures[i] = nullptr;
	}

	sceneTexture = nullptr;

	upScaleTexture = nullptr;

 
 
	baseScene = nullptr;
	textureShader = nullptr;

}
App1::~App1()
{
	// Run base application deconstructor
	BaseApplication::~BaseApplication();


	//clean up memory
	if (currentScene)
	{
		delete currentScene;
		currentScene = nullptr;
	}



	if (orthoMeshNormalScaled)
	{
		delete orthoMeshNormalScaled;
		orthoMeshNormalScaled = nullptr;
	}
	for (int i = 0; i < 4; i++)
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

	if (baseScene)
	{
		delete baseScene;
		baseScene = nullptr;
	}
}


void App1::init(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input *in)
{
	// Call super init function (required!)
	BaseApplication::init(hinstance, hwnd, screenWidth, screenHeight, in);


	textureShader = new TextureShader(m_Direct3D->GetDevice(), hwnd);

	// Initialize lights and depth texture 
	for (int i = 0; i < 4; i++)
	{
		lights[i] = new Light();

		lights[i]->SetDirection(0, 0, 0);
		lights[i]->SetDiffuseColour(0, 0, 0, 0);
		lights[i]->SetLookAt(0, 0, 0);
		lights[i]->SetPosition(0, 0, -4);
 		lights[i]->SetRange(255);

		depthTextures[i] = new RenderTexture(m_Direct3D->GetDevice(), SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_NEAR, SCREEN_DEPTH);
	}
	 

	orthoMeshNormalScaled = new OrthoMesh(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0);
	upScaleTexture =new  RenderTexture(m_Direct3D->GetDevice(), SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_NEAR, SCREEN_DEPTH);

	sceneTexture = new  RenderTexture(m_Direct3D->GetDevice(), SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_NEAR, SCREEN_DEPTH);

 
	postPro.Init(m_Direct3D, hwnd, m_Timer);
 
	baseScene = new BaseScene("base Scene");

	// set starting scene to be terrain scene
	currentScene = baseScene;

	// set my lights up correcntly for current scene
	currentScene->ResetLights(lights);
}
 


bool App1::Frame()
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

bool App1::Render()
{

	// Render current scene  
	currentScene->Render(sceneTexture, m_Direct3D, m_Camera , depthTextures,lights);

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

 

 
void App1::RenderToScreen()
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

 

void App1::CreateMainMenuBar()
{
	
	static bool show_light_option[4];

	// Let user choice the scene viewing 
	if (ImGui::BeginMenu("Scene selection menu")) 
	{
		
		ImGui::EndMenu();

	}
 		
		if(ImGui::BeginMenu("Lights"))
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
		 
	postPro.PostProccessingMenu();

	currentScene->MenuOptions();
	  // display light settings
	for (int i = 0; i < 4; i++)
	{

		lights[i]->DisplayGUIEditor(std::to_string(i), &show_light_option[i]);
	}

}

 