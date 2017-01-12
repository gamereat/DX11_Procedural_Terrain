#ifndef _SHADOWSHADER_H_
#define _SHADOWSHADER_H_

#include "../DXFramework/BaseShader.h"
#include "../DXFramework/Light.h"
#include "VertexShader.h"
using namespace std;
using namespace DirectX;

class ShadowShader : public BaseShader
{
private:

	struct MatrixBufferType
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
		XMMATRIX lightView[NUM_LIGHTS];
		XMMATRIX lightProjection[NUM_LIGHTS];
	};
	struct CammeraBufferType
	{
		XMFLOAT3 cammeraPostion;
		float padding;
	};
	struct LightBufferType
	{
		//XMFLOAT4 ambient;
		//XMFLOAT4 diffuse; 

		XMFLOAT4 diffuseColour[NUM_LIGHTS];
		XMFLOAT4 lightDirection[NUM_LIGHTS];
		XMFLOAT4 ambientColour[NUM_LIGHTS];
		XMFLOAT4 specularColour[NUM_LIGHTS];
		XMFLOAT4 position[NUM_LIGHTS];

		XMFLOAT4 specularPower[NUM_LIGHTS];

		// 0 is constant
		// 1 linear
		// 2 quadratic
		// 3 range
		XMFLOAT4 attenuationValues[NUM_LIGHTS];

		//0 - directional
		//1- spot
		//2- point
		XMINT4 lightType[NUM_LIGHTS];

		int isSpecular[NUM_LIGHTS];

		int willGenerateShadows[NUM_LIGHTS];

	};

	struct LightBufferType2
	{
		XMFLOAT4 position[NUM_LIGHTS];
		//float padding;
	};

public:

	ShadowShader(ID3D11Device* device, HWND hwnd);
	~ShadowShader();

	void SetShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX &world, const XMMATRIX &view,
		const XMMATRIX &projection, ID3D11ShaderResourceView* texture, ID3D11ShaderResourceView*depthMap[], Light* light[]);
	void Render(ID3D11DeviceContext* deviceContext, int vertexCount);

private:
	void InitShader(WCHAR*, WCHAR*);

private:
	ID3D11Buffer* m_matrixBuffer;
	ID3D11SamplerState* m_sampleState;
	ID3D11SamplerState* m_sampleStateClamp;
	ID3D11Buffer* m_LightBuffer;
	ID3D11Buffer* m_LightBuffer2;

};

#endif