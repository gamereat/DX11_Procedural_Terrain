#pragma once
#include "../DXFramework\BaseMesh.h"
class TesselatedQuadMess :
	public BaseMesh
{
public:
 
	TesselatedQuadMess(ID3D11Device* device, ID3D11DeviceContext* deviceContext, WCHAR* textureFilename, int resolution = 100);
	~TesselatedQuadMess();
	void SendData(ID3D11DeviceContext * deviceContext);


protected:
	void InitBuffers(ID3D11Device* device);
	int resolution;
};

