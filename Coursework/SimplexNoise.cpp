#include "SimplexNoise.h"
#include <cstdint> 

/*
* FOR 1D and 2D implmenation 
*
* This C++ implementation is based on the speed - improved Java version 2012 - 03 - 09
* by Stefan Gustavson(original Java source code in the public domain).
* http://webstaff.itn.liu.se/~stegu/simplexnoise/SimplexNoise.java:
*-Based on example code by Stefan Gustavson(stegu@itn.liu.se).
* -Optimisations by Peter Eastman(peastman@drizzle.stanford.edu).
* -Better rank ordering method by Stefan Gustavson in 2012.
*
* This implementation is "Simplex Noise" as presented by
* Ken Perlin at a relatively obscure and not often cited course
* session "Real-Time Shading" at Siggraph 2001 (before real
* time shading actually took on), under the title "hardware noise".
* The 3D function is numerically equivalent to his Java reference
* code available in the PDF course notes, although I re - implemented
* it from scratch to get more readable code.The 1D, 2D and 4D cases
* were implemented from scratch by me from Ken Perlin's text.
*
* Distributed under the MIT License(MIT) (See accompanying file LICENSE.txt
	* or copy at http ://opensource.org/licenses/MIT)

	
	
3D implmentation 

http://mrl.nyu.edu/~perlin/noise/ 
// JAVA REFERENCE IMPLEMENTATION OF IMPROVED NOISE - COPYRIGHT 2002 KEN PERLIN.

	
	*/







static double fade(double t) { return t * t * t * (t * (t * 6 - 15) + 10); }
static double lerp(double t, double a, double b) { return a + t * (b - a); }
static double grad(int hash, double x, double y, double z) {
	int h = hash & 15;                      // CONVERT LO 4 BITS OF HASH CODE
	double u = h<8 ? x : y,                 // INTO 12 GRADIENT DIRECTIONS.
		v = h<4 ? y : h == 12 || h == 14 ? x : z;
	return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
}




/**
* Permutation table. This is just a random jumble of all numbers 0-255.
*
* This produce a repeatable pattern of 256, but Ken Perlin stated
* that it is not a problem for graphic texture as the noise features disappear
* at a distance far enough to be able to see a repeatable pattern of 256.
*
* This needs to be exactly the same for all instances on all platforms,
* so it's easiest to just keep it as static explicit data.
* This also removes the need for any initialisation of this class.
*
* Note that making this an uint32_t[] instead of a uint8_t[] might make the
* code run faster on platforms with a high penalty for unaligned single
* byte addressing. Intel x86 is generally single-byte-friendly, but
* some other CPUs are faster with 4-aligned reads.
* However, a char[] is smaller, which avoids cache trashing, and that
* is probably the most important aspect on most architectures.
* This array is accessed a *lot* by the noise functions.
* A vector-valued noise over 3D accesses it 96 times, and a
* float-valued 4D noise 64 times. We want this to fit in the cache!
*/
static const uint8_t perm[256] = {
	151, 160, 137, 91, 90, 15,
	131, 13, 201, 95, 96, 53, 194, 233, 7, 225, 140, 36, 103, 30, 69, 142, 8, 99, 37, 240, 21, 10, 23,
	190, 6, 148, 247, 120, 234, 75, 0, 26, 197, 62, 94, 252, 219, 203, 117, 35, 11, 32, 57, 177, 33,
	88, 237, 149, 56, 87, 174, 20, 125, 136, 171, 168, 68, 175, 74, 165, 71, 134, 139, 48, 27, 166,
	77, 146, 158, 231, 83, 111, 229, 122, 60, 211, 133, 230, 220, 105, 92, 41, 55, 46, 245, 40, 244,
	102, 143, 54, 65, 25, 63, 161, 1, 216, 80, 73, 209, 76, 132, 187, 208, 89, 18, 169, 200, 196,
	135, 130, 116, 188, 159, 86, 164, 100, 109, 198, 173, 186, 3, 64, 52, 217, 226, 250, 124, 123,
	5, 202, 38, 147, 118, 126, 255, 82, 85, 212, 207, 206, 59, 227, 47, 16, 58, 17, 182, 189, 28, 42,
	223, 183, 170, 213, 119, 248, 152, 2, 44, 154, 163, 70, 221, 153, 101, 155, 167, 43, 172, 9,
	129, 22, 39, 253, 19, 98, 108, 110, 79, 113, 224, 232, 178, 185, 112, 104, 218, 246, 97, 228,
	251, 34, 242, 193, 238, 210, 144, 12, 191, 179, 162, 241, 81, 51, 145, 235, 249, 14, 239, 107,
	49, 192, 214, 31, 181, 199, 106, 157, 184, 84, 204, 176, 115, 121, 50, 45, 127, 4, 150, 254,
	138, 236, 205, 93, 222, 114, 67, 29, 24, 72, 243, 141, 128, 195, 78, 66, 215, 61, 156, 180
};

/**
* Helper function to hash an integer using the above permutation table
*
*  This inline function costs around 1ns, and is called N+1 times for a noise of N dimension.
*
*  Using a real hash function would be better to improve the "repeatability of 256" of the above permutation table,
* but fast integer Hash functions uses more time and have bad random properties.
*
* @param[in] i Integer value to hash
*
* @return 8-bits hashed value
*/
static inline uint8_t hash(int32_t i) {
	return perm[static_cast<uint8_t>(i)];
}
/**
* Helper function to compute gradients-dot-residual vectors (1D)
*
* @note that these generate gradients of more than unit length. To make
* a close match with the value range of classic Perlin noise, the final
* noise values need to be rescaled to fit nicely within [-1,1].
* (The simplex noise functions as such also have different scaling.)
* Note also that these noise functions are the most practical and useful
* signed version of Perlin noise.
*
* @param[in] hash  hash value
* @param[in] x     distance to the corner
*
* @return gradient value
*/
static float grad(int32_t hash, float x) {
	int32_t h = hash & 0x0F;        // Convert low 4 bits of hash code
	float grad = 1.0f + (h & 7);    // Gradient value 1.0, 2.0, ..., 8.0
	if ((h & 8) != 0) grad = -grad; // Set a random sign for the gradient
									//  float grad = gradients1D[h];    // NOTE : Test of Gradient look-up table instead of the above
	return (grad * x);              // Multiply the gradient with the distance
} 
/**
* Helper functions to compute gradients-dot-residual vectors (2D)
*
* @param[in] hash  hash value
* @param[in] x     x coord of the distance to the corner
* @param[in] y     y coord of the distance to the corner
*
* @return gradient value
*/
static float grad(int32_t hash, float x, float y) {
	int32_t h = hash & 0x3F;  // Convert low 3 bits of hash code
	float u = h < 4 ? x : y;  // into 8 simple gradient directions,
	float v = h < 4 ? y : x;  // and compute the dot product with (x,y).
	return ((h & 1) ? -u : u) + ((h & 2) ? -2.0f*v : 2.0f*v);
}
/**
* 1D Perlin simplex noise
*
*  Takes around 74ns on an AMD APU.
*
* @param[in] x float coordinate
*
* @return Noise value in the range[-1; 1], value of 0 on all integer coordinates.
*/
float SimplexNoise::noise(float x, float freqancy)
{
	x *= freqancy;

	float n0, n1;   // Noise contributions from the two "corners"

					// No need to skew the input space in 1D

					// Corners coordinates (nearest integer values):
	int32_t i0 = fastfloor(x);
	int32_t i1 = i0 + 1;
	// Distances to corners (between 0 and 1):
	float x0 = x - i0;
	float x1 = x0 - 1.0f;

	// Calculate the contribution from the first corner
	float t0 = 1.0f - x0*x0;
	//  if(t0 < 0.0f) t0 = 0.0f; // not possible
	t0 *= t0;
	n0 = t0 * t0 * grad(hash(i0), x0);

	// Calculate the contribution from the second corner
	float t1 = 1.0f - x1*x1;
	//  if(t1 < 0.0f) t1 = 0.0f; // not possible
	t1 *= t1;
	n1 = t1 * t1 * grad(hash(i1), x1);

	// The maximum value of this noise is 8*(3/4)^4 = 2.53125
	// A factor of 0.395 scales to fit exactly within [-1,1]
	return 0.395f * (n0 + n1);
}


 
SimplexNoise::SimplexNoise()
{
 
}


SimplexNoise::~SimplexNoise()
{
}





float SimplexNoise::noise(float x, float y, float freqancy)
{
	x *= freqancy;
	y *= freqancy;
	float n0, n1, n2;   // Noise contributions from the three corners

						// Skewing/Unskewing factors for 2D
	const float F2 = 0.366025403f;  // F2 = (sqrt(3) - 1) / 2
	const float G2 = 0.211324865f;  // G2 = (3 - sqrt(3)) / 6   = F2 / (1 + 2 * K)

									// Skew the input space to determine which simplex cell we're in
	float s = (x + y) * F2;  // Hairy factor for 2D
	float xs = x + s;
	float ys = y + s;
	int32_t i = fastfloor(xs);
	int32_t j = fastfloor(ys);

	// Unskew the cell origin back to (x,y) space
	float t = static_cast<float>(i + j) * G2;
	float X0 = i - t;
	float Y0 = j - t;
	float x0 = x - X0;  // The x,y distances from the cell origin
	float y0 = y - Y0;

	// For the 2D case, the simplex shape is an equilateral triangle.
	// Determine which simplex we are in.
	int32_t i1, j1;  // Offsets for second (middle) corner of simplex in (i,j) coords
	if (x0 > y0) {   // lower triangle, XY order: (0,0)->(1,0)->(1,1)
		i1 = 1;
		j1 = 0;
	}
	else {   // upper triangle, YX order: (0,0)->(0,1)->(1,1)
		i1 = 0;
		j1 = 1;
	}

	// A step of (1,0) in (i,j) means a step of (1-c,-c) in (x,y), and
	// a step of (0,1) in (i,j) means a step of (-c,1-c) in (x,y), where
	// c = (3-sqrt(3))/6

	float x1 = x0 - i1 + G2;            // Offsets for middle corner in (x,y) unskewed coords
	float y1 = y0 - j1 + G2;
	float x2 = x0 - 1.0f + 2.0f * G2;   // Offsets for last corner in (x,y) unskewed coords
	float y2 = y0 - 1.0f + 2.0f * G2;

	// Calculate the contribution from the first corner
	float t0 = 0.5f - x0*x0 - y0*y0;
	if (t0 < 0.0f) {
		n0 = 0.0f;
	}
	else {
		t0 *= t0;
		n0 = t0 * t0 * grad(hash(i + hash(j)), x0, y0);
	}

	// Calculate the contribution from the second corner
	float t1 = 0.5f - x1*x1 - y1*y1;
	if (t1 < 0.0f) {
		n1 = 0.0f;
	}
	else {
		t1 *= t1;
		n1 = t1 * t1 * grad(hash(i + i1 + hash(j + j1)), x1, y1);
	}

	// Calculate the contribution from the third corner
	float t2 = 0.5f - x2*x2 - y2*y2;
	if (t2 < 0.0f) {
		n2 = 0.0f;
	}
	else {
		t2 *= t2;
		n2 = t2 * t2 * grad(hash(i + 1 + hash(j + 1)), x2, y2);
	}

	// Add contributions from each corner to get the final noise value.
	// The result is scaled to return values in the interval [-1,1].
	return 45.23065f * (n0 + n1 + n2);
}



// http://mrl.nyu.edu/~perlin/noise/
float SimplexNoise::noise(float xin, float yin, float zin, float freqancy)
{
	xin *= freqancy;
	yin *= freqancy;
	zin *= freqancy;

	static int p[512];
	static int permutation[] = { 151,160,137,91,90,15,
		131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,8,99,37,240,21,10,23,
		190, 6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,35,11,32,57,177,33,
		88,237,149,56,87,174,20,125,136,171,168, 68,175,74,165,71,134,139,48,27,166,
		77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,55,46,245,40,244,
		102,143,54, 65,25,63,161, 1,216,80,73,209,76,132,187,208, 89,18,169,200,196,
		135,130,116,188,159,86,164,100,109,198,173,186, 3,64,52,217,226,250,124,123,
		5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,189,28,42,
		223,183,170,213,119,248,152, 2,44,154,163, 70,221,153,101,155,167, 43,172,9,
		129,22,39,253, 19,98,108,110,79,113,224,232,178,185, 112,104,218,246,97,228,
		251,34,242,193,238,210,144,12,191,179,162,241, 81,51,145,235,249,14,239,107,
		49,192,214, 31,181,199,106,157,184, 84,204,176,115,121,50,45,127, 4,150,254,
		138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180
	};


	for (int i = 0; i < 256; i++)
	{
		p[256 + i] = p[i] = permutation[i];
	}

	int X = (int)fastfloor(xin) & 255,                  // FIND UNIT CUBE THAT
		Y = (int)fastfloor(yin) & 255,                  // CONTAINS POINT.
		Z = (int)fastfloor(zin) & 255;

	xin -= fastfloor(xin);                                // FIND RELATIVE X,Y,Z
	yin -= fastfloor(yin);                                // OF POINT IN CUBE.
	zin -= fastfloor(zin);
	double u = fade(xin),                                // COMPUTE FADE CURVES
		v = fade(yin),                                // FOR EACH OF X,Y,Z.
		w = fade(zin);
	int A = p[X] + Y, AA = p[A] + Z, AB = p[A + 1] + Z,      // HASH COORDINATES OF
		B = p[X + 1] + Y, BA = p[B] + Z, BB = p[B + 1] + Z;      // THE 8 CUBE CORNERS,

	return lerp(w, lerp(v, lerp(u, grad(p[AA], xin, yin, zin),  // AND ADD
		grad(p[BA], xin - 1, yin, zin)), // BLENDED
		lerp(u, grad(p[AB], xin, yin - 1, zin),  // RESULTS
			grad(p[BB], xin - 1, yin - 1, zin))),// FROM  8
		lerp(v, lerp(u, grad(p[AA + 1], xin, yin, zin - 1),  // CORNERS
			grad(p[BA + 1], xin - 1, yin, zin - 1)), // OF CUBE
			lerp(u, grad(p[AB + 1], xin, yin - 1, zin - 1),
				grad(p[BB + 1], xin - 1, yin - 1, zin - 1))));
}

int32_t SimplexNoise::fastfloor(float fp)
{
	int32_t i = (int32_t)fp;
	return (fp < i) ? (i - 1) : (i);
}
