// Tessellation Mesh, point list for tessellation
#ifndef _TESSELLATIONMESH_H_
#define _TESSELLATIONMESH_H_

#include "BaseMesh.h"

using namespace DirectX;

class TessellationMesh : public BaseMesh
{

public:
	TessellationMesh(ID3D11Device* device, ID3D11DeviceContext* deviceContext, WCHAR* textureFilename);
	~TessellationMesh();

	void SendData(ID3D11DeviceContext*);

protected:
	void InitBuffers(ID3D11Device* device);
	
};

#endif