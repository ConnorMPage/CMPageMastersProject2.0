#include "TerrainGen.h"
#include <array>

TerrainGen::TerrainGen()
{
}

bool TerrainGen::InitialiseTerrain(ID3D11Device* gfxDevice, std::string filename, bool norm)
{
	bool initialised;

	initialised = InitialiseHeightMap(filename);
	if (!initialised) return false;
	if(norm)NormHeightmap();

	initialised = CalcNormals();
	if (!initialised) return false;

	initialised = InitialiseBuffers(gfxDevice);


	return initialised;
}

void TerrainGen::RenderTerrain(ID3D11DeviceContext* gfxContext)
{
	RenderBuffers(gfxContext);
}

int TerrainGen::getIndexCount()
{
	return mIndexCount;
}

void TerrainGen::Update(float updateMulti)
{
}

bool TerrainGen::InitialiseHeightMap(std::string filename)
{
	//const char* add = std::filename;
	FILE* filePtr;
	int error;
	//../CMPageMastersProject2.0/Data/testingHMap.bmp
	//.. / CMPageMastersProject2.0 / output.bmp
	error = fopen_s(&filePtr, filename.c_str(), "rb");
	if (error != 0) return false;

	unsigned int count;
	BITMAPFILEHEADER bitmapFileHeader;
	count = fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, filePtr);
	if (count != 1)return false;

	BITMAPINFOHEADER bitmapInfoHeader;
	count = fread(&bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, filePtr);
	if (count != 1) return false;
	
	mTerrainWidth = bitmapInfoHeader.biWidth;
	mTerrainHeight = bitmapInfoHeader.biHeight;

	int imageSize = mTerrainWidth * mTerrainHeight * 3;

	unsigned char* bitmapImage = new unsigned char[imageSize];
	if (!bitmapImage) return false;

	fseek(filePtr, bitmapFileHeader.bfOffBits, SEEK_SET);
	
	count = fread(bitmapImage, 1, imageSize, filePtr);
	if (count != imageSize) return false;

	error = fclose(filePtr);
	if (error != 0) return false;

	// create heighmap structure

	pHeightMap = new HeightMapType[mTerrainWidth * mTerrainHeight];
	
	int k = 0;
	unsigned char height;
	int index;

	for (int j = 0; j < mTerrainHeight; j++)
	{
		for (int i = 0; i < mTerrainWidth; i++)
		{
			height = bitmapImage[k];
			index = (mTerrainHeight * j) + i;

			pHeightMap[index].x = i;
			pHeightMap[index].y = height;
			pHeightMap[index].z = j;

			k += 3;
		}
	}
	delete[]bitmapImage;

	return true;
}

void TerrainGen::NormHeightmap()
{
	for (int j = 0; j < mTerrainHeight; j++)
	{
		for (int i = 0; i < mTerrainWidth; i++)
		{
			pHeightMap[(mTerrainHeight * j) + i].y /= 15.0f;
		}
	}
}

bool TerrainGen::CalcNormals()
{
	int index1, index2, index3, index, count;
	float vertex1[3], vertex2[3], vertex3[3], vector1[3], vector2[3], sum[3], length;
	VectorType* normals;

	normals = new VectorType[(mTerrainHeight - 1) * (mTerrainWidth - 1)];
	if (!normals)
	{
		return false;
	}

	//calc mesh noramls for all faces 
	for (int j = 0; j < mTerrainHeight - 1; j++)
	{
		for (int i = 0; i < mTerrainWidth - 1; i++)
		{
			index1 = (j * mTerrainHeight) + i;
			index2 = (j * mTerrainHeight) + (i + 1);
			index3 = ((j + 1) * mTerrainHeight) + i;

			// Get three vertices from the face.
			vertex1[0] = pHeightMap[index1].x;
			vertex1[1] = pHeightMap[index1].y;
			vertex1[2] = pHeightMap[index1].z;

			vertex2[0] = pHeightMap[index2].x;
			vertex2[1] = pHeightMap[index2].y;
			vertex2[2] = pHeightMap[index2].z;

			vertex3[0] = pHeightMap[index3].x;
			vertex3[1] = pHeightMap[index3].y;
			vertex3[2] = pHeightMap[index3].z;

			// Calculate the two vectors for this face.
			vector1[0] = vertex1[0] - vertex3[0];
			vector1[1] = vertex1[1] - vertex3[1];
			vector1[2] = vertex1[2] - vertex3[2];
			vector2[0] = vertex3[0] - vertex2[0];
			vector2[1] = vertex3[1] - vertex2[1];
			vector2[2] = vertex3[2] - vertex2[2];

			index = (j * (mTerrainHeight - 1)) + i;

			// Calculate the cross product of those two vectors to get the un-normalized value for this face normal.
			normals[index].x = (vector1[1] * vector2[2]) - (vector1[2] * vector2[1]);
			normals[index].y = (vector1[2] * vector2[0]) - (vector1[0] * vector2[2]);
			normals[index].z = (vector1[0] * vector2[1]) - (vector1[1] * vector2[0]);
		}
	}


	for (int j = 0; j < mTerrainHeight; j++)
	{
		for (int i = 0; i < mTerrainWidth; i++)
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
				index = ((j - 1) * (mTerrainHeight - 1)) + (i - 1);

				sum[0] += normals[index].x;
				sum[1] += normals[index].y;
				sum[2] += normals[index].z;
				count++;
			}

			// Bottom right face.
			if ((i < (mTerrainWidth - 1)) && ((j - 1) >= 0))
			{
				index = ((j - 1) * (mTerrainHeight - 1)) + i;

				sum[0] += normals[index].x;
				sum[1] += normals[index].y;
				sum[2] += normals[index].z;
				count++;
			}

			// Upper left face.
			if (((i - 1) >= 0) && (j < (mTerrainHeight - 1)))
			{
				index = (j * (mTerrainHeight - 1)) + (i - 1);

				sum[0] += normals[index].x;
				sum[1] += normals[index].y;
				sum[2] += normals[index].z;
				count++;
			}

			// Upper right face.
			if ((i < (mTerrainWidth - 1)) && (j < (mTerrainHeight - 1)))
			{
				index = (j * (mTerrainHeight - 1)) + i;

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

			index = (j * mTerrainHeight) + i;

			
			pHeightMap[index].norm.x = (sum[0] / length);
			pHeightMap[index].norm.y = (sum[1] / length);
			pHeightMap[index].norm.z = (sum[2] / length);
		}
	}
	delete[] normals;
	return true;
}

bool TerrainGen::InitialiseBuffers(ID3D11Device* gfxDevice)
{
	
	unsigned long* indices;
	int index;
	float positionX, positionZ;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;


	int mVertexCount = (mTerrainWidth - 1) * (mTerrainHeight - 1) * 12;
	mIndexCount = mVertexCount;
	
	
	VertexType* vertices;
	vertices = new VertexType[mVertexCount];
	if (!vertices)return false;

	indices = new unsigned long[mIndexCount];
	
	if (!indices)
	{
		return false;
	}

	index = 0;
	int index1, index2, index3, index4;
	for (int j = 0; j < (mTerrainHeight - 1); j++)
	{
		for (int i = 0; i < (mTerrainWidth - 1); i++)
		{
			index1 = (mTerrainHeight * j) + i;
			index2 = (mTerrainHeight * j) + (i + 1);
			index3 = (mTerrainHeight * (j + 1)) + i;
			index4 = (mTerrainHeight * (j + 1)) + (i + 1);

			//upper left 
			vertices[index].position = { pHeightMap[index3].x, pHeightMap[index3].y, pHeightMap[index3].z };
			vertices[index].normal = { pHeightMap[index3].norm.x, pHeightMap[index3].norm.y, pHeightMap[index3].norm.z };
			indices[index] = index;
			index++;

			//upper right 
			vertices[index].position = { pHeightMap[index4].x, pHeightMap[index4].y, pHeightMap[index4].z };
			vertices[index].normal = { pHeightMap[index4].norm.x, pHeightMap[index4].norm.y, pHeightMap[index4].norm.z };
			indices[index] = index;
			index++;


			//bottom left 
			vertices[index].position = { pHeightMap[index1].x, pHeightMap[index1].y, pHeightMap[index1].z };
			vertices[index].normal = { pHeightMap[index1].norm.x, pHeightMap[index1].norm.y, pHeightMap[index1].norm.z };
			indices[index] = index;
			index++;

			//bottom left 
			vertices[index].position = { pHeightMap[index1].x, pHeightMap[index1].y, pHeightMap[index1].z };
			vertices[index].normal = { pHeightMap[index1].norm.x, pHeightMap[index1].norm.y, pHeightMap[index1].norm.z };
			indices[index] = index;
			index++;

			//Upper Right
			vertices[index].position = { pHeightMap[index4].x, pHeightMap[index4].y, pHeightMap[index4].z };
			vertices[index].normal = { pHeightMap[index4].norm.x, pHeightMap[index4].norm.y, pHeightMap[index4].norm.z };
			indices[index] = index;
			index++;


			//Bottom Right
			vertices[index].position = { pHeightMap[index2].x, pHeightMap[index2].y, pHeightMap[index2].z };
			vertices[index].normal = { pHeightMap[index2].norm.x, pHeightMap[index2].norm.y, pHeightMap[index2].norm.z };
			indices[index] = index;
			index++;

		}
	}

	// Set up the description of the static vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * mVertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Now create the vertex buffer.
	result = gfxDevice->CreateBuffer(&vertexBufferDesc, &vertexData, &pVertexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Set up the description of the static index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * mIndexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = gfxDevice->CreateBuffer(&indexBufferDesc, &indexData, &pIndexBuffer);
	if (FAILED(result))
	{
		return false;
	}


	delete[] vertices;
	vertices = 0;

	delete[] indices;
	indices = 0;

	return true;
}

void TerrainGen::RenderBuffers(ID3D11DeviceContext* gfxContext)
{
	unsigned int stride;
	unsigned int offset;


	// Set vertex buffer stride and offset.
	stride = sizeof(VertexType);
	offset = 0;

	
	gfxContext->IASetVertexBuffers(0, 1, pVertexBuffer.GetAddressOf(), &stride, &offset);

	gfxContext->IASetIndexBuffer(pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0u);

	gfxContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}
