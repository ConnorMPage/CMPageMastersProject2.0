#include "TerrainGen.h"
#include <array>
TerrainGen::TerrainGen()
{
}

bool TerrainGen::initialiseTerrain(ID3D11Device* gfxDevice, int height, int width)
{
	bool initialised;

	mTerrainHeight = height;
	mTerrainWidth = width;

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

bool TerrainGen::initialiseBuffers(ID3D11Device* gfxDevice)
{
	
	unsigned long* indices;
	int index;
	float positionX, positionZ;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;


	int mVertexCount = (mTerrainWidth - 1) * (mTerrainHeight - 1) * 8;
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

	for (int j = 0; j < (mTerrainHeight - 1); j++)
	{
		for (int i = 0; i < (mTerrainWidth - 1); i++)
		{
			//line 1
			//upper left 
			positionX = (float)i;
			positionZ = (float)(j + 1);

			vertices[index].position = { positionX, 0.0f, positionZ};
			vertices[index].color = {1.0f, 1.0f, 1.0f, 1.0f};
			indices[index] = index;
			index++;

			//upper right
			positionX = (float)(i + 1);
			positionZ = (float)(j + 1);
			vertices[index].position = { positionX, 0.0f, positionZ };
			vertices[index].color = { 1.0f, 1.0f, 1.0f, 1.0f };
			indices[index] = index;
			index++;

			// LINE 2
			// Upper right.
			positionX = (float)(i + 1);
			positionZ = (float)(j + 1);
			vertices[index].position = { positionX, 0.0f, positionZ};
			vertices[index].color = { 1.0f, 1.0f, 1.0f, 1.0f };
			indices[index] = index;
			index++;

			// Bottom right.
			positionX = (float)(i + 1);
			positionZ = (float)j;
			vertices[index].position = { positionX, 0.0f, positionZ};
			vertices[index].color = { 1.0f, 1.0f, 1.0f, 1.0f };
			indices[index] = index;
			index++;

			// LINE 3
			// Bottom right.
			positionX = (float)(i + 1);
			positionZ = (float)j;
			vertices[index].position = { positionX, 0.0f, positionZ };
			vertices[index].color = { 1.0f, 1.0f, 1.0f, 1.0f };
			indices[index] = index;
			index++;

			// Bottom left.
			positionX = (float)i;
			positionZ = (float)j;
			vertices[index].position = { positionX, 0.0f, positionZ};
			vertices[index].color = { 1.0f, 1.0f, 1.0f, 1.0f };
			indices[index] = index;
			index++;

			// LINE 4
			// Bottom left.
			positionX = (float)i;
			positionZ = (float)j;
			vertices[index].position = { positionX, 0.0f, positionZ};
			vertices[index].color = { 1.0f, 1.0f, 1.0f, 1.0f };
			indices[index] = index;
			index++;

			// Upper left.
			positionX = (float)i;
			positionZ = (float)(j + 1);
			vertices[index].position = { positionX, 0.0f, positionZ};
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
