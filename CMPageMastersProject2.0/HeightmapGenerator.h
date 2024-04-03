#pragma once
class PerlinEngine;
using namespace std;
class HeightmapGenerator
{
public:

	HeightmapGenerator();
	~HeightmapGenerator() = default;

	void GenerateHeightmap(int height, int width);
	
	
private:
	
	PerlinEngine* PerlinNoiseGenerator;
};

