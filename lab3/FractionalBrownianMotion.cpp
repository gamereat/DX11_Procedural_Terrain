#include "FractionalBrownianMotion.h"
#include "PirlinNoise.h"


float FractionalBrownianMotion::frequency = 1;

float FractionalBrownianMotion::amplitude = 1;
float FractionalBrownianMotion::lacunarity = 2.0f;
float FractionalBrownianMotion::persistence = 1.0f / FractionalBrownianMotion::lacunarity;

FractionalBrownianMotion::FractionalBrownianMotion()
{
}


FractionalBrownianMotion::~FractionalBrownianMotion()
{
}

double FractionalBrownianMotion::FBm(float x, float y, int octaves)
{
	float output = 0.f;
	float denom = 0.f;
	float frequency = FractionalBrownianMotion::frequency;
	float amplitude = FractionalBrownianMotion::amplitude;

	for (int i = 0; i < octaves; i++) {
		output += (amplitude * PirlinNoise::noise(x * frequency, y * frequency));
		denom += amplitude; 

		frequency *= lacunarity;
		amplitude *= persistence;
	}

	return (output / denom);
}
