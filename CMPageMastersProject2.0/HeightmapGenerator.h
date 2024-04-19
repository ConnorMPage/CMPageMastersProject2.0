#pragma once
#include <EasyBMP.h>


class PerlinEngine;
using namespace std;
class HeightmapGenerator
{
public:

	HeightmapGenerator();//constructor 
	~HeightmapGenerator() = default;//destructor 

	bool GenerateHeightmap(int height, int width, int scale, int octaves);//function to create a heightmap 
	
	
private:
	

	PerlinEngine* PerlinNoiseGenerator;//reference to perlin engine 
};

