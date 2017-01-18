// texture shader.h
#ifndef _TEXTURESHADER_H_
#define _TEXTURESHADER_H_

#include "../DXFramework/BaseShader.h"

using namespace std;
using namespace DirectX;


class TextureShader : public BaseShader
{

public:

	TextureShader(ID3D11Device* device, HWND hwnd);
	~TextureShader();

	void SetShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX &world, const XMMATRIX &view, const XMMATRIX &projection, ID3D11ShaderResourceView* texture);
	void Render(ID3D11DeviceContext* deviceContext, int vertexCount);

private:
	void InitShader(WCHAR*, WCHAR*);

private:
	ID3D11Buffer* m_matrixBuffer;
	ID3D11SamplerState* m_sampleState;
};

#endif