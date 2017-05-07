#ifndef _LSYSTEMSHADER_H_
#define _LSYSTEMSHADER_H_

#include "../DXFramework/BaseShader.h"

using namespace std;
using namespace DirectX;
/* See the file "LICENSE" for the full license governing this code. */

/*
* L-system Shdaer
*
* @author      Alan Yeats
*
*/
class LSystemShader : public BaseShader
{

public:
	/*
	* @param device DirectX11 Device used for window
	* @param hwnd	Handle to window
	*/
	LSystemShader(ID3D11Device* device, HWND hwnd);
	~LSystemShader();

	/*
	@param deviceContext	Device context for directx window
	@param worldMatrix		World Matrix for current scene
	@param viewMatrix		View Matrix for current Scene
	@param projectionMatrix projection Matrix for current Scene
	@param texture			Texture for the l-system 

	*/
	void SetShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX &world,
		const XMMATRIX &view, const XMMATRIX &projection, ID3D11ShaderResourceView* texture);

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

	/*
	Sample state for given texture
	*/
	ID3D11SamplerState* sampleState;

};

#endif