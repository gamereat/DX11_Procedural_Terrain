#include "DiamondSquare.h"
#include <random>
#include <math.h>
#include <algorithm>
 
DiamondSquare::DiamondSquare()
{
}


DiamondSquare::~DiamondSquare()
{
}

void DiamondSquare::DiamondSquareAlgorithm(float * heightMap, float width, float height, float range, int seed) 
{
	for (int sideLength = width - 1; sideLength >= 2; sideLength /= 2)
	{
		int halfSide = sideLength / 2;
		// Complete diamond part then square 
		Diamond(heightMap, sideLength, halfSide, range, height, seed);
		Square(heightMap, sideLength, halfSide, range, height, seed);
		
		// half the range for next iteration
		range /= 2;
	}
 
}


void DiamondSquare::Square(float * heightMap, int sideLength, int halfSide, float range, int fullHight, int seed)
{


	int index = 0;
	// seed with current seed 
 	std::mt19937 gen(seed);
	// gen from range
	std::uniform_real_distribution<> dis(-range, range);

	for (int x = 0; x < fullHight - 1; x += halfSide)
	{
		for (int y = (x + halfSide) % sideLength; y < fullHight - 1; y += sideLength)
		{
			// caculate the total value of the diamond 
			float  avg = heightMap[((x - halfSide + fullHight - 1) % (fullHight - 1) * fullHight) + y];
			avg += heightMap[((x + halfSide) % (fullHight - 1) * fullHight) + y];
			avg += heightMap[(x * fullHight) + ((y + halfSide) % (fullHight - 1))];
			avg += heightMap[x * fullHight + ((y - halfSide + fullHight - 1) % (fullHight - 1))];

			// work out adverage plus random value 
			avg /= 4.0 + dis(gen);

			// set the new vaue 
			heightMap[x * fullHight + y] = avg;
			

			if (x == 0)
			{
				heightMap[(fullHight - 1) + y] = avg;
			}
			if (y == 0) 
			{
				heightMap[x * fullHight + (fullHight - 1)] = avg;
			}
		}
	}
}

 
void DiamondSquare::Diamond(float * heightMap, int sideLength, int halfSide, float range, int fullHight, int seed)
{


	int index = 0;
	std::random_device rd;
	std::mt19937 gen(seed);
	std::uniform_real_distribution<> dis(-range, range);

	for (int x = 0; x < fullHight - 1; x += sideLength)
	{
		for (int y = 0; y < fullHight - 1; y += sideLength)
		{
			// caculate the adverage of square
			double avg = heightMap[(x*fullHight) + y];
			avg += heightMap[(fullHight*(x + sideLength)) + y];
			avg += heightMap[(x*fullHight) + (y + sideLength)];
			avg += heightMap[(fullHight*(x + sideLength)) + (y + sideLength)];
			avg /= 4.0;
			// set to terrain but a random value
			heightMap[(x + halfSide)*fullHight +(y + halfSide)] = avg + dis(gen);
		}
	}
}
