// Light shader.h
// Basic single light shader setup
#ifndef _BOXBLURSHADER_H_
#define _BOXBLURSHADER_H_

#include "../DXFramework/BaseShader.h"
#include "../DXFramework/Light.h"

using namespace std;
using namespace DirectX;


class BoxBlurShader : public BaseShader
{
	struct ScreenSizeBufferType
	{
		float screenWidth;
		float screenHeight;
		XMFLOAT2 padding;
	};


public:


	BoxBlurShader(ID3D11Device* device, HWND hwnd);
	~BoxBlurShader();

	void SetShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX &worldMatrix, const XMMATRIX &viewMatrix, const XMMATRIX &projectionMatrix, ID3D11ShaderResourceView* texture);
	void Render(ID3D11DeviceContext* deviceContext, int vertexCount);

private:
	void InitShader(WCHAR*, WCHAR*);

private:

	ID3D11Buffer* m_matrixBuffer;
	ID3D11SamplerState* m_sampleState;

	ID3D11Buffer* m_screenSizeBuffer;

};

#endif