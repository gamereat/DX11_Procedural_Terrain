// texture.cpp
#include "texture.h"

Texture::Texture(ID3D11Device* device, ID3D11DeviceContext* deviceContext, WCHAR* filename)
{
	HRESULT result;

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

bool Texture::does_file_exist(const WCHAR *fname)
{
	std::ifstream infile(fname);
	return infile.good();
}