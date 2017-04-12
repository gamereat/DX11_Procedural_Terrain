#pragma once
// Based off open soure code from
//http://weber.itn.liu.se/~stegu/simplexnoise/SimplexNoise.java

#include <math.h>
#include <cstdint> 
// Inner class to speed upp gradient computations

 
/*
Simplex Noise Class

@Author Alan Yeats


*/
class SimplexNoise
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

 

private:
 
	static  int32_t fastfloor(float fp);

 



};

