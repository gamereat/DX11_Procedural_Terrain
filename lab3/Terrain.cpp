#include "Terrain.h"
#include <random>
#include "../imgui/imgui.h"
#include "Sound.h"
#include "SimplexNoise.h"
#include "FractionalBrownianMotion.h"
#include "DiamondSquare.h"
Terrain::Terrain(std::string name, ID3D11Device* device, ID3D11DeviceContext* deviceContext, WCHAR* textureFilename, int resolution)
: PlaneMesh(name,device, deviceContext, textureFilename, resolution)
{
	generateTerrain = false;

	heightMap = nullptr;

	xAxisWaveSettings.amplitude = 1;
	yAxisWaveSettings.amplitude = 1;
	zAxisWaveSettings.amplitude = 1;


	xAxisWaveSettings.period = 1;
	yAxisWaveSettings.period = 1;
	zAxisWaveSettings.period = 1;


	xAxisWaveSettings.waveType = waveSettings::WaveType::cos;
	yAxisWaveSettings.waveType = waveSettings::WaveType::cos;
	zAxisWaveSettings.waveType = waveSettings::WaveType::cos;


	diamondSquareRange = 10;

	smoothingValue = false;
 
	perlinNoiseFrequancy = 1;
	perlinNoiseHeightRange = 1;

	smoothingValue = 1;
}


Terrain::~Terrain()
{
	PlaneMesh::~PlaneMesh();

	if (heightMap)
	{
		delete[] heightMap;
		heightMap = nullptr;
	}

	if (startingHeightmap)
	{
		delete[] startingHeightmap;
		startingHeightmap = nullptr;
	}

}
bool Terrain::InitializeTerrain(ID3D11Device* device, int terrainWidth, int terrainHeight)
{
	int index;
	float height = 0.0;
 
	// Save the dimensions of the terrain.
	this->terrainWidth = terrainWidth;
	this->terrainHeight = terrainHeight; 
	// Create the structure to hold the terrain data.
	this->heightMap = new HeightMapType[this->terrainWidth * this->terrainHeight];
	this->startingHeightmap = new HeightMapType[this->terrainWidth * this->terrainHeight];
	this->perinNoiseValues = new double[this->terrainWidth * this->terrainHeight];
	this->diamondSquarePoints;;
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
			 
			double pelinNoise = SimplexNoise::noise(j, i, perlinNoiseFrequancy);
			perinNoiseValues[index] = pelinNoise;
			this->heightMap[index].x = (float)i;
			this->heightMap[index].y = (float)height;
			this->heightMap[index].z = (float)j;
			startingHeightmap[index].y = (float)height;
			diamondSquarePoints[index] = 0;

		}
	} 


	// Initialize the vertex and index buffer that hold the geometry for the terrain.
	InitBuffers(device); 
	return true;
}

bool Terrain::CalculateNormals()
{
	int i, j, index1, index2, index3, index, count;
	float vertex1[3], vertex2[3], vertex3[3], vector1[3], vector2[3], sum[3], length;
	VectorType* normals;


	// Create a temporary array to hold the un-normalized normal vectors.
	normals = new VectorType[(terrainHeight - 1) * (terrainWidth - 1)];
	if (!normals)
	{
		return false;
	}

	// Go through all the faces in the mesh and calculate their normals.
	for (j = 0; j<(terrainHeight - 1); j++)
	{
		for (i = 0; i<(terrainWidth - 1); i++)
		{
			index1 = (j * terrainHeight) + i;
			index2 = (j * terrainHeight) + (i + 1);
			index3 = ((j + 1) * terrainHeight) + i;

			// Get three vertices from the face.
			vertex1[0] = heightMap[index1].x;
			vertex1[1] = heightMap[index1].y;
			vertex1[2] = heightMap[index1].z;

			vertex2[0] = heightMap[index2].x;
			vertex2[1] = heightMap[index2].y;
			vertex2[2] = heightMap[index2].z;

			vertex3[0] = heightMap[index3].x;
			vertex3[1] = heightMap[index3].y;
			vertex3[2] = heightMap[index3].z;

			// Calculate the two vectors for this face.
			vector1[0] = vertex1[0] - vertex3[0];
			vector1[1] = vertex1[1] - vertex3[1];
			vector1[2] = vertex1[2] - vertex3[2];
			vector2[0] = vertex3[0] - vertex2[0];
			vector2[1] = vertex3[1] - vertex2[1];
			vector2[2] = vertex3[2] - vertex2[2];

			index = (j * (terrainHeight - 1)) + i;

			// Calculate the cross product of those two vectors to get the un-normalized value for this face normal.
			normals[index].x = (vector1[1] * vector2[2]) - (vector1[2] * vector2[1]);
			normals[index].y = (vector1[2] * vector2[0]) - (vector1[0] * vector2[2]);
			normals[index].z = (vector1[0] * vector2[1]) - (vector1[1] * vector2[0]);
		}
	}

	// Now go through all the vertices and take an average of each face normal 	
	// that the vertex touches to get the averaged normal for that vertex.
	for (j = 0; j<terrainHeight; j++)
	{
		for (i = 0; i<terrainWidth; i++)
		{
			// Initialize the sum.
			sum[0] = 0.0f;
			sum[1] = 0.0f;
			sum[2] = 0.0f;

			// Initialize the count.
			count = 0;

			// Bottom left face.
			if (((i - 1) >= 0) && ((j - 1) >= 0))
			{
				index = ((j - 1) * (terrainHeight - 1)) + (i - 1);

				sum[0] += normals[index].x;
				sum[1] += normals[index].y;
				sum[2] += normals[index].z;
				count++;
			}

			// Bottom right face.
			if ((i < (terrainWidth - 1)) && ((j - 1) >= 0))
			{
				index = ((j - 1) * (terrainHeight - 1)) + i;

				sum[0] += normals[index].x;
				sum[1] += normals[index].y;
				sum[2] += normals[index].z;
				count++;
			}

			// Upper left face.
			if (((i - 1) >= 0) && (j < (terrainHeight - 1)))
			{
				index = (j * (terrainHeight - 1)) + (i - 1);

				sum[0] += normals[index].x;
				sum[1] += normals[index].y;
				sum[2] += normals[index].z;
				count++;
			}

			// Upper right face.
			if ((i < (terrainWidth - 1)) && (j < (terrainHeight - 1)))
			{
				index = (j * (terrainHeight - 1)) + i;

				sum[0] += normals[index].x;
				sum[1] += normals[index].y;
				sum[2] += normals[index].z;
				count++;
			}

			// Take the average of the faces touching this vertex.
			sum[0] = (sum[0] / (float)count);
			sum[1] = (sum[1] / (float)count);
			sum[2] = (sum[2] / (float)count);

			// Calculate the length of this normal.
			length = sqrt((sum[0] * sum[0]) + (sum[1] * sum[1]) + (sum[2] * sum[2]));

			// Get an index to the vertex location in the height map array.
			index = (j * terrainHeight) + i;

			// Normalize the final shared normal for this vertex and store it in the height map array.
			heightMap[index].nx = (sum[0] / length);
			heightMap[index].ny = (sum[1] / length);
			heightMap[index].nz = (sum[2] / length);
		}
	}

	// Release the temporary normals.
	delete[] normals;
	normals = 0;

	return true;
}


bool Terrain::GenerateHeightMap(ID3D11Device * device, bool keydown, Sound* sound)
{

	if (diamondSquareNeedRegenerated)
	{	
		diamondSquareNeedRegenerated = false;
		GenerateDimondSquare();
		terrainNeedReGeneration = true;
 		
	}

	if (faultLineDisplacementRegenerated)
	{
		faultLineDisplacementRegenerated = false;
		GenerateFaultLineDisplacement();
		terrainNeedReGeneration = true;

	}
	if (simplexNoiseRegenerated)
	{		
		simplexNoiseRegenerated = false;

		GenerateSimplexNoiseNoise();
		terrainNeedReGeneration = true;
	}

	if (terrainNeedReGeneration )
	{
		terrainNeedReGeneration = false;

		if (enableSmoothing)
		{
			NormalizeHeightMap();
		}
		InitBuffers(device);

	}


	if (resetTerrain)
	{
		resetTerrain = false;
		InitializeTerrain(device, this->terrainHeight, this->terrainWidth);
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

	// Create the vertex array.	vertices = new VertexType[m_vertexCount];
	vertices = new VertexType[m_indexCount];
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
			vertices[index].texture = XMFLOAT2(u + increment, v + increment);
			indices[index] = index;
			index++;
			
			//bottom left
			vertices[index].position = XMFLOAT3(heightMap[index1].x, heightMap[index1].y, heightMap[index1].z);
			vertices[index].normal = XMFLOAT3(heightMap[index1].nx, heightMap[index1].ny, heightMap[index1].nz);	
			vertices[index].texture = XMFLOAT2(u  , v );
			indices[index] = index;
			index++;

		

			// Bottom right.
			vertices[index].position = XMFLOAT3(heightMap[index2].x, heightMap[index2].y, heightMap[index2].z);
			vertices[index].normal = XMFLOAT3(heightMap[index2].nx, heightMap[index2].ny, heightMap[index2].nz);
			vertices[index].texture = XMFLOAT2(u + increment , v );
			indices[index] = index;
			index++;
			

			// Upper right.
			vertices[index].position = XMFLOAT3(heightMap[index4].x, heightMap[index4].y, heightMap[index4].z);
			vertices[index].normal = XMFLOAT3(heightMap[index4].nx, heightMap[index4].ny, heightMap[index4].nz);
			vertices[index].texture = XMFLOAT2(u + increment, v + increment);
			indices[index] = index;
			index++;


			// Upper left.
			vertices[index].position = XMFLOAT3(heightMap[index3].x, heightMap[index3].y, heightMap[index3].z);
			vertices[index].normal = XMFLOAT3(heightMap[index3].nx, heightMap[index3].ny, heightMap[index3].nz);
			vertices[index].texture = XMFLOAT2(u , v + increment);
			indices[index] = index;
			index++;

	
			// Bottom left.
			vertices[index].position = XMFLOAT3(heightMap[index1].x, heightMap[index1].y, heightMap[index1].z);
			vertices[index].normal = XMFLOAT3(heightMap[index1].nx, heightMap[index1].ny, heightMap[index1].nz);
			vertices[index].texture = XMFLOAT2(u , v );
			indices[index] = index;
			index++;
 	
			u += increment;

		}
		u = 0;
		v += increment;

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

	CalculateNormals();



	// Release the arrays now that the buffers have been created and loaded.
	delete[] vertices;
	vertices = 0;

	delete[] indices;
	indices = 0;

 }

 XMMATRIX Terrain::SendData(ID3D11DeviceContext * deviceContext)
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




	// Set the position on screen
	return XMMatrixTranslation(translation.x, translation.y, translation.z) *
		XMMatrixScaling(scale.x, scale.y, scale.z) * XMMatrixRotationX(rotate.x) *
		XMMatrixRotationY(rotate.y) *  XMMatrixRotationZ(rotate.z);
}

void Terrain::Settings(bool* is_open , TerrainGeneration generation)
{
	if (*is_open == true)
	{

		if (generation == TerrainGeneration::DiamondSquare)
		{

			if (ImGui::Checkbox("Enable Smoothing", &enableSmoothing))
			{
				terrainNeedReGeneration = true;
				GenerateDimondSquare();
			}
			if (ImGui::DragFloat("Smoothing Value ", &smoothingValue))
			{
				terrainNeedReGeneration = true;

 			}

			if (ImGui::InputInt("Range", &diamondSquareRange))
			{
				GenerateDimondSquare();
				terrainNeedReGeneration = true;
			}
		}
		else if (generation == TerrainGeneration::SimplexNoise)
		{

			if (ImGui::Checkbox("Enable Smoothing", &enableSmoothing))
			{
				GenerateSimplexNoiseNoise();
				terrainNeedReGeneration = true;
				simplexNoiseRegenerated = true;

			}
			if (ImGui::DragFloat("Smoothing Value ", &smoothingValue))
			{
				terrainNeedReGeneration = true;

				simplexNoiseRegenerated = true;
			}
			if (ImGui::InputFloat("Perlin Noise Frequancy", &perlinNoiseFrequancy))
			{

				terrainNeedReGeneration = true;

				simplexNoiseRegenerated = true;
			}

			if (ImGui::InputFloat("Perlin Noise Height Range", &perlinNoiseHeightRange))
			{
				terrainNeedReGeneration = true;

				simplexNoiseRegenerated = true;
			}

			
		}
		else if (generation == TerrainGeneration::FractionalBrowningNoise)
		{

			if (ImGui::Checkbox("Enable Smoothing", &enableSmoothing))
			{
				GenerateFBmNoise();
				terrainNeedReGeneration = true;

			}
	
		}
		else if (generation == TerrainGeneration::FaultLineDisplacement)
		{
			if (ImGui::Checkbox("Enable Smoothing", &enableSmoothing))
			{
				faultLineDisplacementRegenerated = true;
				GenerateFaultLineDisplacement();
			}
			if (ImGui::DragFloat("Smoothing Value ", &smoothingValue))
			{
				terrainNeedReGeneration = true;

 			}
		}
	
		
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


	if (smoothingValue > 0)
	{

		for (j = 0; j < terrainHeight - 1; j++)
		{
			for (i = 0; i < terrainWidth - 1; i++)
			{
				float totalHeight = 0;
				int countPoints = 0;
				for (int aroundX = j - smoothingValue; aroundX <= j + smoothingValue; aroundX++)
				{
					if (aroundX < 0 || aroundX > terrainWidth - 1) continue;

					for (int aroundZ = i - smoothingValue; aroundZ <= i + smoothingValue; aroundZ++)
					{
						if (aroundZ < 0 || aroundZ > terrainHeight - 1) continue;


						int indexDF = (this->terrainHeight * aroundX) + aroundZ;

						totalHeight += heightMap[indexDF].y;
						countPoints++;

					}


				}

				if (countPoints != 0 && totalHeight != 0)
				{
					heightMap[(terrainHeight * j) + i].y = totalHeight / (float)countPoints;

				}

			}
		}
	}

	return;
}

void Terrain::GenerateDimondSquare()
{


 
		DiamondSquare::DiamondSquareAlgorithm(diamondSquarePoints, terrainWidth, terrainHeight, diamondSquareRange);

		//heightMap = startingHeightmap;
		int indexDF = 0;
		for (int j = 0; j < terrainHeight; j++)
		{
			for (int i = 0; i < terrainWidth; i++)
			{
				int indexDF = (this->terrainHeight * j) + i;
				this->heightMap[indexDF].y = diamondSquarePoints[indexDF] ;
			}
		}
		 
		 



	
}


void Terrain::GenerateSimplexNoiseNoise()
{
	 

	int index = 0;


	// Initialise the data in the height map (flat).
	for (int j = 0; j < this->terrainHeight; j++)
	{
		for (int i = 0; i < this->terrainWidth; i++)
		{
			index = (this->terrainHeight * j) + i;

			this->heightMap[index].x = (float)i;

			double pelinNoise;

			//if (simplexNoiseRegenerated)
			//{
			//	pelinNoise =;
			//	perinNoiseValues[index] = pelinNoise;

			//}
			//else
			//{
			//	pelinNoise = perinNoiseValues[index];
			//}

		
			
			this->heightMap[index].y = (float)SimplexNoise::noise(j, i, perlinNoiseFrequancy) * perlinNoiseHeightRange;

		
			this->heightMap[index].z = (float)j;

		}
	}
	simplexNoiseRegenerated = false;
 
}

void Terrain::GenerateFBmNoise()
{
}

void Terrain::GenerateFaultLineDisplacement()
{
	int index = 0;
	
	for (int j = 0; j < this->terrainHeight; j++)
	{
		for (int i = 0; i < this->terrainWidth; i++)
		{
			index = (this->terrainHeight * j) + i;

			this->heightMap[index].x = (float)i;


			this->heightMap[index].y = (float)FaultLineDisplacement(i, j);


			this->heightMap[index].z = (float)j;
		}
	}
	faultLineDisplacementRegenerated = false;
}

float Terrain::FaultLineDisplacement(int x, int z)
{
	float yAxisChange = 0;


	float displacement = faultLineSettings->startingDisplacement;
	for (int iter = 0; iter <  faultLineSettings->numberOfIterations; iter++)
	{



		XMFLOAT2 randPoint1, randPoint2;

		randPoint1 = XMFLOAT2((float)faultLineSettings->interationsRandomPoints[iter].x, (float)faultLineSettings->interationsRandomPoints[iter].y);
		randPoint2 = XMFLOAT2((float)faultLineSettings->interationsRandomPoints[iter].z, (float)faultLineSettings->interationsRandomPoints[iter].w);


		float a = (float)((float)randPoint2.y - (float)randPoint1.y);
		float b = (float)-((float)randPoint2.x - (float)randPoint1.x);
		float c = (float)(-(float)randPoint1.x * ((float)randPoint2.y - (float)randPoint1.y)) + ((float)randPoint1.y * ((float)randPoint2.x - (float)randPoint1.x));



		if (((a * z) + (b * x) - c) > 0)
		{
			yAxisChange += displacement;
		}
		else
		{
			yAxisChange -= displacement;
		}


		if (iter <  faultLineSettings->numberOfIterations)
		{
			displacement = faultLineSettings->startingDisplacement - ((float)iter / (float)faultLineSettings->numberOfIterations) * ((float)displacement - (float)faultLineSettings->minimumDisplacement);
		}
		else
		{
			displacement = faultLineSettings->startingDisplacement;
		}
	}




	return yAxisChange;
}
