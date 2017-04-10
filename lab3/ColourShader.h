
#ifndef _COLOURSHADER_H_
#define _COLOURSHADER_H_

#include "../DXFramework/BaseShader.h"

using namespace std;
using namespace DirectX;

/*
* Colour Shdaer
*
* @author      Alan Yeats
*
* Basic shader for pure colour outputing
*/
class ColourShader : public BaseShader
{

public:

	/*
	* @param device DirectX11 Device used for window
	* @param hwnd	Handle to window
	*/
	ColourShader(ID3D11Device* device, HWND hwnd);
	~ColourShader();

	/*
	@param deviceContext	Device context for directx window
	@param worldMatrix		World Matrix for current scene
	@param viewMatrix		View Matrix for current Scene
	@param projectionMatrix projection Matrix for current Scene 
	*/
	void SetShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX &worldMatrix, const XMMATRIX &viewMatrix, const XMMATRIX &projectionMatrix);
	
	
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
	Buffer to hold matrix information for shader
	*/
	ID3D11Buffer* matrixBuffer;
};

#endif