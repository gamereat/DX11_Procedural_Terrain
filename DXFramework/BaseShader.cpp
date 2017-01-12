// base shader.cpp
#include "baseshader.h"


BaseShader::BaseShader(ID3D11Device* device, HWND hwnd)
{
	m_device = device;
	m_hwnd = hwnd;
}


BaseShader::~BaseShader()
{
	// Shutdown the vertex and pixel shaders as well as the related objects.
	// Release the pixel shader.
	if (m_pixelShader)
	{
		m_pixelShader->Release();
		m_pixelShader = 0;
	}

	// Release the vertex shader.
	if (m_vertexShader)
	{
		m_vertexShader->Release();
		m_vertexShader = 0;
	}

	// Release the hull shader.
	if (m_hullShader)
	{
		m_hullShader->Release();
		m_hullShader = 0;
	}

	// Release the domain shader.
	if (m_domainShader)
	{
		m_domainShader->Release();
		m_domainShader = 0;
	}

	// Release the geometry shader.
	if (m_geometryShader)
	{
		m_geometryShader->Release();
		m_geometryShader = 0;
	}
}


void BaseShader::loadVertexShader(WCHAR* filename)
{

	HRESULT result;
	ID3DBlob* errorMessage;
	ID3DBlob* vertexShaderBuffer;
	D3D11_INPUT_ELEMENT_DESC polygonLayout[3];
	unsigned int numElements;
	
	// Initialize the pointers this function will use to null.
	errorMessage = 0;
	vertexShaderBuffer = 0;

	// Compile the vertex shader code.
	result = D3DCompileFromFile(filename, NULL, NULL, "main", "vs_5_0", D3DCOMPILE_ENABLE_STRICTNESS, 0, &vertexShaderBuffer, &errorMessage);
	if (FAILED(result))
	{
		// If the shader failed to compile it should have writen something to the error message.
		if (errorMessage)
		{
			OutputShaderErrorMessage(errorMessage, m_hwnd, filename);
		}
		// If there was nothing in the error message then it simply could not find the shader file itself.
		else
		{
			MessageBox(m_hwnd, filename, L"Missing Shader File", MB_OK);
		}
		exit(0);
	}

	// Create the vertex shader from the buffer.
	result = m_device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &m_vertexShader);
	if (FAILED(result))
	{
		//return false;
	}

	// Create the vertex input layout description.
	// This setup needs to match the VertexType stucture in the MeshClass and in the shader.
	polygonLayout[0].SemanticName = "POSITION";
	polygonLayout[0].SemanticIndex = 0;
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[0].InputSlot = 0;
	polygonLayout[0].AlignedByteOffset = 0;
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;

	polygonLayout[1].SemanticName = "TEXCOORD";
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	polygonLayout[1].InputSlot = 0;
	polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;

	polygonLayout[2].SemanticName = "NORMAL";
	polygonLayout[2].SemanticIndex = 0;
	polygonLayout[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[2].InputSlot = 0;
	polygonLayout[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[2].InstanceDataStepRate = 0;

	// Get a count of the elements in the layout.
	numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	// Create the vertex input layout.
	result = m_device->CreateInputLayout(polygonLayout, numElements, vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(),
		&m_layout);
	if (FAILED(result))
	{
		//return false;
	}

	// Release the vertex shader buffer and pixel shader buffer since they are no longer needed.
	vertexShaderBuffer->Release();
	vertexShaderBuffer = 0;
}

void BaseShader::loadPixelShader(WCHAR* filename)
{
	HRESULT result;
	ID3DBlob* errorMessage;
	ID3DBlob* pixelShaderBuffer;
	
	// Compile the pixel shader code.
	result = D3DCompileFromFile(filename, NULL, NULL, "main", "ps_5_0", D3DCOMPILE_ENABLE_STRICTNESS, 0, &pixelShaderBuffer, &errorMessage);
	if (FAILED(result))
	{
		// If the shader failed to compile it should have writen something to the error message.
		if (errorMessage)
		{
			OutputShaderErrorMessage(errorMessage, m_hwnd, filename);
		}
		// If there was  nothing in the error message then it simply could not find the file itself.
		else
		{
			MessageBox(m_hwnd, filename, L"Missing Shader File", MB_OK);
		}

		exit(0);
	}

	// Create the pixel shader from the buffer.
	result = m_device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &m_pixelShader);
	
	pixelShaderBuffer->Release();
	pixelShaderBuffer = 0;
}

void BaseShader::loadVertexShader(WCHAR * filename, const  char* functionEntyPoint)
{
	HRESULT result;
	ID3DBlob* errorMessage;
	ID3DBlob* vertexShaderBuffer;
	D3D11_INPUT_ELEMENT_DESC polygonLayout[3];
	unsigned int numElements;

	// Initialize the pointers this function will use to null.
	errorMessage = 0;
	vertexShaderBuffer = 0;

	// Compile the vertex shader code.
	result = D3DCompileFromFile(filename, NULL, NULL, functionEntyPoint, "vs_5_0", D3DCOMPILE_ENABLE_STRICTNESS, 0, &vertexShaderBuffer, &errorMessage);
	if (FAILED(result))
	{
		// If the shader failed to compile it should have writen something to the error message.
		if (errorMessage)
		{
			OutputShaderErrorMessage(errorMessage, m_hwnd, filename);
		}
		// If there was nothing in the error message then it simply could not find the shader file itself.
		else
		{
			MessageBox(m_hwnd, filename, L"Missing Shader File", MB_OK);
		}
		exit(0);
	}

	// Create the vertex shader from the buffer.
	result = m_device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &m_vertexShader);
	if (FAILED(result))
	{
		//return false;
	}

	// Create the vertex input layout description.
	// This setup needs to match the VertexType stucture in the MeshClass and in the shader.
	polygonLayout[0].SemanticName = "POSITION";
	polygonLayout[0].SemanticIndex = 0;
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[0].InputSlot = 0;
	polygonLayout[0].AlignedByteOffset = 0;
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;

	polygonLayout[1].SemanticName = "TEXCOORD";
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	polygonLayout[1].InputSlot = 0;
	polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;

	polygonLayout[2].SemanticName = "NORMAL";
	polygonLayout[2].SemanticIndex = 0;
	polygonLayout[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[2].InputSlot = 0;
	polygonLayout[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[2].InstanceDataStepRate = 0;

	// Get a count of the elements in the layout.
	numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	// Create the vertex input layout.
	result = m_device->CreateInputLayout(polygonLayout, numElements, vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(),
		&m_layout);
	if (FAILED(result))
	{
		//return false;
	}

	// Release the vertex shader buffer and pixel shader buffer since they are no longer needed.
	vertexShaderBuffer->Release();
	vertexShaderBuffer = 0;
}

void BaseShader::loadHullShader(WCHAR * filename, const  char* functionEntyPoint)
{
	HRESULT result;
	ID3D10Blob* errorMessage;
	ID3D10Blob* hullShaderBuffer;

	// Compile the hull shader code.
	result = D3DCompileFromFile(filename, NULL, NULL, functionEntyPoint, "hs_5_0", D3DCOMPILE_ENABLE_STRICTNESS, 0, &hullShaderBuffer, &errorMessage);
	if (FAILED(result))
	{
		// If the shader failed to compile it should have writen something to the error message.
		if (errorMessage)
		{
			OutputShaderErrorMessage(errorMessage, m_hwnd, filename);
		}
		// If there was nothing in the error message then it simply could not find the shader file itself.
		else
		{
			MessageBox(m_hwnd, filename, L"Missing Shader File", MB_OK);
		}

		exit(0);
	}

	// Create the hull shader from the buffer.
	result = m_device->CreateHullShader(hullShaderBuffer->GetBufferPointer(), hullShaderBuffer->GetBufferSize(), NULL, &m_hullShader);

	hullShaderBuffer->Release();
	hullShaderBuffer = 0;
}

void BaseShader::loadDomainShader(WCHAR * filename, const  char* functionEntyPoint)
{
	HRESULT result;
	ID3D10Blob* errorMessage;
	ID3D10Blob* domainShaderBuffer;

	// Compile the domain shader code.
	result = D3DCompileFromFile(filename, NULL, NULL, functionEntyPoint, "ds_5_0", D3DCOMPILE_ENABLE_STRICTNESS, 0, &domainShaderBuffer, &errorMessage);
	if (FAILED(result))
	{
		// If the shader failed to compile it should have writen something to the error message.
		if (errorMessage)
		{
			OutputShaderErrorMessage(errorMessage, m_hwnd, filename);
		}
		// If there was nothing in the error message then it simply could not find the shader file itself.
		else
		{
			MessageBox(m_hwnd, filename, L"Missing Shader File", MB_OK);
		}

		exit(0);
	}

	// Create the domain shader from the buffer.
	result = m_device->CreateDomainShader(domainShaderBuffer->GetBufferPointer(), domainShaderBuffer->GetBufferSize(), NULL, &m_domainShader);

	domainShaderBuffer->Release();
	domainShaderBuffer = 0;
}

void BaseShader::loadGeometryShader(WCHAR * filename, const  char* functionEntyPoint)
{
	HRESULT result;
	ID3D10Blob* errorMessage;
	ID3D10Blob* geometryShaderBuffer;

	// Compile the domain shader code.
	result = D3DCompileFromFile(filename, NULL, NULL, functionEntyPoint, "gs_5_0", D3DCOMPILE_ENABLE_STRICTNESS, 0, &geometryShaderBuffer, &errorMessage);
	if (FAILED(result))
	{
		// If the shader failed to compile it should have writen something to the error message.
		if (errorMessage)
		{
			OutputShaderErrorMessage(errorMessage, m_hwnd, filename);
		}
		// If there was nothing in the error message then it simply could not find the shader file itself.
		else
		{
			MessageBox(m_hwnd, filename, L"Missing Shader File", MB_OK);
		}

		exit(0);
	}

	// Create the domain shader from the buffer.
	m_device->CreateGeometryShader(geometryShaderBuffer->GetBufferPointer(), geometryShaderBuffer->GetBufferSize(), NULL, &m_geometryShader);

	geometryShaderBuffer->Release();
	geometryShaderBuffer = 0;
}

void BaseShader::loadPixelShader(WCHAR * filename, const  char* functionEntyPoint)
{
	HRESULT result;
	ID3DBlob* errorMessage;
	ID3DBlob* pixelShaderBuffer;

	// Compile the pixel shader code.
	result = D3DCompileFromFile(filename, NULL, NULL, functionEntyPoint, "ps_5_0", D3DCOMPILE_ENABLE_STRICTNESS, 0, &pixelShaderBuffer, &errorMessage);
	if (FAILED(result))
	{
		// If the shader failed to compile it should have writen something to the error message.
		if (errorMessage)
		{
			OutputShaderErrorMessage(errorMessage, m_hwnd, filename);
		}
		// If there was  nothing in the error message then it simply could not find the file itself.
		else
		{
			MessageBox(m_hwnd, filename, L"Missing Shader File", MB_OK);
		}

		exit(0);
	}

	// Create the pixel shader from the buffer.
	result = m_device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &m_pixelShader);

	pixelShaderBuffer->Release();
	pixelShaderBuffer = 0;
}

void BaseShader::loadHullShader(WCHAR* filename)
{
	HRESULT result;
	ID3D10Blob* errorMessage;
	ID3D10Blob* hullShaderBuffer;

	// Compile the hull shader code.
	result = D3DCompileFromFile(filename, NULL, NULL, "main", "hs_5_0", D3DCOMPILE_ENABLE_STRICTNESS, 0, &hullShaderBuffer, &errorMessage);
	if (FAILED(result))
	{
		// If the shader failed to compile it should have writen something to the error message.
		if (errorMessage)
		{
			OutputShaderErrorMessage(errorMessage, m_hwnd, filename);
		}
		// If there was nothing in the error message then it simply could not find the shader file itself.
		else
		{
			MessageBox(m_hwnd, filename, L"Missing Shader File", MB_OK);
		}

		exit(0);
	}

	// Create the hull shader from the buffer.
	result = m_device->CreateHullShader(hullShaderBuffer->GetBufferPointer(), hullShaderBuffer->GetBufferSize(), NULL, &m_hullShader);
	
	hullShaderBuffer->Release();
	hullShaderBuffer = 0;
}

void BaseShader::loadDomainShader(WCHAR* filename)
{
	HRESULT result;
	ID3D10Blob* errorMessage;
	ID3D10Blob* domainShaderBuffer;

	// Compile the domain shader code.
	result = D3DCompileFromFile(filename, NULL, NULL, "main", "ds_5_0", D3DCOMPILE_ENABLE_STRICTNESS, 0, &domainShaderBuffer, &errorMessage);
	if (FAILED(result))
	{
		// If the shader failed to compile it should have writen something to the error message.
		if (errorMessage)
		{
			OutputShaderErrorMessage(errorMessage, m_hwnd, filename);
		}
		// If there was nothing in the error message then it simply could not find the shader file itself.
		else
		{
			MessageBox(m_hwnd, filename, L"Missing Shader File", MB_OK);
		}

		exit(0);
	}

	// Create the domain shader from the buffer.
	result = m_device->CreateDomainShader(domainShaderBuffer->GetBufferPointer(), domainShaderBuffer->GetBufferSize(), NULL, &m_domainShader);
	
	domainShaderBuffer->Release();
	domainShaderBuffer = 0;
}

void BaseShader::loadGeometryShader(WCHAR* filename)
{
	HRESULT result;
	ID3D10Blob* errorMessage;
	ID3D10Blob* geometryShaderBuffer;

	// Compile the domain shader code.
	result = D3DCompileFromFile(filename, NULL, NULL, "main", "gs_5_0", D3DCOMPILE_ENABLE_STRICTNESS, 0, &geometryShaderBuffer, &errorMessage);
	if (FAILED(result))
	{
		// If the shader failed to compile it should have writen something to the error message.
		if (errorMessage)
		{
			OutputShaderErrorMessage(errorMessage, m_hwnd, filename);
		}
		// If there was nothing in the error message then it simply could not find the shader file itself.
		else
		{
			MessageBox(m_hwnd, filename, L"Missing Shader File", MB_OK);
		}

		exit(0);
	}

	// Create the domain shader from the buffer.
	m_device->CreateGeometryShader(geometryShaderBuffer->GetBufferPointer(), geometryShaderBuffer->GetBufferSize(), NULL, &m_geometryShader);

	geometryShaderBuffer->Release();
	geometryShaderBuffer = 0;
}

void BaseShader::OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, WCHAR* shaderFilename)
{
	char* compileErrors;
	unsigned long bufferSize, i;
	ofstream fout;


	// Get a pointer to the error message text buffer.
	compileErrors = (char*)(errorMessage->GetBufferPointer());

	// Get the length of the message.
	bufferSize = errorMessage->GetBufferSize();

	// Open a file to write the error message to.
	fout.open("shader-error.txt");

	// Write out the error message.
	for (i = 0; i<bufferSize; i++)
	{
		fout << compileErrors[i];
	}

	// Close the file.
	fout.close();

	// Release the error message.
	errorMessage->Release();
	errorMessage = 0;

	// Pop a message up on the screen to notify the user to check the text file for compile errors.
	MessageBox(hwnd, L"Error compiling shader.  Check shader-error.txt for message.", shaderFilename, MB_OK);

	return;
}


void BaseShader::Render(ID3D11DeviceContext* deviceContext, int indexCount)
{
	// Set the vertex input layout.
	deviceContext->IASetInputLayout(m_layout);

	// Set the vertex and pixel shaders that will be used to render.
	deviceContext->VSSetShader(m_vertexShader, NULL, 0);
	deviceContext->PSSetShader(m_pixelShader, NULL, 0);
	
	// if Hull shader is not null then set HS and DS
	if (m_hullShader)
	{
		deviceContext->HSSetShader(m_hullShader, NULL, 0);
		deviceContext->DSSetShader(m_domainShader, NULL, 0);
	}
	else
	{
		deviceContext->HSSetShader(NULL, NULL, 0);
		deviceContext->DSSetShader(NULL, NULL, 0);
	}

	// if geometry shader is not null then set GS
	if (m_geometryShader)
	{
		deviceContext->GSSetShader(m_geometryShader, NULL, 0);
	}
	else
	{
		deviceContext->GSSetShader(NULL, NULL, 0);
	}

	// Render the triangle.
	deviceContext->DrawIndexed(indexCount, 0, 0);
}
