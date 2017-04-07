#include "TerrainShader.h"

 

TerrainShader::TerrainShader(ID3D11Device * device, HWND hwnd) : BaseShader(device, hwnd)
{
	// Load in Shaders
 	InitShader(L"shaders/Terrain_vs.hlsl",   L"shaders/Terrain_ps.hlsl" );
 
}

 

TerrainShader::~TerrainShader()
{	
	// Release memory

	if (sampleState)
	{
		sampleState->Release();
		sampleState = nullptr;
	}

	if (matrixBuffer)
	{
		matrixBuffer->Release();
		matrixBuffer = nullptr;
	}
	
	if (sampleStateClamp)
	{
		sampleStateClamp->Release();
		sampleStateClamp = nullptr;
	}
	
	if (lightBuffer)
	{
		lightBuffer->Release();
		lightBuffer = nullptr;
	}

	if (secondaryLightBuffer)
	{
		secondaryLightBuffer->Release();
		secondaryLightBuffer = nullptr;
	}

	if (terrainTexturingBuffer)
	{
		terrainTexturingBuffer->Release();
		terrainTexturingBuffer = nullptr;
	}

	if (fractionalBrowningNoiseBuffer)
	{
		fractionalBrowningNoiseBuffer->Release();
		fractionalBrowningNoiseBuffer = nullptr;
	}

	if (faultLineBuffer)
	{
		faultLineBuffer->Release();
		faultLineBuffer = nullptr;
	}

	if (terrainGenerationBuffer)
	{
		terrainGenerationBuffer->Release();
	}
	if (layout)
	{
		layout->Release();
		layout = nullptr;
	}


	if (faultLineBuffer)
	{
		faultLineBuffer->Release();
		faultLineBuffer = nullptr;
	}
}



void TerrainShader::InitShader(WCHAR * vsFilename, WCHAR * psFilename)
{
	D3D11_BUFFER_DESC matrixBufferDesc;
	D3D11_BUFFER_DESC faultLineBufferDesc;
	D3D11_BUFFER_DESC fractalBrowningNoiseDesc;
	D3D11_BUFFER_DESC terrainTexturingBufferDesc;
	D3D11_BUFFER_DESC dimondSquareDesc;
	D3D11_BUFFER_DESC terrainBufferDesc;
	D3D11_BUFFER_DESC lightBufferDesc;
	D3D11_BUFFER_DESC secondaryLightBufferDesc;
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
	m_device->CreateBuffer(&matrixBufferDesc, NULL, &matrixBuffer);

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
	m_device->CreateSamplerState(&samplerDesc, &sampleState);

	// Required a CLAMPED sampler for sampling the depth map
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	// Create the texture sampler state.
	m_device->CreateSamplerState(&samplerDesc, &sampleStateClamp);



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
	terrainBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	terrainBufferDesc.ByteWidth = sizeof(TerrainGenerationBufferType);
	terrainBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	terrainBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	terrainBufferDesc.MiscFlags = 0;
	terrainBufferDesc.StructureByteStride = 0;

	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	m_device->CreateBuffer(&terrainBufferDesc, NULL, &terrainGenerationBuffer);

	
	// Setup the description of the dynamic matrix constant buffer that is in the vertex shader.
	terrainTexturingBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	terrainTexturingBufferDesc.ByteWidth = sizeof(TerrainSettingTextureType);
	terrainTexturingBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	terrainTexturingBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	terrainTexturingBufferDesc.MiscFlags = 0;
	terrainTexturingBufferDesc.StructureByteStride = 0;

	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	m_device->CreateBuffer(&terrainTexturingBufferDesc, NULL, &terrainTexturingBuffer);

	// Setup the description of the dynamic matrix constant buffer that is in the vertex shader.
	fractalBrowningNoiseDesc.Usage = D3D11_USAGE_DYNAMIC;
	fractalBrowningNoiseDesc.ByteWidth = sizeof(FractionalBrowningNoiseBuffer);
	fractalBrowningNoiseDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	fractalBrowningNoiseDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	fractalBrowningNoiseDesc.MiscFlags = 0;
	fractalBrowningNoiseDesc.StructureByteStride = 0;

	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	m_device->CreateBuffer(&fractalBrowningNoiseDesc, NULL, &fractionalBrowningNoiseBuffer);


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
	m_device->CreateBuffer(&lightBufferDesc, NULL, &lightBuffer);

	// Setup the description of the camera dynamic constant buffer that is in the vertex shader.
	secondaryLightBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	secondaryLightBufferDesc.ByteWidth = sizeof(LightBufferType2);
	secondaryLightBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	secondaryLightBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	secondaryLightBufferDesc.MiscFlags = 0;
	secondaryLightBufferDesc.StructureByteStride = 0;

	// Create the camera constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	m_device->CreateBuffer(&secondaryLightBufferDesc, NULL, &secondaryLightBuffer);


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
	ID3D11ShaderResourceView * defaultTexture, TerrainGenerationBufferType * terrinSetting,
	FaultLineDisplacementBufferType * faultLineSettings, TerrainSettingTextureType * terrainTextureSettings, 
	FractionalBrowningNoiseBuffer * fractionalBrowningNoiseSettings, Light * light[NUM_LIGHTS], ID3D11ShaderResourceView * depthMap[],
	ID3D11ShaderResourceView * lowTexture, ID3D11ShaderResourceView * mediumTexture, ID3D11ShaderResourceView * hightTexture, 
	ID3D11ShaderResourceView* underWaterTexture, ID3D11ShaderResourceView* hitByWaterTexture)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
 	FaultLineDisplacementBufferType* faultLinePtr;
	FractionalBrowningNoiseBuffer* fbnPtr;
	TerrainGenerationBufferType* terrinGenPtr;
 	MatrixBufferType2* dataPtr;
	LightBufferType* lightPtr;
	LightBufferType2* lightPtr2;
	TerrainSettingTextureType* terrainTextureSettingsPtr;
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
	result = deviceContext->Map(matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

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
	deviceContext->Unmap(matrixBuffer, 0);
 

	// Now set the constant buffer in the vertex shader with the updated values.
	deviceContext->VSSetConstantBuffers(0, 1, &matrixBuffer);





	//Additional
	// Send light data to pixel shader
	deviceContext->Map(lightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
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
	deviceContext->Unmap(lightBuffer, 0);
 	deviceContext->PSSetConstantBuffers(0, 1, &lightBuffer);

	// Send light data to vertex shader
	deviceContext->Map(secondaryLightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	lightPtr2 = (LightBufferType2*)mappedResource.pData;

	for (int i = 0; i < NUM_LIGHTS; i++)
	{
		lightPtr2->position[i] = XMFLOAT4(light[i]->GetPosition().x, light[i]->GetPosition().y, light[i]->GetPosition().z, 0);
	}
	deviceContext->Unmap(secondaryLightBuffer, 0);
 	deviceContext->VSSetConstantBuffers(2, 1, &secondaryLightBuffer);



	// Send light data to vertex shader
	deviceContext->Map(terrainGenerationBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	terrinGenPtr = (TerrainGenerationBufferType*)mappedResource.pData;
	terrinGenPtr->highScale = terrinSetting->highScale;
	terrinGenPtr->enableGPUEffect = terrinSetting->enableGPUEffect;
	terrinGenPtr->terrainGenerationType = terrinSetting->terrainGenerationType;
	terrinGenPtr->padding = 0;
	deviceContext->Unmap(terrainGenerationBuffer, 0);
 	deviceContext->VSSetConstantBuffers(3, 1, &terrainGenerationBuffer);
	 

	// Load in Terrain Texturing settings
	
 	deviceContext->Map(terrainTexturingBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	terrainTextureSettingsPtr = (TerrainSettingTextureType*)mappedResource.pData;

	terrainTextureSettingsPtr->displayNormalMap = terrainTextureSettings->displayNormalMap;
	terrainTextureSettingsPtr->enableLighting = terrainTextureSettings->enableLighting;
	terrainTextureSettingsPtr->textureTiling = terrainTextureSettings->textureTiling;
	terrainTextureSettingsPtr->maxHeightOfWater = terrainTextureSettings->maxHeightOfWater;
	terrainTextureSettingsPtr->medHeightOfWater = terrainTextureSettings->medHeightOfWater;
	terrainTextureSettingsPtr->minHeightOfWater = terrainTextureSettings->minHeightOfWater;
	terrainTextureSettingsPtr->maxHightOfHill = terrainTextureSettings->maxHightOfHill;

	terrainTextureSettingsPtr->topHighPercentage = terrainTextureSettings->topHighPercentage;
	terrainTextureSettingsPtr->midHighPercentage = terrainTextureSettings->midHighPercentage;
	terrainTextureSettingsPtr->blendingPercentage = terrainTextureSettings->blendingPercentage;
	terrainTextureSettingsPtr->minHightOfTerrain = terrainTextureSettings->minHightOfTerrain;



	terrainTextureSettingsPtr->padding = 0;
 	deviceContext->Unmap(terrainTexturingBuffer, 0);
 	deviceContext->PSSetConstantBuffers(1, 1, &terrainTexturingBuffer);




	// Load in fbn settings

 	deviceContext->Map(fractionalBrowningNoiseBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	fbnPtr = (FractionalBrowningNoiseBuffer*)mappedResource.pData;
 
	fbnPtr->fbnAmplitude = fractionalBrowningNoiseSettings->fbnAmplitude;
	fbnPtr->fbnFrequancy = fractionalBrowningNoiseSettings->fbnFrequancy;
	fbnPtr->fbnLacunarity = fractionalBrowningNoiseSettings->fbnLacunarity;
	fbnPtr->fbnOctaves= fractionalBrowningNoiseSettings->fbnOctaves;
	fbnPtr->fbnPadding = 0;
	fbnPtr->heightScale = fractionalBrowningNoiseSettings->heightScale;
	fbnPtr->fbnPelinNoiseFreqnacy = fractionalBrowningNoiseSettings->fbnPelinNoiseFreqnacy;
	fbnPtr->fbnPersistence  = fractionalBrowningNoiseSettings->fbnPersistence;
	deviceContext->Unmap(fractionalBrowningNoiseBuffer, 0);
 	deviceContext->VSSetConstantBuffers(5, 1, &fractionalBrowningNoiseBuffer);




	// load in fault line settings

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
 
	deviceContext->VSSetConstantBuffers(4, 1, &faultLineBuffer);




	// Set shader texture resource in the pixel shader.
	deviceContext->PSSetShaderResources(0, 1, &defaultTexture);

	deviceContext->PSSetShaderResources(1, 1, &lowTexture);
	deviceContext->PSSetShaderResources(2, 1, &mediumTexture);
	deviceContext->PSSetShaderResources(3, 1, &hightTexture);
	deviceContext->PSSetShaderResources(4, 1, &underWaterTexture);
	deviceContext->PSSetShaderResources(5, 1, &hitByWaterTexture);


	deviceContext->PSSetShaderResources(6, 1, (&depthMap[0]));
	deviceContext->PSSetShaderResources(7, 1, (&depthMap[1]));
	deviceContext->PSSetShaderResources(8, 1, (&depthMap[2]));
	deviceContext->PSSetShaderResources(9, 1, (&depthMap[3]));

}


 
 

 
void TerrainShader::Render(ID3D11DeviceContext * deviceContext, int indexCount)
{

	// Set the sampler state in the pixel shader.
	deviceContext->PSSetSamplers(0, 1, &sampleState);
	deviceContext->PSSetSamplers(1, 1, &sampleStateClamp);

	// Base render function.
	BaseShader::Render(deviceContext, indexCount);
}
