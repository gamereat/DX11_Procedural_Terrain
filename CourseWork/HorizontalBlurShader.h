// Horizontal blur shader handler
// Loads horizontal blur shaders (vs and ps)
// Passes screen width to shaders, for sample coordinate calculation

#ifndef _HORIZONTALBLURSHADER_H_
#define _HORIZONTALBLURSHADER_H_

//#include "../DXFramework/DXF.h"
#include "../DXFramework/BaseShader.h"


using namespace std;
using namespace DirectX;


class HorizontalBlurShader : public BaseShader
{
private:
	struct ScreenSizeBufferType
	{
		float screenWidth;
		int neightRange;
 		XMFLOAT2 padding;
	};

public:

	HorizontalBlurShader(ID3D11Device* device, HWND hwnd);
	~HorizontalBlurShader();

	void SetShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX &world, const XMMATRIX &view, const XMMATRIX &projection, ID3D11ShaderResourceView* texture, float width, int neigboursToCheck);
	void Render(ID3D11DeviceContext* deviceContext, int vertexCount);

private:
	void InitShader(WCHAR*, WCHAR*);

private:
	ID3D11Buffer* m_matrixBuffer;
	ID3D11SamplerState* m_sampleState;
	ID3D11Buffer* m_ScreenSizeBuffer;
};

#endif