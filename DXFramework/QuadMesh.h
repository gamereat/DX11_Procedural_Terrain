// Quad Mesh
// Simple square mesh
#ifndef _QUADMESH_H_
#define _QUADMESH_H_

#include "BaseMesh.h"

using namespace DirectX;

class QuadMesh : public BaseMesh
{

public:
	QuadMesh(ID3D11Device* device, ID3D11DeviceContext* deviceContext, WCHAR* textureFilename);
	~QuadMesh();
	void SendData(ID3D11DeviceContext * deviceContext);

protected:
	void InitBuffers(ID3D11Device* device);
	
};

#endif