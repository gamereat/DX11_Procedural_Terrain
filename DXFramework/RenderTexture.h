#ifndef _RENDERTEXTURE_H_
#define _RENDERTEXTURE_H_

#include <d3d11.h>
#include <directxmath.h>

using namespace DirectX;

class RenderTexture
{
public:
	void* operator new(size_t i)
	{
		return _mm_malloc(i, 16);
	}

	void operator delete(void* p)
	{
		_mm_free(p);
	}

	RenderTexture(ID3D11Device* device, int textureWidth, int textureHeight, float screenNear, float screenDepth);
	~RenderTexture();

	void SetRenderTarget(ID3D11DeviceContext* deviceContext);
	void ClearRenderTarget(ID3D11DeviceContext* deviceContext, float red, float green, float blue, float alpha);
	ID3D11ShaderResourceView* GetShaderResourceView();

	XMMATRIX GetProjectionMatrix();
	XMMATRIX GetOrthoMatrix();

	int GetTextureWidth();
	int GetTextureHeight();

private:
	int m_textureWidth, m_textureHeight;
	ID3D11Texture2D* m_renderTargetTexture;
	ID3D11RenderTargetView* m_renderTargetView;
	ID3D11ShaderResourceView* m_shaderResourceView;
	ID3D11Texture2D* m_depthStencilBuffer;
	ID3D11DepthStencilView* m_depthStencilView;
	D3D11_VIEWPORT m_viewport;
	XMMATRIX m_projectionMatrix;
	XMMATRIX m_orthoMatrix;
};

#endif