#pragma once
#include "EngineDevice.h"
#include <DirectXMath.h>


class TerrainGen
{
public:
	TerrainGen();
	~TerrainGen() = default;

	bool initialiseTerrain(ID3D11Device* gfxDevice,int height,int width);
	void RenderTerrain(ID3D11DeviceContext* gfxContext);

	int getIndexCount();
private:
	struct VertexType
	{
	public:
		struct 
		{
			float x;
			float y;
			float z;
		 } position;
		struct  
		{
			float r;
			float g;
			float b;
			float a;
		}color;
	};


	bool initialiseBuffers(ID3D11Device* gfxDevice);
	void RenderBuffers(ID3D11DeviceContext* gfxContext);

	int mTerrainWidth;
	int mTerrainHeight;
	int mVertexCount;
	int mIndexCount;

	
	

	Microsoft::WRL::ComPtr <ID3D11Buffer>pVertexBuffer;
	Microsoft::WRL::ComPtr <ID3D11Buffer>pIndexBuffer;
};

 