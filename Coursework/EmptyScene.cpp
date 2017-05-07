#include "EmptyScene.h"

/* See the file "LICENSE" for the full license governing this code. */


EmptyScene::EmptyScene(string sceneName)
	: Scene(sceneName)
{
	mesh = nullptr;
	colourShader = nullptr;
}


EmptyScene::~EmptyScene()
{

	// Release the Direct3D object.
	if (mesh)
	{
		delete mesh;
		mesh = 0;
	}

	if (colourShader)
	{
		delete colourShader;
		colourShader = 0;
	}
}

void EmptyScene::Init(HWND hwnd, ID3D11Device * device, ID3D11DeviceContext * deviceContext, Sound * sound)
{
	Scene::Init(hwnd, device, deviceContext,sound);

	// Create Mesh object
	mesh = new DoubleTriangleMesh( "BaseMesh",device, deviceContext, L"../res/DefaultDiffuse.png");

	colourShader = new ColourShader(device, hwnd);
}

void EmptyScene::Update(Timer * timer)
{
}

void EmptyScene::Render(RenderTexture * renderTexture, D3D * device, Camera * camera, RenderTexture * depthMap[], Light * light[])
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

	//// Send geometry data (from mesh)
	mesh->SendData(device->GetDeviceContext());
	//// Set shader parameters (matrices and texture)
	colourShader->SetShaderParameters(device->GetDeviceContext(), worldMatrix, viewMatrix, projectionMatrix);
	//// Render object (combination of mesh geometry and shader process
	colourShader->Render(device->GetDeviceContext(), mesh->GetIndexCount());

	device->SetBackBufferRenderTarget();


 }

void EmptyScene::MenuOptions()
{
}

void EmptyScene::ResetLights(Light * lights[])
{
}

void EmptyScene::GenerateDepthPass(D3D * device, Camera * camera, RenderTexture * depthMap[], Light * light[])
{
}

void EmptyScene::SceneInformationPopUp(bool * is_open)
{
}
