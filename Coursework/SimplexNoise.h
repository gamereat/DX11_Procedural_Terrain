#pragma once
/* See the file "LICENSE" for the full license governing this code. */


// Based off 1d and 2d open soure code from 
//http://weber.itn.liu.se/~stegu/simplexnoise/SimplexNoise.java

//3d
//3D implmentation 
//http://mrl.nyu.edu/~perlin/noise/ 

#include <math.h>
#include <cstdint> 
// Inner class to speed upp gradient computations

 
/*
Simplex Noise Class

@Author Alan Yeats


*/
static class SimplexNoise
{


public:

	
	SimplexNoise();
	~SimplexNoise();
public:

	/*
	1D Noise Function

	@param xin			X Posision
	@param freqancy		Freqancy of noise

	@returns			Value between -1 and 1
	*/
	static float noise(float xin, float freqancy);

	/*
	2D Noise Function

	@param xin			X Posision
	@param yin			Y Posision
	@param freqancy		Freqancy of noise

	@returns			Value between -1 and 1
	*/
	static float noise(float xin, float yin, float freqancy);

	/*
	3D Noise Function

	@param xin			X Posision
	@param yin			Y Posision
	@param zin			Z Posision
	@param freqancy		Freqancy of noise

	@returns			Value between -1 and 1
	*/
	static float noise(float xin, float yin, float zin, float freqancy);

private:
 
	static  int32_t fastfloor(float fp);

 



};

