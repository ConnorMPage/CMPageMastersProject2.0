#pragma once
#include "EngineDevice.h"
#include <DirectXMath.h>
#include <stdio.h>
#include <string>
class TerrainGen
{
public:
	TerrainGen();
	~TerrainGen() = default;

	bool initialiseTerrain(ID3D11Device* gfxDevice, std::string filename, bool norm);
	void RenderTerrain(ID3D11DeviceContext* gfxContext);

	int getIndexCount();

	void Update(float updateMulti);
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
			float x;
			float y;
			float z;
			
		}normal;
	};
	struct HeightMapType
	{
		float x;
		float y;
		float z;

		struct
		{
			float x;
			float y;
			float z;
		}norm;
	};


	struct VectorType {
		float x; 
		float y;
		float z;
	};
	bool InitialiseHeightMap(std::string filename);
	void NormHeightmap();

	bool CalcNormals();

	bool InitialiseBuffers(ID3D11Device* gfxDevice);
	void RenderBuffers(ID3D11DeviceContext* gfxContext);

	int mTerrainWidth;
	int mTerrainHeight;
	int mVertexCount;
	int mIndexCount;

	
	

	Microsoft::WRL::ComPtr <ID3D11Buffer>pVertexBuffer;
	Microsoft::WRL::ComPtr <ID3D11Buffer>pIndexBuffer;

	HeightMapType* pHeightMap;
};

 