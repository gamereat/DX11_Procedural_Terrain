#include "FractionalBrownianMotion.h"
#include "SimplexNoise.h"
#include <cmath>

float FractionalBrownianMotion::frequency = 1;

float FractionalBrownianMotion::amplitude = 1;
float FractionalBrownianMotion::lacunarity = 2.0f;
float FractionalBrownianMotion::gain = 1.0f / FractionalBrownianMotion::lacunarity;

FractionalBrownianMotion::FractionalBrownianMotion()
{
}


FractionalBrownianMotion::~FractionalBrownianMotion()
{
}

float FractionalBrownianMotion::FBm(float x, float y, int octaves, float pelinFrequancy, bool useAbs,bool useRidged)
{
	float output = 0.f;
	float denom = 0.f;
	float frequency = FractionalBrownianMotion::frequency;
	float amplitude = FractionalBrownianMotion::amplitude;

	for (int i = 0; i < octaves; i++) 
	{

		if (useAbs)
		{
			output += (amplitude * abs( SimplexNoise::noise(x * frequency, y * frequency, pelinFrequancy)));

		}
		else if (useRidged)
		{
			output += 1- (amplitude * abs(SimplexNoise::noise(x * frequency, y * frequency, pelinFrequancy)));

		}
		else
		{
			output += (amplitude * SimplexNoise::noise(x * frequency, y * frequency, pelinFrequancy));
		}
		denom += amplitude;

		frequency *= lacunarity;
		amplitude *= gain;
	}

	return (output / denom);
}
