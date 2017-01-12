#pragma once


#include "../DXFramework/BaseShader.h"

using namespace std;
using namespace DirectX;

class GeomentryShader : public BaseShader
{
 
public:

	struct GeomentryBufferType
	{
		
		float time;
		float gravity;
		float explosiveAmmount;
		float explosiveAcceleration;


		float maxTime;
		//@DEPECATED
		//just used as padding now 
		XMFLOAT3 explotionMaximums;
	};
		struct MatrixBufferType
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection; 
	};
	GeomentryShader(ID3D11Device* device, HWND hwnd);
	~GeomentryShader();

	void SetShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX &worldMatrix, const XMMATRIX &viewMatrix, const XMMATRIX &projectionMatrix, ID3D11ShaderResourceView* texture, GeomentryBufferType explosiveSettings);
	void Render(ID3D11DeviceContext * deviceContext, int indexCount);


private:
	void InitShader(WCHAR* vsFilename, WCHAR* psFilename);
	void InitShader(WCHAR* vsFilename, WCHAR* psFilename, WCHAR* gsFilename);

private:
	ID3D11Buffer* m_matrixBuffer;
	ID3D11Buffer* m_GeomentryBuffer;

	ID3D11SamplerState* m_sampleState;

};


