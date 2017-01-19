#include "Terrain.h"

#include "../imgui/imgui.h"

Terrain::Terrain(ID3D11Device* device, ID3D11DeviceContext* deviceContext, WCHAR* textureFilename, int resolution)
: PlaneMesh(device, deviceContext, textureFilename, resolution)
{
	isTerrainGeneratedEnabled = false;
	generateTerrain = false;

	heightMap = nullptr;
}


Terrain::~Terrain()
{
	PlaneMesh::~PlaneMesh();

	if (heightMap)
	{
		delete[] heightMap;
		heightMap = nullptr;
	}

}

bool Terrain::InitializeTerrain(ID3D11Device* device, int terrainWidth, int terrainHeight)
{
	int index;
	float height = 0.0;
	bool result;

	// Save the dimensions of the terrain.
	this->terrainWidth = terrainWidth;
	this->terrainHeight = terrainHeight;

	// Create the structure to hold the terrain data.
	this->heightMap = new HeightMapType[this->terrainWidth * this->terrainHeight];
	if (!this->heightMap)
	{
		return false;
	}

	// Initialise the data in the height map (flat).
	for (int j = 0; j<this->terrainHeight; j++)
	{
		for (int i = 0; i<this->terrainWidth; i++)
		{
			index = (this->terrainHeight * j) + i;

			this->heightMap[index].x = (float)i;
			this->heightMap[index].y = (float)height;
			this->heightMap[index].z = (float)j;

		}
	} 
	// Initialize the vertex and index buffer that hold the geometry for the terrain.
	InitBuffers(device);
 

	return true;
}

bool Terrain::GenerateHeightMap(ID3D11Device * device, bool keydown)
{

	bool result;
	//the toggle is just a bool that I use to make sure this is only called ONCE when you press a key
	//until you release the key and start again. We dont want to be generating the terrain 500
	//times per second. 
	if (generateTerrain && (!isTerrainGeneratedEnabled))
	{
		int index;
		float height = 0.0;


		//loop through the terrain and set the hieghts how we want. This is where we generate the terrain
		//in this case I will run a sin-wave through the terrain in one axis.

		for (int j = 0; j<terrainHeight; j++)
		{
			for (int i = 0; i<terrainWidth; i++)
			{
				index = (terrainHeight * j) + i;

				heightMap[index].x = (float)i;
				heightMap[index].y = (float)(sin((float)i / (terrainWidth / 12))*3.0); //magic numbers ahoy, just to ramp up the height of the sin function so its visible.
				heightMap[index].z = (float)j;
			}
		}



		// Initialize the vertex and index buffer that hold the geometry for the terrain.
		InitBuffers(device);

		isTerrainGeneratedEnabled = true;
	}
	else
	{
		isTerrainGeneratedEnabled = false;
	}




	return true;
}
void Terrain::InitBuffers(ID3D11Device* device)
{
	VertexType* vertices;
	unsigned long* indices;
	int index, i, j;
	float positionX, positionZ, u, v, increment;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	int index1, index2, index3, index4;

	// Calculate the number of vertices in the terrain mesh.
	m_vertexCount = (m_resolution - 1) * (m_resolution - 1) * 8;

	// Set the index count to the same as the vertex count.
	m_indexCount = m_vertexCount;

	// Create the vertex array.
	vertices = new VertexType[m_vertexCount];

	// Create the index array.
	indices = new unsigned long[m_indexCount];

	// Initialize the index to the vertex array.
	index = 0;

	// UV coords.
	u = 0;
	v = 0;
	increment = 1.0f / m_resolution;


	// Load the vertex and index arrays with the terrain data.
	for (j = 0; j<(m_resolution - 1); j++)
	{
		for (i = 0; i<(m_resolution - 1); i++)

		{
			index1 = (terrainHeight * j) + i;          // Bottom left.
			index2 = (terrainHeight * j) + (i + 1);      // Bottom right.
			index3 = (terrainHeight * (j + 1)) + i;      // Upper left.
			index4 = (terrainHeight * (j + 1)) + (i + 1);  // Upper right.

														   // Bottom left.
				// Upper right.
			vertices[index].position = XMFLOAT3(heightMap[index4].x, heightMap[index4].y, heightMap[index4].z);
			vertices[index].normal = XMFLOAT3(heightMap[index4].nx, heightMap[index4].ny, heightMap[index4].nz);
			indices[index] = index;
			index++;
			
			vertices[index].position = XMFLOAT3(heightMap[index1].x, heightMap[index1].y, heightMap[index1].z);
			vertices[index].normal = XMFLOAT3(heightMap[index1].nx, heightMap[index1].ny, heightMap[index1].nz);
			indices[index] = index;
			index++;

		

			// Bottom right.
			vertices[index].position = XMFLOAT3(heightMap[index2].x, heightMap[index2].y, heightMap[index2].z);
			vertices[index].normal = XMFLOAT3(heightMap[index2].nx, heightMap[index2].ny, heightMap[index2].nz);
			indices[index] = index;
			index++;
			

		// Upper right.
			vertices[index].position = XMFLOAT3(heightMap[index4].x, heightMap[index4].y, heightMap[index4].z);
			vertices[index].normal = XMFLOAT3(heightMap[index4].nx, heightMap[index4].ny, heightMap[index4].nz);
			indices[index] = index;
			index++;


			// Upper left.
			vertices[index].position = XMFLOAT3(heightMap[index3].x, heightMap[index3].y, heightMap[index3].z);
			vertices[index].normal = XMFLOAT3(heightMap[index3].nx, heightMap[index3].ny, heightMap[index3].nz);
			indices[index] = index;
			index++;

	
			// Bottom left.
			vertices[index].position = XMFLOAT3(heightMap[index1].x, heightMap[index1].y, heightMap[index1].z);
			vertices[index].normal = XMFLOAT3(heightMap[index1].nx, heightMap[index1].ny, heightMap[index1].nz);
			indices[index] = index;
			index++;

	

		}
		 
	}

	// Set up the description of the static vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType)* m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Now create the vertex buffer.
	device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);

	// Set up the description of the static index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long)* m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);

	// Release the arrays now that the buffers have been created and loaded.
	delete[] vertices;
	vertices = 0;

	delete[] indices;
	indices = 0;

 }
void Terrain::SendData(ID3D11DeviceContext * deviceContext)
{
	unsigned int stride;
	unsigned int offset;

	// Set vertex buffer stride and offset.
	stride = sizeof(VertexType);
	offset = 0;

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	// Set the index buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return;
}

void Terrain::Settings(bool* is_open)
{
	if (*is_open == true)
	{ 
		// Create the window
		if (!ImGui::Begin("Terrain Settings", is_open, ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::End();
			return;
		}

		ImGui::Checkbox("Generate terrain", &generateTerrain);

		ImGui::End();

	}

 }

bool Terrain::LoadHeightMap(char * filename)
{
	FILE* filePtr;
	int error;
	unsigned int count;
	BITMAPFILEHEADER bitmapFileHeader;
	BITMAPINFOHEADER bitmapInfoHeader;
	int imageSize, i, j, k, index;
	unsigned char* bitmapImage;
	unsigned char height;


	// Open the height map file in binary.
	error = fopen_s(&filePtr, filename, "rb");
	if (error != 0)
	{
		return false;
	}

	// Read in the file header.
	count = fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, filePtr);
	if (count != 1)
	{
		return false;
	}

	// Read in the bitmap info header.
	count = fread(&bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, filePtr);
	if (count != 1)
	{
		return false;
	}

	// Save the dimensions of the terrain.
	terrainWidth = bitmapInfoHeader.biWidth;
	terrainHeight = bitmapInfoHeader.biHeight;

	// Calculate the size of the bitmap image data.
	imageSize = terrainWidth * terrainHeight * 3;

	// Allocate memory for the bitmap image data.
	bitmapImage = new unsigned char[imageSize];
	if (!bitmapImage)
	{
		return false;
	}

	// Move to the beginning of the bitmap data.
	fseek(filePtr, bitmapFileHeader.bfOffBits, SEEK_SET);

	// Read in the bitmap image data.
	count = fread(bitmapImage, 1, imageSize, filePtr);
	if (count != imageSize)
	{
		return false;
	}

	// Close the file.
	error = fclose(filePtr);
	if (error != 0)
	{
		return false;
	}

	// Create the structure to hold the height map data.
	heightMap = new HeightMapType[terrainWidth * terrainHeight];
	if (!heightMap)
	{
		return false;
	}

	// Initialize the position in the image data buffer.
	k = 0;

	// Read the image data into the height map.
	for (j = 0; j<terrainHeight; j++)
	{
		for (i = 0; i<terrainWidth; i++)
		{
			height = bitmapImage[k];

			index = (terrainHeight * j) + i;

			heightMap[index].x = (float)i;
			heightMap[index].y = (float)height;
			heightMap[index].z = (float)j;

			k += 3;
		}
	}

	// Release the bitmap image data.
	delete[] bitmapImage;
	bitmapImage = 0;

	return true;
}

void Terrain::NormalizeHeightMap()
{
	int i, j;


	for (j = 0; j<terrainHeight; j++)
	{
		for (i = 0; i<terrainWidth; i++)
		{
			heightMap[(terrainHeight * j) + i].y /= 15.0f;
		}
	}

	return;
}

 