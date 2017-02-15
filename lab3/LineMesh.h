#ifndef _LINEMESH_H_
#define _LINEMESH_H_

#include "../DXFramework/BaseMesh.h"

using namespace DirectX;

class LineMesh : public BaseMesh
{

public:
	LineMesh(ID3D11Device* device, ID3D11DeviceContext* deviceContext, WCHAR* textureFilename, int resolution = 20);
	~LineMesh();

protected:
	void InitBuffers(ID3D11Device* device);
	int m_resolution;
};

#endif