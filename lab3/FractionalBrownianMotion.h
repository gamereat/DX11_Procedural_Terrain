#pragma once

// Fratical brownian motion noise functions
// Expanded from 
//https://github.com/SRombauts/SimplexNoise
static class FractionalBrownianMotion
{
public:
	FractionalBrownianMotion();
	~FractionalBrownianMotion();


	static inline void set_frequency(float freq) { frequency = freq; };
	static inline void set_amplitude(float amp) { amplitude = amp; };
	static inline void set_lacunarity(float  newLacunarity) { lacunarity = newLacunarity; };
	static inline void set_persistence(float  newPersistence) { persistence = newPersistence; };


	static inline float get_frequency() { return frequency; };
	static inline float get_amplitude() { return amplitude; };
	static inline float get_lacunarity() { return lacunarity; };
	static inline float get_persistence() { return persistence; };


	/*
	Fractical browning noise function
 
	@param x				X position
	@param y				Y posision
	@param octaves			The number of itterations to complete 
	@param pelinFrequancy	the frequancy values used for perlin noise function
	@returns noise values between -1 and 1
	*/
	static float FBm(float x, float y, int octaves,float pelinFrequancy);
private:
	/*

	*/
	static float frequency;  
	static float amplitude;   
	static float lacunarity;  
	static float persistence; 
};
