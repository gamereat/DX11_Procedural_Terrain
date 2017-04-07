// texture shader.h
#ifndef _TEXTURESHADER_H_
#define _TEXTURESHADER_H_

#include "../DXFramework/BaseShader.h"

using namespace std;
using namespace DirectX;

/*
* Texture Shader
* 
* @author      Alan Yeats
*
* Used as a basic shader to simply texture a object
*/
class TextureShader : public BaseShader
{

public:

	/*
	* @param device DirectX11 Device used for window
	* @param hwnd	Handle to window
	*/
	TextureShader(ID3D11Device* device, HWND hwnd);
	~TextureShader();

	/*
	@param deviceContext	Device context for directx window
	@param worldMatrix		World Matrix for current scene
	@param viewMatrix		View Matrix for current Scene
	@param projectionMatrix projection Matrix for current Scene
	@param texture			Texture for the water
	*/
	void SetShaderParameters(ID3D11DeviceContext* deviceContext, 
		const XMMATRIX &worldMatrix, const XMMATRIX &viewMartix, const XMMATRIX &projectionMatrix, 
		ID3D11ShaderResourceView* texture);


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
	void InitShader(WCHAR*, WCHAR*);
	
	/*
	Buffer to hold matrix information for shader
	*/
	ID3D11Buffer* matrixBuffer;

	/*
	Sample state for given texture
	*/
	ID3D11SamplerState* sampleState;
};

#endif