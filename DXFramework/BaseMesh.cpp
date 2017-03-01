#include "BaseMesh.h"
#include "BaseMesh.h"
#include "../imgui/imgui.h"
// BaseMesh.cpp
// Base mesh class, for inheriting base mesh functionality.

 


BaseMesh::BaseMesh(std::string name)
{
	this->name = name;

	this->scale = XMFLOAT3(1,1,1);
	this->translation = XMFLOAT3(0, 0, 0);;
	this->rotate = XMFLOAT3(0, 0, 0);;
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

void BaseMesh::GuiSettings(bool * is_open)
{
	if (*is_open)
	{
		if (!ImGui::Begin(name.c_str(), is_open, ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::End();
			return;
		}

		ImGui::Text("Set the rotation scale and translation of the given object");

		ImGui::Separator();

		ImGui::InputFloat3("Translation", &translation.x);
		ImGui::InputFloat3("Rotation", &rotate.x);
		ImGui::InputFloat3("Scale", &scale.x);
	
		ImGui::End();
	}
}

void BaseMesh::setScale(XMFLOAT3 scale)
{
	this->scale = scale;
}

void BaseMesh::setRotation(XMFLOAT3 rotation)
{	
	this->rotate = rotation;
}

void BaseMesh::setTranslation(XMFLOAT3 translation)
{
	this->translation = translation;
}

void BaseMesh::ToggleMenu()
{
	menuOpen = menuOpen ? false : true;
}

void BaseMesh::setScaleRotationTranslation(XMFLOAT3 scale, XMFLOAT3 rotation, XMFLOAT3 translation)
{
	this->scale = scale;
	this->translation = translation;
	this->rotate = rotation;
}

XMFLOAT3 BaseMesh::getRotation()
{
	return rotate;
}

XMFLOAT3 BaseMesh::getScale()
{
	return scale;
}

XMFLOAT3 BaseMesh::getTranslation()
{
	return translation;
}

std::string BaseMesh::getName()
{
	return name;
}

 
 

//void BaseMesh::SendData(ID3D11DeviceContext* deviceContext, & worldMartix)
XMMATRIX BaseMesh::SendData(ID3D11DeviceContext* deviceContext)
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


	// Set the position on screen
	 return  XMMatrixTranslation(translation.x, translation.y, translation.z) *
		XMMatrixScaling(scale.x, scale.y, scale.z) * XMMatrixRotationX(rotate.x) *
		XMMatrixRotationY(rotate.y) *  XMMatrixRotationZ(rotate.z);
}

void BaseMesh::LoadTexture(ID3D11Device* device, ID3D11DeviceContext* deviceContext, WCHAR* filename)
{
	// Create the texture object.
	m_Texture = new Texture(device, deviceContext, filename);
}



