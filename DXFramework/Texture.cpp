#include "Texture.h"
#include "Texture.h"
#include "Texture.h"
// texture.cpp
#include "texture.h"

Texture::Texture(ID3D11Device* device, ID3D11DeviceContext* deviceContext, WCHAR* filename)
{
	HRESULT result;
//	filename = L"../res/bunny.png";

	// check if file exists
	if (!filename)
	{
		filename = L"../res/DefaultDiffuse.png";
	}
	// if not set default texture
	if (!does_file_exist(filename))
	{
		// change default texture
		filename = L"../res/DefaultDiffuse.png";
	}

	// check file extension for correct loading function.
	std::wstring fn(filename);
	std::string::size_type idx;
	std::wstring extension;

	idx = fn.rfind('.');

	if (idx != std::string::npos)
	{
		extension = fn.substr(idx + 1);
	}
	else
	{
		// No extension found
	}

	// Load the texture in.
	if (extension == L"dds")
	{
	 	result = CreateDDSTextureFromFile(device, deviceContext, filename, NULL, &m_texture, 0, NULL);
	}
	else
	{
	 	result = CreateWICTextureFromFile(device, deviceContext, filename, NULL, &m_texture, 0);
	}

	
 	 if (FAILED(result))
	{
	 	MessageBox(NULL, L"Texture loading error", L"ERROR", MB_OK);
	}
}

Texture::~Texture()
{
	// Release the texture resource.
	if (m_texture)
	{
		m_texture->Release();
		m_texture = 0;
	}
}


ID3D11ShaderResourceView* Texture::GetTexture()
{
	return m_texture;
}

ID3D11Texture2D* Texture::CreateDynamicTexture(ID3D11Device * device, int width, int height, float * buf, D3D11_TEXTURE2D_DESC &desc)
{
 	D3D11_SUBRESOURCE_DATA tbsd;

	int bpp = 12;

	tbsd.pSysMem = (void *)buf;
	tbsd.SysMemPitch = width * bpp;
	tbsd.SysMemSlicePitch = width * height * bpp; // Not needed since this is a 2d texture

	desc.Width = width;
	desc.Height = height;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0; 
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;
	 

	ID3D11Texture2D *pTexture = NULL;
	device->CreateTexture2D(&desc, &tbsd, &pTexture);

	return pTexture;

}

ID3D11ShaderResourceView* Texture::SetTexture(ID3D11Texture2D * texture, ID3D11Device* device, D3D11_TEXTURE2D_DESC* textureDesc)
{
	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
	shaderResourceViewDesc.Format = textureDesc->Format;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	shaderResourceViewDesc.Texture2D.MipLevels = 1;
 
	ID3D11ShaderResourceView* m_shaderResourceView;
	;
	device->CreateShaderResourceView(texture, &shaderResourceViewDesc, &m_shaderResourceView);


	return m_shaderResourceView;

 }

bool Texture::does_file_exist(const WCHAR *fname)
{
	std::ifstream infile(fname);
	return infile.good();
}