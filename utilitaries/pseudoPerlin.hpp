#pragma once
#include <algorithm>
#include <SFML/Graphics.hpp>


inline float Lerp(int posX, int posY, float percent, float* table) {
	// percent = (point - x) / (y-x)
	return (1.0f - percent) * table[posX] + percent * table[posY];
}


void perlin1D(int size, int octaves, float bias, float* output);
void perlin2D(int sizeX, int sizeY, int octaves, float bias, float** output);

void PerlinNoise1D(int nCount, float* fSeed, int nOctaves, float fBias, float* fOutput);
void PerlinNoise2D(int nWidth, int nHeight, float* fSeed, int nOctaves, float fBias, float* fOutput);

void regenerateSprite(int sizeX, int sizeY, int octaves, float bias, sf::Uint8* pixelGrid);

void generateHeightMap(int sizeX, int sizeY, int octaves, float bias, float* heightMap);