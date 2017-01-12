#ifndef _MODEL_H_
#define _MODEL_H_

#include "BaseMesh.h"
#include "TokenStream.h"
#include <vector>

using namespace DirectX;

class Model : public BaseMesh
{
	struct ModelType
	{
		float x, y, z;
		float tu, tv;
		float nx, ny, nz;
	};

public:
	Model(ID3D11Device* device, ID3D11DeviceContext* deviceContext, WCHAR* textureFilename, WCHAR* filename);
	~Model();


protected:
	void InitBuffers(ID3D11Device* device);
	void LoadModel(WCHAR* filename);

	
	ModelType* m_model;
};

#endif