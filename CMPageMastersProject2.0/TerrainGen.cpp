#include "TerrainGen.h"
#include <array>
TerrainGen::TerrainGen()
{
}

bool TerrainGen::initialiseTerrain(ID3D11Device* gfxDevice)
{
	bool initialised;

	initialised = InitialiseHeightMap();
	if (!initialised) return false;
	NormHeightmap();


	initialised = initialiseBuffers(gfxDevice);


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

bool TerrainGen::InitialiseHeightMap()
{
	FILE* filePtr;
	int error;
	//../CMPageMastersProject2.0/Data/testingHMap.bmp
	//.. / CMPageMastersProject2.0 / output.bmp
	error = fopen_s(&filePtr, "../CMPageMastersProject2.0/Data/testingHMap.bmp", "rb");
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

bool TerrainGen::initialiseBuffers(ID3D11Device* gfxDevice)
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
			vertices[index].color = { 1.0f, 1.0f, 1.0f, 1.0f };
			indices[index] = index;
			index++;

			//upper right 
			vertices[index].position = { pHeightMap[index4].x, pHeightMap[index4].y, pHeightMap[index4].z };
			vertices[index].color = { 1.0f, 1.0f, 1.0f, 1.0f };
			indices[index] = index;
			index++;

			//upper right 
			vertices[index].position = { pHeightMap[index4].x, pHeightMap[index4].y, pHeightMap[index4].z };
			vertices[index].color = { 1.0f, 1.0f, 1.0f, 1.0f };
			indices[index] = index;
			index++;

			//bottom left 
			vertices[index].position = { pHeightMap[index1].x, pHeightMap[index1].y, pHeightMap[index1].z };
			vertices[index].color = { 1.0f, 1.0f, 1.0f, 1.0f };
			indices[index] = index;
			index++;

			//bottom left 
			vertices[index].position = { pHeightMap[index1].x, pHeightMap[index1].y, pHeightMap[index1].z };
			vertices[index].color = { 1.0f, 1.0f, 1.0f, 1.0f };
			indices[index] = index;
			index++;

			//upper left 
			vertices[index].position = { pHeightMap[index3].x, pHeightMap[index3].y, pHeightMap[index3].z };
			vertices[index].color = { 1.0f, 1.0f, 1.0f, 1.0f };
			indices[index] = index;
			index++;

			//bottom left 
			vertices[index].position = { pHeightMap[index1].x, pHeightMap[index1].y, pHeightMap[index1].z };
			vertices[index].color = { 1.0f, 1.0f, 1.0f, 1.0f };
			indices[index] = index;
			index++;

			//Upper Right
			vertices[index].position = { pHeightMap[index4].x, pHeightMap[index4].y, pHeightMap[index4].z };
			vertices[index].color = { 1.0f, 1.0f, 1.0f, 1.0f };
			indices[index] = index;
			index++;

			//Upper Right
			vertices[index].position = { pHeightMap[index4].x, pHeightMap[index4].y, pHeightMap[index4].z };
			vertices[index].color = { 1.0f, 1.0f, 1.0f, 1.0f };
			indices[index] = index;
			index++;

			//Bottom Right
			vertices[index].position = { pHeightMap[index2].x, pHeightMap[index2].y, pHeightMap[index2].z };
			vertices[index].color = { 1.0f, 1.0f, 1.0f, 1.0f };
			indices[index] = index;
			index++;

			//Bottom Right
			vertices[index].position = { pHeightMap[index2].x, pHeightMap[index2].y, pHeightMap[index2].z };
			vertices[index].color = { 1.0f, 1.0f, 1.0f, 1.0f };
			indices[index] = index;
			index++;

			//bottom left 
			vertices[index].position = { pHeightMap[index1].x, pHeightMap[index1].y, pHeightMap[index1].z };
			vertices[index].color = { 1.0f, 1.0f, 1.0f, 1.0f };
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

	gfxContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
}
