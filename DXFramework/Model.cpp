// Model mesh and load
// Loads a .obj and creates a mesh object from the data
#include "model.h"

Model::Model(ID3D11Device* device, ID3D11DeviceContext* deviceContext, WCHAR* textureFilename, WCHAR* filename)
{
	// Load model data
	LoadModel(filename);

	// Initialize the vertex and index buffer that hold the geometry for the model.
	InitBuffers(device);

	// Load the texture for this model.
	LoadTexture(device, deviceContext, textureFilename);
}


Model::~Model()
{
	// Run parent deconstructor
	BaseMesh::~BaseMesh();

	if (m_model)
	{
		delete[] m_model;
		m_model = 0;
	}

}



void Model::InitBuffers(ID3D11Device* device)
{
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;

	
	vertices = new VertexType[m_vertexCount];
	
	
	indices = new unsigned long[m_indexCount];
	
	// Load the vertex array and index array with data.
	for (int i = 0; i<m_vertexCount; i++)
	{
		vertices[i].position = XMFLOAT3(m_model[i].x, m_model[i].y, -m_model[i].z);
		vertices[i].texture = XMFLOAT2(m_model[i].tu, m_model[i].tv);
		vertices[i].normal = XMFLOAT3(m_model[i].nx, m_model[i].ny, -m_model[i].nz);

		indices[i] = i;
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

	// Release the arrays now that the vertex and index buffers have been created and loaded.
	delete[] vertices;
	vertices = 0;

	delete[] indices;
	indices = 0;
}

void Model::LoadModel(WCHAR* filename)
{
	// Process model file
	std::ifstream fileStream;
	int fileSize = 0;

	fileStream.open(filename, std::ifstream::in);

	if (fileStream.is_open() == false)
		MessageBox(NULL, filename, L"Missing Model File", MB_OK);

	fileStream.seekg(0, std::ios::end);
	fileSize = (int)fileStream.tellg();
	fileStream.seekg(0, std::ios::beg);

	if (fileSize <= 0)
		MessageBox(NULL, filename, L"Model file empty", MB_OK);

	char *buffer = new char[fileSize];

	if (buffer == 0)
		MessageBox(NULL, filename, L"Model buffer is to small", MB_OK);

	memset(buffer, '\0', fileSize);

	TokenStream tokenStream, lineStream, faceStream;
	std::string tempLine, token;

	fileStream.read(buffer, fileSize);
	tokenStream.SetTokenStream(buffer);

	delete[] buffer;

	tokenStream.ResetStream();

	std::vector<float> verts, norms, texC;
	std::vector<int> faces;


	char lineDelimiters[2] = { '\n', ' ' };

	while (tokenStream.MoveToNextLine(&tempLine))
	{
		lineStream.SetTokenStream((char*)tempLine.c_str());
		tokenStream.GetNextToken(0, 0, 0);

		if (!lineStream.GetNextToken(&token, lineDelimiters, 2))
			continue;

		if (strcmp(token.c_str(), "v") == 0)
		{
			lineStream.GetNextToken(&token, lineDelimiters, 2);
			verts.push_back((float)atof(token.c_str()));

			lineStream.GetNextToken(&token, lineDelimiters, 2);
			verts.push_back((float)atof(token.c_str()));

			lineStream.GetNextToken(&token, lineDelimiters, 2);
			verts.push_back((float)atof(token.c_str()));
		}
		else if (strcmp(token.c_str(), "vn") == 0)
		{
			lineStream.GetNextToken(&token, lineDelimiters, 2);
			norms.push_back((float)atof(token.c_str()));

			lineStream.GetNextToken(&token, lineDelimiters, 2);
			norms.push_back((float)atof(token.c_str()));

			lineStream.GetNextToken(&token, lineDelimiters, 2);
			norms.push_back((float)atof(token.c_str()));
		}
		else if (strcmp(token.c_str(), "vt") == 0)
		{
			lineStream.GetNextToken(&token, lineDelimiters, 2);
			texC.push_back((float)atof(token.c_str()));

			lineStream.GetNextToken(&token, lineDelimiters, 2);
			texC.push_back((float)atof(token.c_str()));
		}
		else if (strcmp(token.c_str(), "f") == 0)
		{
			char faceTokens[3] = { '\n', ' ', '/' };
			std::string faceIndex;

			faceStream.SetTokenStream((char*)tempLine.c_str());
			faceStream.GetNextToken(0, 0, 0);

			for (int i = 0; i < 3; i++)
			{
				faceStream.GetNextToken(&faceIndex, faceTokens, 3);
				faces.push_back((int)atoi(faceIndex.c_str()));

				faceStream.GetNextToken(&faceIndex, faceTokens, 3);
				faces.push_back((int)atoi(faceIndex.c_str()));

				faceStream.GetNextToken(&faceIndex, faceTokens, 3);
				faces.push_back((int)atoi(faceIndex.c_str()));
			}
		}
		else if (strcmp(token.c_str(), "#") == 0)
		{
			int a = 0;
			int b = a;
		}

		token[0] = '\0';
	}

	// "Unroll" the loaded obj information into a list of triangles.

	int vIndex = 0, nIndex = 0, tIndex = 0;
	int numFaces = (int)faces.size() / 9;

	//// Create the model using the vertex count that was read in.
	m_vertexCount = numFaces * 3;
	m_model = new ModelType[m_vertexCount];
	
	for (int f = 0; f < (int)faces.size(); f += 3)
	{
		m_model[vIndex].x = verts[(faces[f + 0] - 1) * 3 + 0];
		m_model[vIndex].y = verts[(faces[f + 0] - 1) * 3 + 1];
		m_model[vIndex].z = verts[(faces[f + 0] - 1) * 3 + 2];
		m_model[vIndex].tu = texC[(faces[f + 1] - 1) * 2 + 0];
		m_model[vIndex].tv = texC[(faces[f + 1] - 1) * 2 + 1];
		m_model[vIndex].nx = norms[(faces[f + 2] - 1) * 3 + 0];
		m_model[vIndex].ny = norms[(faces[f + 2] - 1) * 3 + 1];
		m_model[vIndex].nz = norms[(faces[f + 2] - 1) * 3 + 2];

		//increase index count
		vIndex++;

	}
	m_indexCount = vIndex;

	verts.clear();
	norms.clear();
	texC.clear();
	faces.clear();
}

