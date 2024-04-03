#pragma once
#include <vector>
class PerlinEngine
{
public:
	
	
	PerlinEngine();

	~PerlinEngine() = default;

	double OctaveNoise(double x, double y, double z, int octaves, double persistence);
private:
	double Fade(double t);
	double Lerp(double t, double a, double b);
	double Grad(int hash, double x, double y, double z);
	double Noise(double x, double y, double z);

	std::vector<int> p;
};

