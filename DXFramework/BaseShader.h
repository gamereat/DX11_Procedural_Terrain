// testure shader.h
#ifndef _BASESHADER_H_
#define _BASESHADER_H_


#include <d3d11.h>
#include <D3Dcompiler.h>
#include <dxgi.h>
#include <DirectXMath.h>
#include <fstream>

using namespace std;
using namespace DirectX;


class BaseShader
{
protected:
	struct MatrixBufferType
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
	};

public:
	void* operator new(size_t i)
	{
		return _mm_malloc(i, 16);
	}

	void operator delete(void* p)
	{
		_mm_free(p);
	}

	BaseShader(ID3D11Device* device, HWND hwnd);
	~BaseShader();

	virtual void Render(ID3D11DeviceContext* deviceContext, int vertexCount);

protected:
	virtual void InitShader(WCHAR*, WCHAR*) = 0;
	//void ShutdownShader();
	void OutputShaderErrorMessage(ID3D10Blob*, HWND, WCHAR*);
	void loadVertexShader(WCHAR* filename);
	void loadHullShader(WCHAR* filename);
	void loadDomainShader(WCHAR* filename);
	void loadGeometryShader(WCHAR* filename);
	void loadPixelShader(WCHAR* filename);




	void loadVertexShader(WCHAR* filename, const  char*  functionEntyPoint);
	void loadHullShader(WCHAR* filename, const  char*  functionEntyPoint);
	void loadDomainShader(WCHAR* filename, const  char* functionEntyPoint);
	void loadGeometryShader(WCHAR* filename, const  char* functionEntyPoint);
	void loadPixelShader(WCHAR* filename, const  char* functionEntyPoint);
protected:
	ID3D11Device* m_device;
	HWND m_hwnd;
	
	ID3D11VertexShader* m_vertexShader;
	ID3D11PixelShader* m_pixelShader;
	ID3D11HullShader* m_hullShader;
	ID3D11DomainShader* m_domainShader;
	ID3D11GeometryShader* m_geometryShader;
	ID3D11InputLayout* m_layout;
	ID3D11Buffer* m_matrixBuffer;
	ID3D11SamplerState* m_sampleState;
};

#endif