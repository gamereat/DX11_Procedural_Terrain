// Tessellation Mesh, point list for tessellation
#ifndef _POINTMESH_H_
#define _POINTMESH_H_

#include "BaseMesh.h"

using namespace DirectX;

class PointMesh : public BaseMesh
{

public:
 	PointMesh(std::string name, ID3D11Device * device, ID3D11DeviceContext * deviceContext, WCHAR * textureFilename);
	~PointMesh();

	void SendData(ID3D11DeviceContext*, XMMATRIX& worldMartix);

protected:
	void InitBuffers(ID3D11Device* device);

};

#endif