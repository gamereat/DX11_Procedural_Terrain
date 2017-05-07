
#ifndef _DEPTHSHADER_H_
#define _DEPTHSHADER_H_

#include "../DXFramework/BaseShader.h"

using namespace std;
using namespace DirectX;
/* See the file "LICENSE" for the full license governing this code. */


/*
* Depth Shdaer
*
* @author      Alan Yeats
* Depth shader create a depth texture base based on light pos
*/
class DepthShader : public BaseShader
{

public:
	
	/*
	* @param device DirectX11 Device used for window
	* @param hwnd	Handle to window
	*/
	DepthShader(ID3D11Device* device, HWND hwnd);
	~DepthShader();



	/*
	@param deviceContext	Device context for directx window
	@param worldMatrix		World Matrix for current scene
	@param viewMatrix		View Matrix for current Scene
	@param projectionMatrix projection Matrix for current Scene 

	*/
	void SetShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX &worldMatrix, const XMMATRIX &viewMatrix, const XMMATRIX &projectionMatrix);


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