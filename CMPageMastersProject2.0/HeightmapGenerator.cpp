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

bool HeightmapGenerator::GenerateHeightmap(int height, int width, int scale, int octaves)
{ 
    bool result;
    double scaleFactor = scale;
	BMP heightMap;
	heightMap.SetSize(width, height);
   heightMap.SetBitDepth(24);
    double maxValue = 0.0;
    double minValue = std::numeric_limits<double>::max();
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            double nx = x / static_cast<double>(width) - 0.5;
            double ny = y / static_cast<double>(height) - 0.5;
            double noiseValue = PerlinNoiseGenerator->OctaveNoise(nx * scaleFactor, ny * scaleFactor, 0, octaves, 0.5);//use the noise generator to get the noise value 

          
            if (noiseValue > maxValue) maxValue = noiseValue;
            if (noiseValue < minValue) minValue = noiseValue;
            heightMap(x, y).Red = heightMap(x, y).Green = heightMap(x, y).Blue = static_cast<ebmpBYTE>((noiseValue + 1.0) / 2.0 * 255.0);//set as greyscale 
        }
        
    }
   
	result = heightMap.WriteToFile("../CMPageMastersProject2.0/Data/GeneratedHeightMap.bmp");//write to file 
    return result;
}




