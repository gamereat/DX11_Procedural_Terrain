// D3D.h
#ifndef _D3D_H_
#define _D3D_H_

// Link libaries dxgi.lib, d3d11.lib, d3dx11.lib, d3dx10.lib

#include <Windows.h>
#include <d3d11.h>
 #include <d3dcompiler.h>
#include <DirectXMath.h>
#include "../imgui/examples/directx11_example/imgui_impl_dx11.h"
using namespace DirectX;
struct VERTEX_CONSTANT_BUFFER
{
	float        mvp[4][4];
};
class D3D
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

	D3D(int screenWidth, int screenHeight, bool vsync, HWND hwnd, bool fullscreen, float screenDepth, float screenNear);
	~D3D();

	void BeginScene(float r, float g, float b, float a);
	void EndScene();

	ID3D11Device* GetDevice();
	ID3D11DeviceContext* GetDeviceContext();

	void GetProjectionMatrix(XMMATRIX&);
	void GetWorldMatrix(XMMATRIX&);
	void GetOrthoMatrix(XMMATRIX&);

	void GetVideoCardInfo(char*, int&);

	void TurnZBufferOn();
	void TurnZBufferOff();
	void TurnOnAlphaBlending();
	void TurnOffAlphaBlending();
	void TurnOnWireframe();
	void TurnOffWireframe();

	void SetBackBufferRenderTarget();
	void ResetViewport();


	void DirectXSettingsMenu(bool* is_open);
	
	/*
		Returns what the wire frame mode is currently at
	*/
	bool getWireFrameMode();
 
protected:
	bool m_vsync_enabled;
	int m_videoCardMemory;
	char m_videoCardDescription[128];
	IDXGISwapChain* m_swapChain;
	ID3D11Device* m_device;
	ID3D11DeviceContext* m_deviceContext;
	ID3D11RenderTargetView* m_renderTargetView;
	ID3D11Texture2D* m_depthStencilBuffer;
	ID3D11DepthStencilState* m_depthStencilState;
	ID3D11DepthStencilView* m_depthStencilView;
	ID3D11RasterizerState* m_rasterState;
	ID3D11RasterizerState* m_rasterStateWF;
	XMMATRIX m_projectionMatrix;
	XMMATRIX m_worldMatrix;
	XMMATRIX m_orthoMatrix;
	ID3D11DepthStencilState* m_depthDisabledStencilState;
	ID3D11BlendState* m_alphaEnableBlendingState;
	ID3D11BlendState* m_alphaDisableBlendingState;
	D3D11_VIEWPORT viewport;

	bool m_is_WireFrame_on;
	bool m_is_AlphaBending_on;
	bool m_is_ZBuffer_on;

};

#endif