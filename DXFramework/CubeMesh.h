#ifndef _CUBEMESH_H_
#define _CUBEMESH_H_

#include "BaseMesh.h"

using namespace DirectX;

class CubeMesh : public BaseMesh
{

public:
	CubeMesh(ID3D11Device* device, ID3D11DeviceContext* deviceContext, WCHAR* textureFilename, int resolution = 20);
	~CubeMesh();
	void SendData(ID3D11DeviceContext * deviceContext);

protected:
	void InitBuffers(ID3D11Device* device);
	int m_resolution;
};

#endif