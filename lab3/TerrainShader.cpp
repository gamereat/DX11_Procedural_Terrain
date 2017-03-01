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
	D3D11_SAMPLER_DESC samplerDesc;

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



	// Setup the description of the dynamic matrix constant buffer that is in the vertex shader.
	faultLineBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	faultLineBufferDesc.ByteWidth = sizeof(FaultLineDisplacementBufferType);
	faultLineBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	faultLineBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	faultLineBufferDesc.MiscFlags = 0;
	faultLineBufferDesc.StructureByteStride = 0;

	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	m_device->CreateBuffer(&faultLineBufferDesc, NULL, &faultLineBuffer);




}

void TerrainShader::InitShader(WCHAR * vsFilename, WCHAR * hsFilename, WCHAR * dsFilename, WCHAR * psFilename)
{
	// InitShader must be overwritten and it will load both vertex and pixel shaders + setup buffers
	InitShader(vsFilename, psFilename);

	// Load other required shaders.

	loadHullShader(hsFilename);

	loadDomainShader(dsFilename);
}


void TerrainShader::SetShaderParameters(ID3D11DeviceContext * deviceContext, const XMMATRIX & worldMatrix, const XMMATRIX & viewMatrix, const XMMATRIX & projectionMatrix, ID3D11ShaderResourceView * texture, FaultLineDisplacementBufferType* faultLineSettings)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBufferType* dataPtr;
	FaultLineDisplacementBufferType* faultLinePtr;
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

	// Set shader texture resource in the pixel shader.
	deviceContext->PSSetShaderResources(0, 1, &texture);




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
	bufferNumber = 1;

	deviceContext->VSSetConstantBuffers(bufferNumber, 1, &faultLineBuffer);

}




void TerrainShader::Render(ID3D11DeviceContext * deviceContext, int indexCount)
{

	// Set the sampler state in the pixel shader.
	deviceContext->PSSetSamplers(0, 1, &m_sampleState);
	deviceContext->PSSetSamplers(1, 1, &sampleStateClamp);

	// Base render function.
	BaseShader::Render(deviceContext, indexCount);
}
