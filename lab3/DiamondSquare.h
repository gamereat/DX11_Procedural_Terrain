#pragma once
class DiamondSquare
{
public:
	DiamondSquare();
	~DiamondSquare();


	static void DiamondSquareAlgorithm(float* heightMap, float width, float height,float range);

 
private :
 	static void Diamonds(float * heightMap, int sideLength, int halfSide,float range, int fullHight);

	static void Squares(float * heightMap, int sideLength, int halfSide, float range, int fullHight);




};

