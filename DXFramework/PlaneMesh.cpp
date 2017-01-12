// plane mesh
// Quad mesh made of many quads. Default is 100x100
#include "planemesh.h"

PlaneMesh::PlaneMesh(ID3D11Device* device, ID3D11DeviceContext* deviceContext, WCHAR* textureFilename, int resolution)
{
	m_resolution = resolution;
	// Initialize the vertex and index buffer that hold the geometry for the triangle.
	InitBuffers(device);

	// Load the texture for this model.
	LoadTexture(device, deviceContext, textureFilename);
}

PlaneMesh::~PlaneMesh()
{
	// Run parent deconstructor
	BaseMesh::~BaseMesh();
}


void PlaneMesh::InitBuffers(ID3D11Device* device)
{
	VertexType* vertices;
	unsigned long* indices;
	int index, i, j;
	float positionX, positionZ, u, v, increment;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	
	// Calculate the number of vertices in the terrain mesh.
	m_vertexCount = (m_resolution - 1) * (m_resolution - 1) * 8;

	// Set the index count to the same as the vertex count.
	m_indexCount = m_vertexCount;

	// Create the vertex array.
	vertices = new VertexType[m_vertexCount];

	// Create the index array.
	indices = new unsigned long[m_indexCount];
	
	// Initialize the index to the vertex array.
	index = 0;

	// UV coords.
	u = 0;
	v = 0;
	increment = 1.0f / m_resolution;
	

	// Load the vertex and index arrays with the terrain data.
	for (j = 0; j<(m_resolution - 1); j++)
	{
		for (i = 0; i<(m_resolution - 1); i++)
		{
			// Upper left.
			positionX = (float)i;
			positionZ = (float)(j);

			vertices[index].position = XMFLOAT3(positionX, 0.0f, positionZ);
			vertices[index].texture = XMFLOAT2(u, v);
			vertices[index].normal = XMFLOAT3(0.0, 1.0, 0.0);
			indices[index] = index;
			index++;

			// Upper right.
			positionX = (float)(i + 1);
			positionZ = (float)(j + 1);

			vertices[index].position = XMFLOAT3(positionX, 0.0f, positionZ);
			vertices[index].texture = XMFLOAT2(u + increment, v + increment);
			vertices[index].normal = XMFLOAT3(0.0, 1.0, 0.0);
			indices[index] = index;
			index++;


			// lower left
			positionX = (float)(i);
			positionZ = (float)(j + 1);
			

			vertices[index].position = XMFLOAT3(positionX, 0.0f, positionZ);
			vertices[index].texture = XMFLOAT2(u, v + increment);
			vertices[index].normal = XMFLOAT3(0.0, 1.0, 0.0);
			indices[index] = index;
			index++;

			// Upper left
			positionX = (float)(i);
			positionZ = (float)(j);

			vertices[index].position = XMFLOAT3(positionX, 0.0f, positionZ);
			vertices[index].texture = XMFLOAT2(u, v);
			vertices[index].normal = XMFLOAT3(0.0, 1.0, 0.0);
			indices[index] = index;
			index++;

			// Bottom right
			positionX = (float)(i + 1);
			positionZ = (float)(j);

			vertices[index].position = XMFLOAT3(positionX, 0.0f, positionZ);
			vertices[index].texture = XMFLOAT2(u + increment, v);
			vertices[index].normal = XMFLOAT3(0.0, 1.0, 0.0);
			indices[index] = index;
			index++;

			// Upper right.
			positionX = (float)(i + 1);
			positionZ = (float)(j + 1);

			vertices[index].position = XMFLOAT3(positionX, 0.0f, positionZ);
			vertices[index].texture = XMFLOAT2(u + increment, v + increment);
			vertices[index].normal = XMFLOAT3(0.0, 1.0, 0.0);
			indices[index] = index;
			index++;

			u += increment;
	
		}

		u = 0;
		v += increment;
	}

	// Set up the description of the static vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType)* m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Now create the vertex buffer.
	device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	
	// Set up the description of the static index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long)* m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	
	// Release the arrays now that the buffers have been created and loaded.
	delete[] vertices;
	vertices = 0;

	delete[] indices;
	indices = 0;
}


