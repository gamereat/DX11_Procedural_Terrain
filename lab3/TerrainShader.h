#pragma once
#include "..\DXFramework\BaseShader.h"
#include "Scene.h"
#include "ShaderBuffers.h"
using namespace std;
using namespace DirectX;

class TerrainShader :	public BaseShader
{
public:
 
	 
	TerrainShader(ID3D11Device* device, HWND hwnd );
	~TerrainShader();

	void SetShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX &worldMatrix, 
		const XMMATRIX &viewMatrix, const XMMATRIX &projectionMatrix, 
		ID3D11ShaderResourceView* texture,  FaultLineDisplacementBufferType* faultLineSettings, TerrainSettingTextureType* terrainTextureSettings,
		Light* light[NUM_LIGHTS],
		ID3D11ShaderResourceView*depthMap[]);
	void Render(ID3D11DeviceContext * deviceContext, int indexCount);

 
private:
	void InitShader(WCHAR* vsFilename, WCHAR* psFilename);
	void InitShader(WCHAR* vsFilename, WCHAR* hsFilename, WCHAR* dsFilename, WCHAR* psFilename);



private:
	ID3D11Buffer* m_matrixBuffer;
	ID3D11SamplerState* m_sampleState;
	ID3D11SamplerState* m_sampleStateClamp;
	ID3D11Buffer* m_LightBuffer;
	ID3D11Buffer* m_LightBuffer2;


	ID3D11Buffer* terrainTexturingBuffer;;

	ID3D11Buffer* faultLineBuffer;


};

