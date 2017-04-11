#pragma once
 
#include "../DXFramework/BaseShader.h"


using namespace std;
using namespace DirectX;

class ChromaticAberrationsShader :
	public BaseShader
{
public:
	struct ChromaticAberrationsBufferType
	{
		XMFLOAT3 rgbDistortLevel;
		float totalTime;

		XMFLOAT3 freqancy;
		float padding1;


	};

	ChromaticAberrationsShader(ID3D11Device* device, HWND hwnd);
	~ChromaticAberrationsShader();


	void SetShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX &world, const XMMATRIX &view, const XMMATRIX &projection, ID3D11ShaderResourceView* texture, ChromaticAberrationsBufferType chromticSettings);
	void Render(ID3D11DeviceContext* deviceContext, int vertexCount);

private:
	void InitShader(WCHAR*, WCHAR*);

	ID3D11Buffer* m_matrixBuffer;
	ID3D11Buffer* m_chromaticBuffer;
	ID3D11SamplerState* m_sampleState;
};

