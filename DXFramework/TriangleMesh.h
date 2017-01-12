// TriangleMesh.h
// Simple single triangle mesh (for example purposes).

#ifndef _TRIMESH_H_
#define _TRIMESH_H_

#include "BaseMesh.h"

using namespace DirectX;

class TriangleMesh : public BaseMesh
{

public:
	TriangleMesh(ID3D11Device* device, ID3D11DeviceContext* deviceContext, WCHAR* textureFilename);
	TriangleMesh();
	~TriangleMesh();

protected:
	void InitBuffers(ID3D11Device* device);
	
};

#endif