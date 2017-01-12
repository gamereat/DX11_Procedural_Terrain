// BaseMesh.cpp
// Base mesh class, for inheriting base mesh functionality.

#include "basemesh.h"

BaseMesh::BaseMesh()
{
}


BaseMesh::~BaseMesh()
{
	// Release the index buffer.
	if (m_indexBuffer)
	{
		m_indexBuffer->Release();
		m_indexBuffer = 0;
	}

	// Release the vertex buffer.
	if (m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = 0;
	}

	// Release the texture object.
	if (m_Texture)
	{
		delete m_Texture;
		m_Texture = 0;
	}
}

int BaseMesh::GetIndexCount()
{
	return m_indexCount;
}

ID3D11ShaderResourceView* BaseMesh::GetTexture()
{
	return m_Texture->GetTexture();
}


void BaseMesh::SendData(ID3D11DeviceContext* deviceContext)
{
	unsigned int stride;
	unsigned int offset;
	
	// Set vertex buffer stride and offset.
	stride = sizeof(VertexType);
	offset = 0;

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	// Set the index buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void BaseMesh::LoadTexture(ID3D11Device* device, ID3D11DeviceContext* deviceContext, WCHAR* filename)
{
	// Create the texture object.
	m_Texture = new Texture(device, deviceContext, filename);
}



