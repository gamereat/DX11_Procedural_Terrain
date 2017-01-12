// texture shader.cpp
#include "VertexShader.h"
#include "../imgui/imgui.h"

VertexShader::VertexShader(ID3D11Device* device, HWND hwnd) : BaseShader(device, hwnd)
{
	InitShader(L"shaders/vertexManipulationSpec_vs.hlsl", L"shaders/vertexManipulationSpec_ps.hlsl");
}


VertexShader::~VertexShader()
{
	if (m_camBuffer)
	{
		m_camBuffer->Release();
		m_camBuffer = 0;
	}
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

	// Release the light constant buffer.
	if (m_lightBuffer)
	{
		m_lightBuffer->Release();
		m_lightBuffer = 0;
	}

	//Release base shader components
	BaseShader::~BaseShader();
}


void VertexShader::InitShader(WCHAR* vsFilename, WCHAR* psFilename)
{
	D3D11_BUFFER_DESC matrixBufferDesc;
	D3D11_SAMPLER_DESC samplerDesc;
	D3D11_BUFFER_DESC lightBufferDesc;
	D3D11_BUFFER_DESC camBufferDesc;
	D3D11_BUFFER_DESC timeBufferDesc;

	// Load (+ compile) shader files
	loadVertexShader(vsFilename);
	loadPixelShader(psFilename);

	// Setup the description of the dynamic matrix constant buffer that is in the vertex shader.
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
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

	// Setup light buffer
	// Setup the description of the light dynamic constant buffer that is in the pixel shader.
	// Note that ByteWidth always needs to be a multiple of 16 if using D3D11_BIND_CONSTANT_BUFFER or CreateBuffer will fail.
	lightBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	lightBufferDesc.ByteWidth = sizeof(LightBufferType);
	lightBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	lightBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	lightBufferDesc.MiscFlags = 0;
	lightBufferDesc.StructureByteStride = 0;


	 
	camBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	camBufferDesc.ByteWidth = sizeof(CammeraBufferType);
	camBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	camBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	camBufferDesc.MiscFlags = 0;
	camBufferDesc.StructureByteStride = 0;


	timeBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	timeBufferDesc.ByteWidth = sizeof(TimeBufferType);
	timeBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	timeBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	timeBufferDesc.MiscFlags = 0;
	timeBufferDesc.StructureByteStride = 0;

	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	m_device->CreateBuffer(&lightBufferDesc, NULL, &m_lightBuffer);
	m_device->CreateBuffer(&camBufferDesc, NULL, &m_camBuffer);
	m_device->CreateBuffer(&timeBufferDesc, NULL, &m_timeBuffer);

}


void VertexShader::SetShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX &worldMatrix, const XMMATRIX &viewMatrix, const XMMATRIX &projectionMatrix, ID3D11ShaderResourceView* texture, Light* light[], XMFLOAT3 cammeraPostion, float delatTime,float vertexFequancy, float vertexHeight, typeOfVertexMinimulation typeOfMinimulation)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBufferType* dataPtr;
	LightBufferType* lightPtr;
	CammeraBufferType* cammeraPtr;
	TimeBufferType* timePtr;
	unsigned int bufferNumber;
	XMMATRIX tworld, tview, tproj;


	// Transpose the matrices to prepare them for the shader.
	tworld = XMMatrixTranspose(worldMatrix);
	tview = XMMatrixTranspose(viewMatrix);
	tproj = XMMatrixTranspose(projectionMatrix);

	// Lock the constant buffer so it can be written to.
	result = deviceContext->Map(m_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

	// Get a pointer to the data in the constant buffer.
	dataPtr = (MatrixBufferType*)mappedResource.pData;

	// Copy the matrices into the constant buffer.
	dataPtr->world = tworld;// worldMatrix;
	dataPtr->view = tview;
	dataPtr->projection = tproj;

	// Unlock the constant buffer.
	deviceContext->Unmap(m_matrixBuffer, 0);

	// Set the position of the constant buffer in the vertex shader.
	bufferNumber = 0;

	// Now set the constant buffer in the vertex shader with the updated values.
	deviceContext->VSSetConstantBuffers(bufferNumber, 1, &m_matrixBuffer);
	//Additional
	// Send light data to pixel shader
	deviceContext->Map(m_lightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	lightPtr = (LightBufferType*)mappedResource.pData;
	for (int i = 0; i < NUM_LIGHTS; i++)
	{


		lightPtr->diffuse[i] = light[i]->GetDiffuseColour();
		lightPtr->ambient[i] = light[i]->GetAmbientColour();
		lightPtr->direction[i] = XMFLOAT4(light[i]->GetDirection().x, light[i]->GetDirection().y, light[i]->GetDirection().z, 0);
		lightPtr->specularPower[i].x = light[i]->GetSpecularPower();
		lightPtr->specularColour[i] = light[i]->GetSpecularColour();
		lightPtr->position[i] = XMFLOAT4(light[i]->GetPosition().x, light[i]->GetPosition().y, light[i]->GetPosition().z, 0);
		//	lightPtr->padding[i] = 0;

		switch (light[i]->GetLightType())
		{
		case Light::lightType::directional:
			lightPtr->isDirectionalLight[i].x = true;
			lightPtr->isSpotLight[i].x = false;
			lightPtr->isPointLight[i].x = false;
			break;


		case Light::lightType::point:
			lightPtr->isDirectionalLight[i].x = false;
			lightPtr->isSpotLight[i].x = false;
			lightPtr->isPointLight[i].x = true;
			break;


		case Light::lightType::spot:
			lightPtr->isDirectionalLight[i].x = false;
			lightPtr->isSpotLight[i].x = true;
			lightPtr->isPointLight[i].x = false;
			break;
		default:
			break;
		}
		lightPtr->isSpecular[i].x = light[i]->GetMakesSpecular();

		lightPtr->constantAttenuationFactor[i].x = light[i]->GetAttenuationContantFactor();
		lightPtr->linearAttenuationFactor[i].x = light[i]->GetAttenuationLinearFactor();
		lightPtr->quadraticAttenuationFactor[i].x = light[i]->GetAttenuationQuadraticFactor();
		lightPtr->range[i].x = light[i]->GetRange();


	}
	deviceContext->Unmap(m_lightBuffer, 0);
	bufferNumber = 0;

	deviceContext->PSSetConstantBuffers(bufferNumber, 1, &m_lightBuffer);
	deviceContext->Map(m_camBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

	cammeraPtr = (CammeraBufferType*)mappedResource.pData;
	cammeraPtr->cammeraPostion = cammeraPostion;
		
	cammeraPtr->padding = 0.0f;
	deviceContext->Unmap(m_camBuffer, 0);
	bufferNumber = 1;
	deviceContext->VSSetConstantBuffers(bufferNumber, 1, &m_camBuffer);

	deviceContext->Map(m_timeBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	timePtr = (TimeBufferType*)mappedResource.pData;
	timePtr->delatTime = delatTime;
 	timePtr->height = vertexHeight;
	timePtr->frequancy = vertexFequancy;

	switch (typeOfMinimulation)
	{
	case VertexShader::sin:
		timePtr->isCosWave = false;
		timePtr->isSinWave = true;
		timePtr->isThirdWave = false;
		timePtr->isForthWave = false;
		timePtr->isFithWave = false;

		break;
	case VertexShader::cos:
		timePtr->isCosWave = true;
		timePtr->isSinWave = false;
		timePtr->isThirdWave = false;
		timePtr->isForthWave = false;
		timePtr->isFithWave = false;

		break;
	case VertexShader::third:
		timePtr->isCosWave = false;
		timePtr->isSinWave = false;
		timePtr->isThirdWave = true;
		timePtr->isForthWave = false;
		timePtr->isFithWave = false;

		break;
	case VertexShader::forth:
		timePtr->isCosWave = false;
		timePtr->isSinWave = false;
		timePtr->isThirdWave = false;
		timePtr->isForthWave = true;
		timePtr->isFithWave = false;

		break;
	case VertexShader::fith:
		timePtr->isCosWave = false;
		timePtr->isSinWave = false;
		timePtr->isThirdWave = false;
		timePtr->isForthWave = false;
		timePtr->isFithWave = true;

		break;
	default:
		break;
	}
	deviceContext->Unmap(m_timeBuffer, 0);

	bufferNumber = 2;
	deviceContext->VSSetConstantBuffers(bufferNumber, 1, &m_timeBuffer);

	// Set shader texture resource in the pixel shader.
	deviceContext->PSSetShaderResources(0, 1, &texture);
}

void VertexShader::Render(ID3D11DeviceContext* deviceContext, int indexCount)
{
	// Set the sampler state in the pixel shader.
	deviceContext->PSSetSamplers(0, 1, &m_sampleState);

	// Base render function.
	BaseShader::Render(deviceContext, indexCount);
}



