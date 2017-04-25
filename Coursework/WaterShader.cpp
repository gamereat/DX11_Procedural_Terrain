#include "WaterShader.h"

WaterShader::WaterShader(ID3D11Device* device, HWND hwnd) : BaseShader(device, hwnd)
{
	// Load in shaders
	InitShader(L"shaders/water_vs.hlsl",L"shaders/water_ps.hlsl");

}


WaterShader::~WaterShader()
{

	// Clean up memory

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

	if (waterBuffer)
	{
		waterBuffer->Release();
		waterBuffer = nullptr;
	}

 	if (layout)
	{
		layout->Release();
		layout = nullptr;
	}

	if (waterTextureBuffer)
	{
		waterTextureBuffer->Release();
		waterTextureBuffer = nullptr;
	}

	//Release base shader components
	BaseShader::~BaseShader();
}
 

void WaterShader::InitShader(WCHAR* vsFilename, WCHAR* psFilename)
{
	D3D11_BUFFER_DESC matrixBufferDesc;
	D3D11_SAMPLER_DESC samplerDesc;
	D3D11_BUFFER_DESC waveDesc;
	D3D11_BUFFER_DESC waveTexDesc;

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

	

	waveDesc.Usage = D3D11_USAGE_DYNAMIC;
	waveDesc.ByteWidth = sizeof(WavetBufferType);
	waveDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	waveDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	waveDesc.MiscFlags = 0;
	waveDesc.StructureByteStride = 0;

	m_device->CreateBuffer(&waveDesc, NULL, &waterBuffer);


	waveTexDesc.Usage = D3D11_USAGE_DYNAMIC;
	waveTexDesc.ByteWidth = sizeof(WavetBufferType);
	waveTexDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	waveTexDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	waveTexDesc.MiscFlags = 0;
	waveTexDesc.StructureByteStride = 0;

	m_device->CreateBuffer(&waveTexDesc, NULL, &waterTextureBuffer);
	
	// Create the texture sampler state. 

	 


	// Required a CLAMPED sampler for sampling the depth map
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	// Create the texture sampler state.
	m_device->CreateSamplerState(&samplerDesc, &sampleStateClamp);


}

void WaterShader::InitShader(WCHAR* vsFilename, WCHAR* hsFilename, WCHAR* dsFilename, WCHAR* psFilename)
{

	// InitShader must be overwritten and it will load both vertex and pixel shaders + setup buffers
	InitShader(vsFilename, psFilename);

	// Load other required shaders.
	loadHullShader(hsFilename);
	loadDomainShader(dsFilename);


}


void WaterShader::SetShaderParameters(ID3D11DeviceContext * deviceContext, const XMMATRIX & worldMatrix, 
	const XMMATRIX & viewMatrix, const XMMATRIX & projectionMatrix, ID3D11ShaderResourceView * texture, 
	WavetBufferType * waveInfo, float waveAlphaBend)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBufferType* dataPtr;
	WavetBufferType* waterPtr;
	WaveTextureBuffer* waveTexPtr;
	unsigned int bufferNumber;
	XMMATRIX tworld, tview, tproj;

	// Transpose the matrices to prepare them for the shader.
	tworld = XMMatrixTranspose(worldMatrix);
	tview = XMMatrixTranspose(viewMatrix);
	tproj = XMMatrixTranspose(projectionMatrix);

	// Lock the constant buffer so it can be written to.
	result = deviceContext->Map(matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

	// Get a pointer to the data in the constant buffer.
	dataPtr = (MatrixBufferType*)mappedResource.pData;

	// Copy the matrices into the constant buffer.
	dataPtr->world = tworld;// worldMatrix;
	dataPtr->view = tview;
	dataPtr->projection = tproj;

	// Unlock the constant buffer.
	deviceContext->Unmap(matrixBuffer, 0);

	// Set the position of the constant buffer in the vertex shader.
	bufferNumber = 0;

	// Now set the constant buffer in the vertex shader with the updated values.
	deviceContext->VSSetConstantBuffers(bufferNumber, 1, &matrixBuffer);

	// Set the position of the constant buffer in the vertex shader.
	bufferNumber = 0;

	deviceContext->Map(waterBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	waterPtr = (WavetBufferType*)mappedResource.pData;

	waterPtr->speed = waveInfo->speed;
	waterPtr->amplutude = waveInfo->amplutude;
	waterPtr->freqancy = waveInfo->freqancy;
	waterPtr->steepnesss = waveInfo->steepnesss;
	waterPtr->time = waveInfo->time;
	waterPtr->padding = 0;
	waterPtr->padding = float(0);

	//	 Unlock the constant buffer.
	deviceContext->Unmap(waterBuffer, 0);

	//	 Set the position of the constant buffer in the vertex shader.
	bufferNumber = 1;

	//	 Now set the constant buffer in the vertex shader with the updated values.
	deviceContext->VSSetConstantBuffers(bufferNumber, 1, &waterBuffer);
	

	deviceContext->Map(waterTextureBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	waveTexPtr = (WaveTextureBuffer*)mappedResource.pData;


	waveTexPtr->alphaValue = waveAlphaBend;
	waveTexPtr->padding = XMFLOAT3(0, 0, 0);
	//	 Unlock the constant buffer.
	deviceContext->Unmap(waterTextureBuffer, 0);

	//	 Set the position of the constant buffer in the vertex shader.
	bufferNumber = 1;

	//	 Now set the constant buffer in the vertex shader with the updated values.
	deviceContext->PSSetConstantBuffers(0, 1, &waterTextureBuffer);
	// Set shader texture resource in the pixel shader.
	deviceContext->PSSetShaderResources(0, 1, &texture);


}

void WaterShader::Render(ID3D11DeviceContext* deviceContext, int indexCount)
{
	// Set the sampler state in the pixel shader.
	deviceContext->PSSetSamplers(0, 1, &sampleState);
	deviceContext->PSSetSamplers(1, 1, &sampleStateClamp);

	// Base render function.

	BaseShader::Render(deviceContext, indexCount);
}

 