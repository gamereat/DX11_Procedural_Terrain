#pragma once
#include "..\DXFramework\BaseShader.h"
#include "Scene.h"
#include "ShaderBuffers.h"
using namespace std;
using namespace DirectX;
/* See the file "LICENSE" for the full license governing this code. */

/*
* Water Shdaer
*
* @author      Alan Yeats
*
*/
class TerrainShader :	public BaseShader
{
public:
 
	/*
	* @param device DirectX11 Device used for window
	* @param hwnd	Handle to window
	*/
	TerrainShader(ID3D11Device* device, HWND hwnd );
	~TerrainShader();

	/*
	@param deviceContext					Device context for directx window
	@param worldMatrix						World Matrix for current scene
	@param viewMatrix						View Matrix for current Scene
	@param projectionMatrix					projection Matrix for current Scene
	@param defaultTexture					Default terrain given to the terrain
	@param terrinSetting					Terrain Data to be sent to gpu
	@param faultLineSettings				Fault line displacement settings to be sent to gpu
	@param terrainTextureSettings			Terrain texture settings to be sent to gpu
	@param fractionalBrowningNoiseSettings	Fnm Settings to be sent to gpu
	@param light							Light Data to be sent to gpu
	@param depthMap							depth maps to be sent to gpu
	@param lowAreaTexture					Texture to be used at low level of terrain
	@param mediumAreaTexture				Texture to be used at medium level of terrain
	@param higherAreaTexture				Texture to be used at higher level of terrain
	@param underWaterTexture				Texture to be used at for terrain under water
	@param hitByWaterTexture				Texture to be used at for terrain that could be hit by water

	*/
	void SetShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX &worldMatrix,
		const XMMATRIX &viewMatrix, const XMMATRIX &projectionMatrix,
		ID3D11ShaderResourceView* defaultTexture, 
		TerrainGenerationBufferType* terrinSetting, FaultLineDisplacementBufferType* faultLineSettings, TerrainSettingTextureType* terrainTextureSettings,
		FractionalBrowningNoiseBuffer* fractionalBrowningNoiseSettings, 
		Light* light[NUM_LIGHTS], ID3D11ShaderResourceView*depthMap[],
		ID3D11ShaderResourceView* lowAreaTexture, ID3D11ShaderResourceView* mediumAreaTexture, ID3D11ShaderResourceView* higherAreaTexture,
		ID3D11ShaderResourceView* underWaterTexture, ID3D11ShaderResourceView* hitByWaterTexture);

	/*
	@param deviceContext	Device context for directx window
	@param vertexCount

	*/
	void Render(ID3D11DeviceContext * deviceContext, int indexCount);

 
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
	Buffer to hold primary light information
	*/
	ID3D11Buffer* lightBuffer;


	/*
	Buffer to hold additioanl light data
	*/
	ID3D11Buffer* secondaryLightBuffer;

	/*
	Buffer to hold terrian texture data
	*/
	ID3D11Buffer* terrainTexturingBuffer;;

	/*
	Buffer to hold terrian FBN data
	*/
	ID3D11Buffer* fractionalBrowningNoiseBuffer;;

	/*
	Buffer to hold terrian fault line displacement data
	*/
	ID3D11Buffer* faultLineBuffer;

	/*
	Buffer to hold terrian settings for generation
	*/
	ID3D11Buffer* terrainGenerationBuffer;


};

