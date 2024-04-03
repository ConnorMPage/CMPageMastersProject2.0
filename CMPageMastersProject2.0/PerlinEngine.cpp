#include "PerlinEngine.h"
#include <math.h>
#include <iostream>
#include <cmath>
#include <vector>
#include <random>
PerlinEngine::PerlinEngine()
{
    p.resize(256);

    
    for (int i = 0; i < 256; ++i)
        p[i] = i;

    
    std::random_device rd;
    std::mt19937 rng(rd());
    std::shuffle(p.begin(), p.end(), rng);

   
    p.insert(p.end(), p.begin(), p.end());

}

double PerlinEngine::OctaveNoise(double x, double y, double z, int octaves, double persistence)
{
    double total = 0;
    double frequency = 1;
    double amplitude = 1;
    double maxValue = 0;

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
    int h = hash & 15;
    double u = h < 8 ? x : y;
    double v = h < 4 ? y : h == 12 || h == 14 ? x : z;
    return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
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

    int A = p[X] + Y, AA = p[A] + Z, AB = p[A + 1] + Z;
    int B = p[X + 1] + Y, BA = p[B] + Z, BB = p[B + 1] + Z;

    return Lerp(w, Lerp(v, Lerp(u, Grad(p[AA], x, y, z),
        Grad(p[BA], x - 1, y, z)),
        Lerp(u, Grad(p[AB], x, y - 1, z),
            Grad(p[BB], x - 1, y - 1, z))),
        Lerp(v, Lerp(u, Grad(p[AA + 1], x, y, z - 1),
            Grad(p[BA + 1], x - 1, y, z - 1)),
            Lerp(u, Grad(p[AB + 1], x, y - 1, z - 1),
                Grad(p[BB + 1], x - 1, y - 1, z - 1))));
}


