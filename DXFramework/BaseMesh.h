// Mesh.h
#ifndef _BASEMESH_H_
#define _BASEMESH_H_

#include <d3d11.h>
#include <directxmath.h>
#include "texture.h"

using namespace DirectX;

class BaseMesh
{
protected:

	struct VertexType
	{
		XMFLOAT3 position;
		XMFLOAT2 texture;
		XMFLOAT3 normal;
	};

public:
	BaseMesh();
	~BaseMesh();

	virtual void SendData(ID3D11DeviceContext*);
	int GetIndexCount();
	ID3D11ShaderResourceView* GetTexture();

 
protected:
	virtual void InitBuffers(ID3D11Device*) = 0;
	void LoadTexture(ID3D11Device*, ID3D11DeviceContext* deviceContext,  WCHAR*);

	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	int m_vertexCount, m_indexCount;
	Texture* m_Texture;

 
};

#endif