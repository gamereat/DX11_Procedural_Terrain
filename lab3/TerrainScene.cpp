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

	tessSettings.maxDistance = 10;
	tessSettings.maxTesselationAmmount = 6;
	tessSettings.minDistance = 1;
	tessSettings.minTesselationAmmount = 1;

	
}

void TerrainScene::Init(HWND hwnd, ID3D11Device * device, ID3D11DeviceContext * deviceContext,Sound* sound)
{
	Scene::Init(hwnd, device, deviceContext, sound);

	// Create Mesh object
	terrain = new Terrain(device, deviceContext, L"../res/bunny.png");
 	terrain->InitializeTerrain(device, 100, 100);
  	colourShader = new ColourShader(device, hwnd);
	textureShader = new TextureShader(device, hwnd);
 //	model = new Model(device, deviceContext, L"../res/bunny.png", L"../res/teapot.obj");
//	dbtShader = new DistanceBasedTesselation(device, hwnd);

	terrainShader = new TerrainShader(device,hwnd);
	//sound->Init(L"../res/BlownAway.mp3");
 
 }
void TerrainScene::Update(Timer * timer)
{
 
}
void TerrainScene::Render(RenderTexture * renderTexture, D3D * device, Camera * camera, RenderTexture * depthMap[], Light * light[])
{
 
	terrain->GenerateHeightMap(device->GetDevice(), false,sound);

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


	//model->SendData(device->GetDeviceContext());
	//textureShader->SetShaderParameters(device->GetDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, model->GetTexture());
	//textureShader->Render(device->GetDeviceContext(), model->GetIndexCount());
	//// Send geometry data (from mesh)
	terrain->SendData(device->GetDeviceContext());
	//// Set shader parameters (matrices and texture)
	terrainShader->SetShaderParameters(device->GetDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, terrain->GetTexture(), depthMaps, light);
	//// Render object (combination of mesh geometry and shader process
	terrainShader->Render(device->GetDeviceContext(), terrain->GetIndexCount());

	device->SetBackBufferRenderTarget();


}

void TerrainScene::MenuOptions()
{
	static bool terrainOptions;
	static bool lstsytemSettings;
	if (ImGui::BeginMenu("Terrain Options"))
	{
		if (ImGui::MenuItem("terain stuff "))
		{
			terrainOptions = terrainOptions ? false : true;

		}
		if (ImGui::MenuItem("Sound Settings"))
		{
			soundMenuOpen = soundMenuOpen ? false : true;

		}
		if (ImGui::MenuItem("L-System Settings"))
		{
			lstsytemSettings = lstsytemSettings ? false : true;

		}
		ImGui::EndMenu();

	}
	terrain->Settings(&terrainOptions);

	sound->GUI_Menu(&soundMenuOpen);

	lsystem->Gui_menu(&lstsytemSettings);

}

void TerrainScene::ResetLights(Light * lights[])
{
	lights[0]->SetDiffuseColour(1, 1, 1, 1);
}

void TerrainScene::GenerateDepthPass(D3D * device, Camera * camera, RenderTexture * depthMap[], Light * light[])
{
}

void TerrainScene::SceneInformationPopUp(bool * is_open)
{

}

ID3D11ShaderResourceView* TerrainScene::MakeCheckerboard(ID3D11Device *myDevice)
{


	ID3D11Texture2D*       tex;
	D3D11_TEXTURE2D_DESC   tdesc;
	D3D11_SUBRESOURCE_DATA tbsd;
						  
	const int width = 500;  
	const int height = 500;   
	const	int bpp = 3;
	const	int nb_color = 3;

//
//int g = 0;
//
//	// CREATING THE IMAGE
 	float* buf = new float[width * height * nb_color];
//	for (int i = 0; i < 3; i++)
//	{
//		for (int j = 0; j < 1; j++)
//		{
//			int k = (3 * i) + j;
//		
//			buf[g] = 0.0f;
//			buf[g+1] = 0.0f;
//			buf[g+2] = 1.0f;
//
//			g += 3;
//		}
//	}
	// pixel white
	buf[0] = 1.0f; // red
	buf[1] = 1.0f; // green
	buf[2] = 1.0f; // blue

				   // pixel black
	buf[3] = 0.0f;
	buf[4] = 0.0f;
	buf[5] = 0.0f;

	// pixel white
	buf[6] = 1.0f;
	buf[7] = 1.0f;
	buf[8] = 1.0f;

	tbsd.pSysMem = (void *)buf;
	tbsd.SysMemPitch = width * bpp;
	tbsd.SysMemSlicePitch = width * height * bpp; // Not needed since this is a 2d texture

	tdesc.Width = width;
	tdesc.Height = height;
	tdesc.MipLevels = 1;
	tdesc.ArraySize = 1;
	tdesc.SampleDesc.Count = 1;
	tdesc.SampleDesc.Quality = 0;
	tdesc.Usage = D3D11_USAGE_DEFAULT;
	tdesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
	tdesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	tdesc.CPUAccessFlags = 0;
	tdesc.MiscFlags = 0;

	//D3D11_TEXTURE2D_DESC desc;
	//desc.Width = 256;
	//desc.Height = 256;
	//desc.MipLevels = desc.ArraySize = 1;
	//desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	//desc.SampleDesc.Count = 1;
	//desc.Usage = D3D11_USAGE_DYNAMIC;
	//desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	//desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	//desc.MiscFlags = 0;

 	ID3D11Texture2D *pTexture = NULL;
	myDevice->CreateTexture2D(&tdesc, &tbsd, &tex);

	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
	shaderResourceViewDesc.Format = tdesc.Format;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	shaderResourceViewDesc.Texture2D.MipLevels = 1;
	ID3D11ShaderResourceView* m_shaderResourceView;

	;
	myDevice->CreateShaderResourceView(tex, &shaderResourceViewDesc, &m_shaderResourceView);


	delete[] buf;

	return(m_shaderResourceView);
}