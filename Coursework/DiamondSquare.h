#pragma once
/* See the file "LICENSE" for the full license governing this code. */


/*
* Diamond Square noise function
*
* @author   Alan Yeats
* 
* Used to create diamond square noise 
* @See https://en.wikipedia.org/wiki/Diamond-square_algorithm
 */
static class DiamondSquare
{
public:
	DiamondSquare();
	~DiamondSquare();

	/*
	Applies diamond square noise to given highmap.

	@param heightMap		Holds 2D array for position in terrain
	@param width			The width of the terrain
	@param height			The hight of the terrain
	@param range			The range for the dimaond square to caulcate to 
	@param seed				The seed for the given terrain
	*/
	static void DiamondSquareAlgorithm(float* heightMap, float width, float height, float range, int seed);

 
private :


	/*
	Applies diamond part of dimaond square algorithim

	@param heightMap		Holds 2D array for position in terrain
	@param sideLength		The current side length for given generation
	@param halfSide			The current half side length for given generation
	@param range			The range for the dimaond square to caulcate to
	@param fullHight		The starting hight value
	@param seed				The seed for the given terrain

	*/
 	static void Square(float * heightMap, int sideLength, int halfSide,float range, int fullHight, int seed);



	/*
	Applies Square part of dimaond square algorithim

	@param heightMap		Holds 2D array for position in terrain
	@param sideLength		The current side length for given generation
	@param halfSide			The current half side length for given generation
	@param range			The range for the dimaond square to caulcate to
	@param fullHight		The starting hight value
	@param seed				The seed for the given terrain

	*/
	static void Diamond(float * heightMap, int sideLength, int halfSide, float range, int fullHight, int seed);




};

