// DoubleTriangle.h
#ifndef _DOUBLETRIANGLEMESH_H_
#define _DOUBLETRIANGLEMESH_H_

#include "TriangleMesh.h"

using namespace DirectX;

class DoubleTriangleMesh : public BaseMesh
{
public:
	DoubleTriangleMesh(ID3D11Device* device, ID3D11DeviceContext* deviceContext, WCHAR* textureFilename);

 	~DoubleTriangleMesh();

protected:
	void InitBuffers(ID3D11Device* device);

};

#endif