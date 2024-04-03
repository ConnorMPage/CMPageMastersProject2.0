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
    double scaleFactor = 10.0;
	BMP heightMap;
	heightMap.SetSize(width, height);
    heightMap.SetBitDepth(24);
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            double nx = x / static_cast<double>(width) - 0.5;
            double ny = y / static_cast<double>(height) - 0.5;
            double noiseValue = PerlinNoiseGenerator->OctaveNoise(nx, ny, 0, 16, 0.5);

            noiseValue = (noiseValue + 1.0) / 2.0;
            noiseValue *= scaleFactor;
            int grayValue = static_cast<int>(noiseValue * 255);
            heightMap(x, y).Red = grayValue;
            heightMap(x, y).Green = grayValue;
            heightMap(x, y).Blue = grayValue;
        }
        
    }
    
	heightMap.WriteToFile("../CMPageMastersProject2.0/Data/procHeightMap.bmp");
}

void HeightmapGenerator::GaussianBlur(BMP& image, int radius)
{

}


