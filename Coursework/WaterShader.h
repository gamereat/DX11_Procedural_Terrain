#pragma once
#include "..\DXFramework\BaseShader.h"
#include "../DXFramework/Light.h"
#include "ShaderBuffers.h"

/*
* Water Shdaer 
*
* @author      Alan Yeats 
*
*/
class WaterShader : public BaseShader
{

public:

	/*	
	* @param device DirectX11 Device used for window
	* @param hwnd	Handle to window	
	*/
	WaterShader(ID3D11Device* device, HWND hwnd);
	~WaterShader();

	/*
	@param deviceContext	Device context for directx window
	@param worldMatrix		World Matrix for current scene
	@param viewMatrix		View Matrix for current Scene
	@param projectionMatrix projection Matrix for current Scene
	@param texture			Texture for the water
	@param waveInfo			Wave data to be sent to shader
	
	*/
 	void SetShaderParameters(ID3D11DeviceContext * deviceContext,
		const XMMATRIX & worldMatrix, const XMMATRIX & viewMatrix,
		const XMMATRIX & projectionMatrix, ID3D11ShaderResourceView * texture ,WavetBufferType* waveInfo);
 
	/*
	@param deviceContext	Device context for directx window
	@param vertexCount		

	*/
	void Render(ID3D11DeviceContext* deviceContext, int vertexCount);

private:
	/*
	@param vsFilename	file location of vertex shader
	@param psFilename	file location of pixel shader

	*/
	void InitShader(WCHAR* vsFilename, WCHAR* psFilename);

	/*
	@param vsFilename	file location of vertex shader
	@param hsFilename	file location of hull shader
	@param dsFilename	file location of domain shader
	@param psFilename	file location of pixel shader

	*/
	void InitShader(WCHAR* vsFilename, WCHAR* hsFilename, WCHAR* dsFilename, WCHAR* psFilename);

	/*
	Buffer to hold matrix information for shader
	*/
	ID3D11Buffer* matrixBuffer;

	/*
	Sample state for given texture
	*/
	ID3D11SamplerState* sampleState;

	/*
	Sample state for given text with clamping
	*/
	ID3D11SamplerState* sampleStateClamp;

	/*
	Buffer for water setting to send to GPU
	*/
	ID3D11Buffer* waterBuffer;


};

