#include "GeomentryShader.h"


 
GeomentryShader::GeomentryShader(ID3D11Device* device, HWND hwnd) : BaseShader(device, hwnd)
{
	InitShader(L"shaders/Geometry_Triangle_vs.hlsl", L"shaders/Geometry_Triangle_ps.hlsl", L"shaders/Geometry_Triangle_gs.hlsl");


}




GeomentryShader::~GeomentryShader()
{
	// Release the sampler state.
	if (m_sampleState)
	{
		m_sampleState->Release();
		m_sampleState = 0;
	}
	if (m_GeomentryBuffer)
	{
		m_GeomentryBuffer->Release();
		m_GeomentryBuffer = 0;
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

	//Release base shader components
	BaseShader::~BaseShader();
}

void GeomentryShader::InitShader(WCHAR* vsFilename, WCHAR* psFilename)
{
	D3D11_BUFFER_DESC matrixBufferDesc;
	D3D11_SAMPLER_DESC samplerDesc;
	D3D11_BUFFER_DESC GeomentryBufferDesc;

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
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
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


	GeomentryBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	GeomentryBufferDesc.ByteWidth = sizeof(GeomentryBufferType);
	GeomentryBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	GeomentryBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	GeomentryBufferDesc.MiscFlags = 0;
	GeomentryBufferDesc.StructureByteStride = 0;

	// Create the camera constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	m_device->CreateBuffer(&GeomentryBufferDesc, NULL, &m_GeomentryBuffer);
}

void GeomentryShader::InitShader(WCHAR * vsFilename, WCHAR * psFilename, WCHAR * gsFilename)
{
	// InitShader must be overwritten and it will load both vertex and pixel shaders + setup buffers
	InitShader(vsFilename, psFilename);

	// Load other required shaders.
	loadGeometryShader(gsFilename);

}

void GeomentryShader::SetShaderParameters(ID3D11DeviceContext * deviceContext, const XMMATRIX & worldMatrix, const XMMATRIX & viewMatrix, const XMMATRIX & projectionMatrix, ID3D11ShaderResourceView * texture, GeomentryBufferType explosiveSettings)
{

	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBufferType* dataPtr;
	unsigned int bufferNumber;
	XMMATRIX tworld, tview, tproj;
	GeomentryBufferType* geoPtr;
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
	deviceContext->GSSetConstantBuffers(bufferNumber, 1, &m_matrixBuffer);
	deviceContext->VSSetConstantBuffers(bufferNumber, 1, &m_matrixBuffer);


	// Set shader texture resource in the pixel shader.
	deviceContext->PSSetShaderResources(0, 1, &texture);





	//Additional
	// Send light data to pixel shader
	deviceContext->Map(m_GeomentryBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	geoPtr = (GeomentryBufferType*)mappedResource.pData; 

	geoPtr->explosiveAmmount = explosiveSettings.explosiveAmmount;
	geoPtr->gravity = explosiveSettings.gravity;
	geoPtr->explosiveAcceleration = explosiveSettings.explosiveAcceleration;
	geoPtr->time = explosiveSettings.time;
	geoPtr->maxTime = explosiveSettings.maxTime;
	geoPtr->explotionMaximums = explosiveSettings.explotionMaximums;
 	deviceContext->Unmap(m_GeomentryBuffer, 0);
	bufferNumber = 1;
	deviceContext->GSSetConstantBuffers(bufferNumber, 1, &m_GeomentryBuffer);

}



void GeomentryShader::Render(ID3D11DeviceContext* deviceContext, int indexCount)
{

	// Set the sampler state in the pixel shader.
	deviceContext->PSSetSamplers(0, 1, &m_sampleState);

	// Base render function.
	BaseShader::Render(deviceContext, indexCount);
}



