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

void DiamondSquare::DiamondSquareAlgorithm(float * heightMap, float width, float height, float range)
{
	 
	for (int sideLength = width - 1; sideLength >= 2; sideLength /= 2, range /= 2)
	{
		int halfSide = sideLength / 2;

		Squares(heightMap, sideLength, halfSide, range, height);
		Diamonds(heightMap, sideLength, halfSide, range, height);
	}
 }
void DiamondSquare::Diamonds(float * heightMap, int sideLength, int halfSide, float range, int fullHight)
{


	int index = 0;
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<> dis(-range, range);

	for (int x = 0; x < fullHight - 1; x += halfSide)
	{
		for (int y = (x + halfSide) % sideLength; y < fullHight - 1; y += sideLength)
		{
			double avg = heightMap[((x - halfSide + fullHight - 1) % (fullHight - 1) * fullHight) + y];
			avg += heightMap[((x + halfSide) % (fullHight - 1) * fullHight) + y];
			avg += heightMap[(x * fullHight) + ((y + halfSide) % (fullHight - 1))];
			avg += heightMap[x * fullHight + ((y - halfSide + fullHight - 1) % (fullHight - 1))];

			avg /= 4.0 + dis(gen);
			heightMap[x * fullHight + y] = avg;

			;
			if (x == 0)
			{
				heightMap[(fullHight -1)+ y] = avg;
			}
			if (y == 0) {
				heightMap[x* fullHight + (fullHight - 1)] = avg;
			}
		}
	}
}

 
void DiamondSquare::Squares(float * heightMap, int sideLength, int halfSide, float range, int fullHight)
{


	int index = 0;
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<> dis(-range, range);

	for (int x = 0; x < fullHight - 1; x += sideLength)
	{
		for (int y = 0; y < fullHight - 1; y += sideLength)
		{
			double avg = heightMap[(x*fullHight) + y] + heightMap[(fullHight*(x + sideLength)) + y];
			avg += heightMap[(x*fullHight) + (y + sideLength)];
			avg += heightMap[(fullHight*(x + sideLength)) + (y + sideLength)];
			avg /= 4.0;
			heightMap[(x + halfSide)*fullHight +(y + halfSide)] = avg + dis(gen);
		}
	}
}
