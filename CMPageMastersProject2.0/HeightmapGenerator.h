#pragma once
#include <EasyBMP.h>


class PerlinEngine;
using namespace std;
class HeightmapGenerator
{
public:

	HeightmapGenerator();
	~HeightmapGenerator() = default;

	void GenerateHeightmap(int height, int width);
	
	
private:
	void GaussianBlur(BMP& image, int radius);

	PerlinEngine* PerlinNoiseGenerator;
};

