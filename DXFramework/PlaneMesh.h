#ifndef _PLANEMESH_H_
#define _PLANEMESH_H_

#include "BaseMesh.h"

class PlaneMesh : public BaseMesh
{

public:
	PlaneMesh(ID3D11Device* device, ID3D11DeviceContext* deviceContext, WCHAR* textureFilename, int resolution = 100);
	~PlaneMesh();

protected:
	void InitBuffers(ID3D11Device* device);
	int m_resolution;
};

#endif