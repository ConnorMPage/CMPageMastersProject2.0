#pragma once
#include "EngineDevice.h"
#include <DirectXMath.h>
#include <stdio.h>
#include <string>
class TerrainGen
{
public:
	TerrainGen();//constructor 
	~TerrainGen() = default;//destructor 

	bool InitialiseTerrain(ID3D11Device* gfxDevice, std::string filename, bool norm);
	void RenderTerrain(ID3D11DeviceContext* gfxContext);

	int getIndexCount();

	void Update(float updateMulti);
private:
	struct VertexType//vertex struct
	{
	public:
		struct 
		{
			float x;
			float y;
			float z;
		 } position;//pos 
		struct  
		{
			float x;
			float y;
			float z;
			
		}normal;//normal 
	};
	struct HeightMapType//used struct instead of DXMath 
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
	void NormHeightmap();//normalises the heightmap 


	bool CalcNormals();//calculates the mesh normals 

	bool InitialiseBuffers(ID3D11Device* gfxDevice);
	void RenderBuffers(ID3D11DeviceContext* gfxContext);


	//ints
	int mTerrainWidth;
	int mTerrainHeight;
	int mVertexCount;
	int mIndexCount;

	
	
	//com ptrs to reduce the need for release 
	Microsoft::WRL::ComPtr <ID3D11Buffer>pVertexBuffer;
	Microsoft::WRL::ComPtr <ID3D11Buffer>pIndexBuffer;

	HeightMapType* pHeightMap;
};

 