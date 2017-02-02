#include "TerrainScene.h"
#include "../imgui/imgui.h"


TerrainScene::TerrainScene(string sceneName)
	: Scene(sceneName)
{
	terrain = nullptr;
	colourShader = nullptr;
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

void TerrainScene::Init(HWND hwnd, ID3D11Device * device, ID3D11DeviceContext * deviceContext)
{
	Scene::Init(hwnd, device, deviceContext);

	// Create Mesh object
	terrain = new Terrain(device, deviceContext, L"../res/bunny.png");
	terrain->InitializeTerrain(device, 100, 100);
  	colourShader = new ColourShader(device, hwnd);
	textureShader = new TextureShader(device, hwnd);
	model = new Model(device, deviceContext, L"../res/bunny.png", L"../res/teapot.obj");

}
void TerrainScene::Update(Timer * timer)
{
 
}
void TerrainScene::Render(RenderTexture * renderTexture, D3D * device, Camera * camera, RenderTexture * depthMap[], Light * light[])
{
 
	
	terrain->GenerateHeightMap(device->GetDevice(), false);

	XMMATRIX worldMatrix, viewMatrix, projectionMatrix, baseViewMatrixs;

	// Set the render target to be the render to texture.
	renderTexture->SetRenderTarget(device->GetDeviceContext());

	// Clear the render to texture.
	renderTexture->ClearRenderTarget(device->GetDeviceContext(), 0.0f, 0.0f, 1.0f, 1.0f);


	camera->Update();

	device->GetWorldMatrix(worldMatrix);
	camera->GetViewMatrix(viewMatrix);
	device->GetProjectionMatrix(projectionMatrix);


	//model->SendData(device->GetDeviceContext());
	//textureShader->SetShaderParameters(device->GetDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, model->GetTexture());
	//textureShader->Render(device->GetDeviceContext(), model->GetIndexCount());

	//// Send geometry data (from mesh)
	terrain->SendData(device->GetDeviceContext());
	//// Set shader parameters (matrices and texture)
	textureShader->SetShaderParameters(device->GetDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, terrain->GetTexture());
	//// Render object (combination of mesh geometry and shader process
	textureShader->Render(device->GetDeviceContext(), terrain->GetIndexCount());

	device->SetBackBufferRenderTarget();


}

void TerrainScene::MenuOptions()
{
	static bool terrainOptions;
	if (ImGui::BeginMenu("Terrain Options"))
	{
		if (ImGui::MenuItem("terain stuff "))
		{
			terrainOptions = terrainOptions ? false : true;

		}
		ImGui::EndMenu();

	}
	terrain->Settings(&terrainOptions);

}

void TerrainScene::ResetLights(Light * lights[])
{
}

void TerrainScene::GenerateDepthPass(D3D * device, Camera * camera, RenderTexture * depthMap[], Light * light[])
{
}

void TerrainScene::SceneInformationPopUp(bool * is_open)
{
}
