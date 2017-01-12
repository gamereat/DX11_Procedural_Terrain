#include "BaseScene.h"


 

BaseScene::BaseScene(std::string sceneName) : Scene(sceneName)
{
}

BaseScene::~BaseScene()
{
	// Release the Direct3D object.
	if (m_Mesh)
	{
		delete m_Mesh;
		m_Mesh = 0;
	}

	if (m_ColourShader)
	{
		delete m_ColourShader;
		m_ColourShader = 0;
	}
}

void BaseScene::Init(HWND hwnd, ID3D11Device * device, ID3D11DeviceContext * deviceContext)
{
	Scene::Init(hwnd, device, deviceContext);


	// Create Mesh object
	m_Mesh = new DoubleTriangleMesh(device, deviceContext, L"../res/DefaultDiffuse.png");

	m_ColourShader = new ColourShader(device, hwnd);
}

void BaseScene::Update(Timer * timer)
{
}

void BaseScene::Render(RenderTexture * renderTexture, D3D * device, Camera * camera, RenderTexture * depthMap[], Light * light[])
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix, baseViewMatrix, orthoMartix;

	//m_Direct3D->BeginScene(0.39f, 0.58f, 0.92f, 1.0f);
	renderTexture->SetRenderTarget(device->GetDeviceContext());

	renderTexture->ClearRenderTarget(device->GetDeviceContext(), 0.0f, 0.0f, 1.0f, 1.0f);

	//// Generate the view matrix based on the camera's position.
	camera->Update();

	//// Generate the view matrix based on the camera's position.
	camera->Update();

	//// Get the world, view, projection, and ortho matrices from the camera and Direct3D objects.
	device->GetWorldMatrix(worldMatrix);
	camera->GetViewMatrix(viewMatrix);
	device->GetProjectionMatrix(projectionMatrix);

	//// Send geometry data (from mesh)
	m_Mesh->SendData(device->GetDeviceContext());
	//// Set shader parameters (matrices and texture)
	m_ColourShader->SetShaderParameters(device->GetDeviceContext(), worldMatrix, viewMatrix, projectionMatrix);
	//// Render object (combination of mesh geometry and shader process
	m_ColourShader->Render(device->GetDeviceContext(), m_Mesh->GetIndexCount());

	device->SetBackBufferRenderTarget();
}

void BaseScene::ResetLights(Light * lights[])
{
}

void BaseScene::MenuOptions()
{
}

void BaseScene::GenerateDepthPass(D3D * device, Camera * camera, RenderTexture * depthMap[], Light * lights[])
{
}

void BaseScene::SceneInformationPopUp(bool * is_open)
{
}
