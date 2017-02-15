#pragma once
// Based off open soure code from
//http://weber.itn.liu.se/~stegu/simplexnoise/SimplexNoise.java

#include <math.h>
#include <cstdint> 
// Inner class to speed upp gradient computations

 

class SimplexNoise
{


public:

	
	SimplexNoise();
	~SimplexNoise();
public:

	static float noise(float xin, float freqancy);

	static float noise(float xin, float yin, float freqancy);

 

private:
	

	static  int32_t fastfloor(float fp);

 



};

