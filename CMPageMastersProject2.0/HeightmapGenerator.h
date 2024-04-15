#pragma once
#include <EasyBMP.h>


class PerlinEngine;
using namespace std;
class HeightmapGenerator
{
public:

	HeightmapGenerator();
	~HeightmapGenerator() = default;

	bool GenerateHeightmap(int height, int width, int scale);
	
	
private:
	

	PerlinEngine* PerlinNoiseGenerator;
};

