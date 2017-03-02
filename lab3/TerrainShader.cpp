#include "TerrainShader.h"

 

TerrainShader::TerrainShader(ID3D11Device * device, HWND hwnd)
	: BaseShader(device, hwnd)
{
	//InitShader(L"shaders/TerrainGenerator_vs.hlsl", L"shaders/wobblyBox_hs.hlsl", L"shaders/wobblyBox_ds.hlsl", L"shaders/TerrainGenerator_ps.hlsl");
	InitShader(L"shaders/FaultLineDisplacement_vs.hlsl",   L"shaders/Terrain_ps.hlsl" );
//	InitShader(L"shaders/wobblyBox_vs.hlsl", L"shaders/wobblyBox_hs.hlsl", L"shaders/wobblyBox_ds.hlsl", L"shaders/wobblyBox_ps.hlsl");


}

 

TerrainShader::~TerrainShader()
{	// Release the sampler state.
 
 
	// Release the sampler state.
	if (m_sampleState)
	{
		m_sampleState->Release();
		m_sampleState = 0;
	}

	// Release the matrix constant buffer.
	if (m_matrixBuffer)
	{
		m_matrixBuffer->Release();
		m_matrixBuffer = 0;
	}


	 
	// Release the layout.
	if (m_layout)
	{
		m_layout->Release();
		m_layout = 0;
	}


	if (faultLineBuffer)
	{
		faultLineBuffer->Release();
		faultLineBuffer = 0;
	}
}



void TerrainShader::InitShader(WCHAR * vsFilename, WCHAR * psFilename)
{
	D3D11_BUFFER_DESC matrixBufferDesc;
	D3D11_BUFFER_DESC faultLineBufferDesc;
	D3D11_BUFFER_DESC terrainTexturingBufferDesc;
	D3D11_BUFFER_DESC lightBufferDesc;
	D3D11_BUFFER_DESC lightBufferDesc2;
	D3D11_SAMPLER_DESC samplerDesc;

	// Load (+ compile) shader files
	loadVertexShader(vsFilename);
	loadPixelShader(psFilename);

	// Setup the description of the dynamic matrix constant buffer that is in the vertex shader.
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType2);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	m_device->CreateBuffer(&matrixBufferDesc, NULL, &m_matrixBuffer);

	// Create a texture sampler state description.
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_MIRROR;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_MIRROR;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_MIRROR;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	// Create the texture sampler state.
	m_device->CreateSamplerState(&samplerDesc, &m_sampleState);

	// Required a CLAMPED sampler for sampling the depth map
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	// Create the texture sampler state.
	m_device->CreateSamplerState(&samplerDesc, &m_sampleStateClamp);



	// Setup the description of the dynamic matrix constant buffer that is in the vertex shader.
	faultLineBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	faultLineBufferDesc.ByteWidth = sizeof(FaultLineDisplacementBufferType);
	faultLineBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	faultLineBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	faultLineBufferDesc.MiscFlags = 0;
	faultLineBufferDesc.StructureByteStride = 0;

	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	m_device->CreateBuffer(&faultLineBufferDesc, NULL, &faultLineBuffer);

	// Setup the description of the dynamic matrix constant buffer that is in the vertex shader.
	terrainTexturingBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	terrainTexturingBufferDesc.ByteWidth = sizeof(TerrainSettingTextureType);
	terrainTexturingBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	terrainTexturingBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	terrainTexturingBufferDesc.MiscFlags = 0;
	terrainTexturingBufferDesc.StructureByteStride = 0;

	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	m_device->CreateBuffer(&terrainTexturingBufferDesc, NULL, &terrainTexturingBuffer);


	// Setup light buffer
	// Setup the description of the light dynamic constant buffer that is in the pixel shader.
	// Note that ByteWidth always needs to be a multiple of 16 if using D3D11_BIND_CONSTANT_BUFFER or CreateBuffer will fail.
	lightBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	lightBufferDesc.ByteWidth = sizeof(LightBufferType);
	lightBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	lightBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	lightBufferDesc.MiscFlags = 0;
	lightBufferDesc.StructureByteStride = 0;

	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	m_device->CreateBuffer(&lightBufferDesc, NULL, &m_LightBuffer);

	// Setup the description of the camera dynamic constant buffer that is in the vertex shader.
	lightBufferDesc2.Usage = D3D11_USAGE_DYNAMIC;
	lightBufferDesc2.ByteWidth = sizeof(LightBufferType2);
	lightBufferDesc2.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	lightBufferDesc2.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	lightBufferDesc2.MiscFlags = 0;
	lightBufferDesc2.StructureByteStride = 0;

	// Create the camera constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	m_device->CreateBuffer(&lightBufferDesc2, NULL, &m_LightBuffer2);


}

void TerrainShader::InitShader(WCHAR * vsFilename, WCHAR * hsFilename, WCHAR * dsFilename, WCHAR * psFilename)
{
	// InitShader must be overwritten and it will load both vertex and pixel shaders + setup buffers
	InitShader(vsFilename, psFilename);

	// Load other required shaders.

	loadHullShader(hsFilename);

	loadDomainShader(dsFilename);
}


void TerrainShader::SetShaderParameters(ID3D11DeviceContext * deviceContext, const XMMATRIX & worldMatrix, const XMMATRIX & viewMatrix, const XMMATRIX & projectionMatrix,
	ID3D11ShaderResourceView * defaultTexture, FaultLineDisplacementBufferType * faultLineSettings, TerrainSettingTextureType * terrainTextureSettings, Light * light[NUM_LIGHTS],
	ID3D11ShaderResourceView * depthMap[], ID3D11ShaderResourceView * lowTexture, ID3D11ShaderResourceView * mediumTexture, ID3D11ShaderResourceView * hightTexture)

{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
 	FaultLineDisplacementBufferType* faultLinePtr;
	MatrixBufferType2* dataPtr;
	LightBufferType* lightPtr;
	LightBufferType2* lightPtr2;
	TerrainSettingTextureType* terrainTextureSettingsPtr;
	unsigned int bufferNumber;
	XMMATRIX tworld, tview, tproj;
	XMMATRIX tLightViewMatrix[NUM_LIGHTS], tLightProjectionMatrix[NUM_LIGHTS];


	// Transpose the matrices to prepare them for the shader.
	tworld = XMMatrixTranspose(worldMatrix);
	tview = XMMatrixTranspose(viewMatrix);
	tproj = XMMatrixTranspose(projectionMatrix);
	for (int i = 0; i < NUM_LIGHTS; i++)
	{
		tLightViewMatrix[i] = XMMatrixTranspose(light[i]->GetViewMatrix());
		tLightProjectionMatrix[i] = XMMatrixTranspose(light[i]->GetProjectionMatrix());
	}
	// Lock the constant buffer so it can be written to.
	result = deviceContext->Map(m_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

	// Get a pointer to the data in the constant buffer.
	dataPtr = (MatrixBufferType2*)mappedResource.pData;

	// Copy the matrices into the constant buffer.
	dataPtr->world = tworld;// worldMatrix;
	dataPtr->view = tview;
	dataPtr->projection = tproj;
	for (int i = 0; i < NUM_LIGHTS; i++)
	{
		dataPtr->lightView[i] = tLightViewMatrix[i];

		dataPtr->lightProjection[i] = tLightProjectionMatrix[i];

	}
	// Unlock the constant buffer.
	deviceContext->Unmap(m_matrixBuffer, 0);

	// Set the position of the constant buffer in the vertex shader.
	bufferNumber = 0;

	// Now set the constant buffer in the vertex shader with the updated values.
	deviceContext->VSSetConstantBuffers(bufferNumber, 1, &m_matrixBuffer);





	//Additional
	// Send light data to pixel shader
	deviceContext->Map(m_LightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	lightPtr = (LightBufferType*)mappedResource.pData;

	for (int i = 0; i < NUM_LIGHTS; i++)
	{
		lightPtr->ambientColour[i] = light[i]->GetAmbientColour();
		lightPtr->diffuseColour[i] = light[i]->GetDiffuseColour();
		lightPtr->lightDirection[i] = XMFLOAT4(light[i]->GetDirection().x, light[i]->GetDirection().y, light[i]->GetDirection().z, 0);
		lightPtr->specularPower[i].x = light[i]->GetSpecularPower();
		lightPtr->specularColour[i] = light[i]->GetSpecularColour();
		lightPtr->position[i] = XMFLOAT4(light[i]->GetPosition().x, light[i]->GetPosition().y, light[i]->GetPosition().z, 0);
		lightPtr->willGenerateShadows[i] = (int)light[i]->GetWillGenerateShadows();

		switch (light[i]->GetLightType())
		{
		case Light::lightType::directional:
			lightPtr->lightType[i].x = true;
			lightPtr->lightType[i].y = false;
			lightPtr->lightType[i].z = false;
			break;


		case Light::lightType::point:
			lightPtr->lightType[i].x = false;
			lightPtr->lightType[i].y = true;
			lightPtr->lightType[i].z = false;
			break;


		case Light::lightType::spot:
			lightPtr->lightType[i].x = false;
			lightPtr->lightType[i].y = false;
			lightPtr->lightType[i].z = true;
			break;
		default:
			break;
		}


		lightPtr->attenuationValues[i] = XMFLOAT4(light[i]->GetAttenuationContantFactor(),
			light[i]->GetAttenuationLinearFactor(),
			light[i]->GetAttenuationQuadraticFactor(),
			light[i]->GetRange());
		lightPtr->isSpecular[i] = light[i]->GetMakesSpecular();

	}
	deviceContext->Unmap(m_LightBuffer, 0);
	bufferNumber = 0;
	deviceContext->PSSetConstantBuffers(bufferNumber, 1, &m_LightBuffer);

	// Send light data to vertex shader
	deviceContext->Map(m_LightBuffer2, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	lightPtr2 = (LightBufferType2*)mappedResource.pData;

	for (int i = 0; i < NUM_LIGHTS; i++)
	{
		lightPtr2->position[i] = XMFLOAT4(light[i]->GetPosition().x, light[i]->GetPosition().y, light[i]->GetPosition().z, 0);
	}
	deviceContext->Unmap(m_LightBuffer2, 0);
	bufferNumber = 2;
	deviceContext->VSSetConstantBuffers(bufferNumber, 1, &m_LightBuffer2);









	// Send light data to vertex shader
	deviceContext->Map(terrainTexturingBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	terrainTextureSettingsPtr = (TerrainSettingTextureType*)mappedResource.pData;

	terrainTextureSettingsPtr->displayNormalMap = terrainTextureSettings->displayNormalMap;
	terrainTextureSettingsPtr->padding = XMINT3(0, 0, 0);
	deviceContext->Unmap(terrainTexturingBuffer, 0);
	bufferNumber = 1;
	deviceContext->PSSetConstantBuffers(bufferNumber, 1, &terrainTexturingBuffer);







	result = deviceContext->Map(faultLineBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

	// Get a pointer to the data in the constant buffer.
	faultLinePtr = (FaultLineDisplacementBufferType*)mappedResource.pData;

	faultLinePtr->minimumDisplacement = faultLineSettings->minimumDisplacement;
	faultLinePtr->smoothingValue = faultLineSettings->smoothingValue;
	faultLinePtr->enableFaultLineDisplacement = faultLineSettings->enableFaultLineDisplacement;
	faultLinePtr->padding = XMFLOAT3(0, 0, 0);
	faultLinePtr->numberOfIterations = faultLineSettings->numberOfIterations;
	faultLinePtr->startingDisplacement = faultLineSettings->startingDisplacement;
	for (int i = 0; i < MAX_FAULTLINE_ITERATIONS; i++)
	{
		if (i < faultLineSettings->numberOfIterations)
		{
			faultLinePtr->interationsRandomPoints[i] = faultLineSettings->interationsRandomPoints[i];
		}
		else
		{
			faultLinePtr->interationsRandomPoints[i] = XMINT4(0, 0, 0, 0);
		}
	}
	deviceContext->Unmap(faultLineBuffer, 0);
	bufferNumber = 3;

	deviceContext->VSSetConstantBuffers(bufferNumber, 1, &faultLineBuffer);




	// Set shader texture resource in the pixel shader.
	deviceContext->PSSetShaderResources(0, 1, &defaultTexture);

	deviceContext->PSSetShaderResources(1, 1, &lowTexture);
	deviceContext->PSSetShaderResources(2, 1, &mediumTexture);
	deviceContext->PSSetShaderResources(3, 1, &hightTexture); 


	deviceContext->PSSetShaderResources(4, 1, (&depthMap[0]));
	deviceContext->PSSetShaderResources(5, 1, (&depthMap[1]));
	deviceContext->PSSetShaderResources(6, 1, (&depthMap[2]));
	deviceContext->PSSetShaderResources(7, 1, (&depthMap[3]));

}


 
 

void TerrainShader::Render(ID3D11DeviceContext * deviceContext, int indexCount)
{

	// Set the sampler state in the pixel shader.
	deviceContext->PSSetSamplers(0, 1, &m_sampleState);
	deviceContext->PSSetSamplers(1, 1, &m_sampleStateClamp);

	// Base render function.
	BaseShader::Render(deviceContext, indexCount);
}
