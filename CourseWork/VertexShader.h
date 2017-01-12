// Light shader.h
// Basic single light shader setup
#ifndef _VERTEXSHADER_H_
#define _VERTEXSHADER_H_

#include "../DXFramework/BaseShader.h"
#include "../DXFramework/Light.h"

using namespace std;
using namespace DirectX;
const int NUM_LIGHTS = 4;

class VertexShader : public BaseShader
{
private:
	struct LightBufferType
	{
		XMFLOAT4 diffuse[NUM_LIGHTS];
		XMFLOAT4 direction[NUM_LIGHTS];
		XMFLOAT4 ambient[NUM_LIGHTS];
		XMFLOAT4 specularColour[NUM_LIGHTS];
		XMFLOAT4 position[NUM_LIGHTS];
 
		XMFLOAT4 specularPower[NUM_LIGHTS];

	XMFLOAT4 constantAttenuationFactor[NUM_LIGHTS];
		XMFLOAT4 linearAttenuationFactor[NUM_LIGHTS];
		XMFLOAT4 quadraticAttenuationFactor[NUM_LIGHTS];
		XMFLOAT4 range[NUM_LIGHTS];

		// Using ints indead of bools for spacing errors
		// 0 means false 
		// 1 means true
		XMINT4 isSpecular[NUM_LIGHTS];
		XMINT4 isDirectionalLight[NUM_LIGHTS];
		XMINT4 isSpotLight[NUM_LIGHTS];
		XMINT4 isPointLight[NUM_LIGHTS];


	};
	struct CammeraBufferType  
	{
		XMFLOAT3 cammeraPostion;
		float padding;
	};
	struct TimeBufferType
	{
		float delatTime;
		float height;
		float frequancy;		
		int isSinWave;

		int isCosWave;
		int isThirdWave;
		int isForthWave;
		int isFithWave;

	};

	
public:

	enum typeOfVertexMinimulation
	{
		sin,
		cos,
		third,
		forth,
		fith
	};

	VertexShader(ID3D11Device* device, HWND hwnd);
	~VertexShader();

	void SetShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX &worldMatrix, const XMMATRIX &viewMatrix, const XMMATRIX &projectionMatrix, ID3D11ShaderResourceView* texture, Light* light[], XMFLOAT3 cammeraPostion, float delatTime, float vertexFequancy, float vertexHeight, typeOfVertexMinimulation typeOfMinimulation);
	void Render(ID3D11DeviceContext* deviceContext, int vertexCount);

private:
	void InitShader(WCHAR*, WCHAR*);

private:
	ID3D11Buffer* m_matrixBuffer;
	ID3D11SamplerState* m_sampleState;
	ID3D11Buffer* m_lightBuffer;
	ID3D11Buffer* m_camBuffer;
	ID3D11Buffer* m_timeBuffer;
};

#endif