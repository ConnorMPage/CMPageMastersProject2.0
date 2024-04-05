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
    double scaleFactor = 2;
	BMP heightMap;
	heightMap.SetSize(width, height);
    heightMap.SetBitDepth(24);
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
          /*  double nx = x / static_cast<double>(width) - 0.5;
            double ny = y / static_cast<double>(height) - 0.5;
            double noiseValue = PerlinNoiseGenerator->OctaveNoise(nx, ny, 0, 16, 0.7);

            noiseValue = (noiseValue + 1.0) / 2.0;
            noiseValue *= scaleFactor;
            int grayValue = static_cast<int>(noiseValue * 255);
            heightMap(x, y).Red = grayValue;
            heightMap(x, y).Green = grayValue;
            heightMap(x, y).Blue = grayValue;*/
        }
        
    }
    //GaussianBlur(heightMap, 2);
	heightMap.WriteToFile("../CMPageMastersProject2.0/Data/procHeightMap.bmp");
}

void HeightmapGenerator::GaussianBlur(BMP& image, int radius)
{
    //int width = image.TellWidth();
    //int height = image.TellHeight();

    //BMP tempImage;
    //tempImage.SetSize(width, height);

    //for (int x = 0; x < width; ++x) {
    //    for (int y = 0; y < height; ++y) {
    //        double sum = 0.0;
    //        double totalWeight = 0.0;

    //        for (int dx = -radius; dx <= radius; ++dx) {
    //            for (int dy = -radius; dy <= radius; ++dy) {
    //                int nx = std::min(std::max(x + dx, 0), width - 1);
    //                int ny = std::min(std::max(y + dy, 0), height - 1);

    //                double weight = exp(-(dx * dx + dy * dy) / (2.0 * radius * radius));
    //                sum += weight * image(nx, ny).Red; // Assuming grayscale image
    //                totalWeight += weight;
    //            }
    //        }

    //        tempImage(x, y).Red = static_cast<int>(sum / totalWeight);
    //        tempImage(x, y).Green = tempImage(x, y).Red;
    //        tempImage(x, y).Blue = tempImage(x, y).Red;
    //    }
    //}

    //image = tempImage;
}


