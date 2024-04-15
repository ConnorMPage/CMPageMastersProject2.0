#include "PerlinEngine.h"
#include <math.h>
#include <iostream>
#include <cmath>
#include <vector>
#include <random>
PerlinEngine::PerlinEngine()
{

    

    
    for (int i = 0; i < 512; ++i)
        p[i] = permutation[i%256];

    

}

double PerlinEngine::OctaveNoise(double x, double y, double z, int octaves, double persistence)
{
    double total = 0.0;
    double frequency = 1.0;
    double amplitude = 1;
    double maxValue = 0.0;

    for (int i = 0; i < octaves; ++i) {
        total += Noise(x * frequency, y * frequency, z * frequency) * amplitude;
        maxValue += amplitude;
        amplitude *= persistence;
        frequency *= 2;
    }

    return total / maxValue;
}

double PerlinEngine::Fade(double t)
{
    return t * t * t * (t * (t * 6 - 15) + 10);
}

double PerlinEngine::Lerp(double t, double a, double b)
{
    return  a + t * (b - a);
}

double PerlinEngine::Grad(int hash, double x, double y, double z)
{
    /*int h = hash & 15;
    double u = h < 8 ? x : y;
    double v = h < 4 ? y : h == 12 || h == 14 ? x : z;
    return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);*/
    switch (hash & 0xF)
    {
    case 0x0: return  x + y;
    case 0x1: return -x + y;
    case 0x2: return  x - y;
    case 0x3: return -x - y;
    case 0x4: return  x + z;
    case 0x5: return -x + z;
    case 0x6: return  x - z;
    case 0x7: return -x - z;
    case 0x8: return  y + z;
    case 0x9: return -y + z;
    case 0xA: return  y - z;
    case 0xB: return -y - z;
    case 0xC: return  y + x;
    case 0xD: return -y + z;
    case 0xE: return  y - x;
    case 0xF: return -y - z;
    default: return 0; // never happens
    }
}

double PerlinEngine::Noise(double x, double y, double z)
{
    int X = (int)floor(x) & 255;
    int Y = (int)floor(y) & 255;
    int Z = (int)floor(z) & 255;

    x -= floor(x);
    y -= floor(y);
    z -= floor(z);

    double u = Fade(x);
    double v = Fade(y);
    double w = Fade(z);

 

    int A = p[X] + Y;
    int AA = p[A] + Z;
    int AB = p[A + 1] + Z;
    int B = p[X + 1] + Y;
    int BA = p[B] + Z;
    int BB = p[B + 1] + Z;

    return Lerp(w, Lerp(v, Lerp(u, Grad(p[AA], x, y, z),
        Grad(p[BA], x - 1, y, z)),
        Lerp(u, Grad(p[AB], x, y - 1, z),
            Grad(p[BB], x - 1, y - 1, z))),
        Lerp(v, Lerp(u, Grad(p[AA + 1], x, y, z - 1),
            Grad(p[BA + 1], x - 1, y, z - 1)),
            Lerp(u, Grad(p[AB + 1], x, y - 1, z - 1),
                Grad(p[BB + 1], x - 1, y - 1, z - 1))));
}




