#include "HeightmapGenerator.h"
#include <iostream>
#include <cmath>
#include <EasyBMP.h>
#include <random>
#include "PerlinEngine.h"
HeightmapGenerator::HeightmapGenerator()
{
    PerlinNoiseGenerator = new PerlinEngine();
}

void HeightmapGenerator::GenerateHeightmap(int height, int width)
{
	float scale = 0.5f;
	BMP heightMap;
	heightMap.SetSize(width, height);
    heightMap.SetBitDepth(24);
   /* for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            float noiseValue = PerlinNoise(x * scale, y * scale);
            int pixelValue = static_cast<int>((noiseValue + 1.0f) * 127.5f);
            
            heightMap(x, y).Red = pixelValue;
            heightMap(x, y).Green = pixelValue;
            heightMap(x, y).Blue = pixelValue;
        }
        
    }*/

	heightMap.WriteToFile("../CMPageMastersProject2.0/Data/procHeightMap.bmp");
}


