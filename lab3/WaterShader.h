#pragma once
#include "..\DXFramework\BaseShader.h"
#include "../DXFramework/Light.h"
#include "ShaderBuffers.h"
class WaterShader :public BaseShader
{

	

public:


 
	WaterShader(ID3D11Device* device, HWND hwnd);
	~WaterShader();

 	void SetShaderParameters(ID3D11DeviceContext * deviceContext,
		const XMMATRIX & worldMatrix, const XMMATRIX & viewMatrix,
		const XMMATRIX & projectionMatrix, ID3D11ShaderResourceView * texture,WavetBufferType* waveInfo);

	void Render(ID3D11DeviceContext* deviceContext, int vertexCount);

private:
	void InitShader(WCHAR* vsFilename, WCHAR* psFilename);
	void InitShader(WCHAR* vsFilename, WCHAR* hsFilename, WCHAR* dsFilename, WCHAR* psFilename);

private:
	ID3D11Buffer* m_matrixBuffer;
	ID3D11SamplerState* m_sampleState;
	ID3D11SamplerState* sampleStateClamp;
	ID3D11Buffer* planetBuffer;;


};

