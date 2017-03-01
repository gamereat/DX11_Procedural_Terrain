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
	BaseMesh(std::string name);
	~BaseMesh();

	//virtual void SendData(ID3D11DeviceContext*,XMMATRIX& worldMartix);
	virtual XMMATRIX SendData(ID3D11DeviceContext*);
	virtual int GetIndexCount();
	ID3D11ShaderResourceView* GetTexture();


	void GuiSettings(bool* is_open);

	void setScale(XMFLOAT3 scale);
	void setRotation(XMFLOAT3 rotation);
	void setTranslation(XMFLOAT3 translation);
 
	void ToggleMenu();

	void setScaleRotationTranslation(XMFLOAT3 scale, XMFLOAT3 rotation, XMFLOAT3 translation);

	XMFLOAT3 getRotation();
	XMFLOAT3 getScale();
	XMFLOAT3 getTranslation();

	std::string getName();
	bool menuOpen;

 protected:


	virtual void InitBuffers(ID3D11Device*) = 0;
	void LoadTexture(ID3D11Device*, ID3D11DeviceContext* deviceContext,  WCHAR*);

	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	int m_vertexCount, m_indexCount;
	Texture* m_Texture;


	/*
	Name of object
	*/
	std::string name;
	/*
	Scaling used by the mesh
	*/
	XMFLOAT3 scale;
	/*
	Rotation used by the mesh
	*/
	XMFLOAT3 rotate;
	/*
	Translation used by the mesh
	*/
	XMFLOAT3 translation;
};

#endif