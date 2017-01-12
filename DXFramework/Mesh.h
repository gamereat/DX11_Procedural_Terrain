// Mesh.h
#ifndef _MESH_H_
#define _MESH_H_

#include <d3d11.h>
#include <directxmath.h>
#include "texture.h"

using namespace DirectX;

class Mesh
{
protected:

	struct VertexType
	{
		XMFLOAT3 position;
		XMFLOAT2 texture;
		XMFLOAT3 normal;
	};

public:
	Mesh(ID3D11Device* device, ID3D11DeviceContext* deviceContext, WCHAR* textureFilename);
	~Mesh();

	void SendData(ID3D11DeviceContext*);
	int GetIndexCount();
	ID3D11ShaderResourceView* GetTexture();

protected:
	bool InitBuffers(ID3D11Device*);
	void LoadTexture(ID3D11Device*, ID3D11DeviceContext* deviceContext, WCHAR*);
	
protected:
	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	int m_vertexCount, m_indexCount;
	Texture* m_Texture;
};

#endif