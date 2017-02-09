// Colour shader.h
// Simple shader example.
#ifndef _DISTANCEBASETESSELATION_H_
#define _DISTANCEBASETESSELATION_H_

#include "../DXFramework/BaseShader.h"
#include "ShaderBuffers.h"
using namespace std;
using namespace DirectX;


class DistanceBasedTesselation : public BaseShader
{

public:

	DistanceBasedTesselation(ID3D11Device* device, HWND hwnd);
	~DistanceBasedTesselation();

	void SetShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX &world, const XMMATRIX &view, const XMMATRIX &projection);
	void SetShaderParameters(ID3D11DeviceContext * deviceContext,
		const XMMATRIX & worldMatrix, const XMMATRIX & viewMatrix,
		const XMMATRIX & projectionMatrix, ID3D11ShaderResourceView * texture,
	
		TessellationBufferType tesselationInfo,

		Light* light[]);
	void Render(ID3D11DeviceContext* deviceContext, int vertexCount);

private:
	void InitShader(WCHAR*, WCHAR*);
	void InitShader(WCHAR* vsFilename, WCHAR* hsFilename, WCHAR* dsFilename, WCHAR* psFilename);


private:
 
	ID3D11Buffer* m_matrixBuffer;
	ID3D11SamplerState* m_sampleState;
	ID3D11SamplerState* sampleStateClamp;
	ID3D11Buffer* m_tessellationBuffer;
	ID3D11Buffer* lightBuffer;
	ID3D11Buffer* lightBuffer2;
};

#endif