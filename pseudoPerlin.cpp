#include "pseudoPerlin.hpp"


void perlin1D(int size, int octaves, float bias, float* output) {

	float* randomMap = new float[size];
	for (int i = 0; i < size; i++) randomMap[i] = (float)rand() / (float)RAND_MAX;

	for (int pos = 0; pos < size; pos++) {
		
		float noise = 0.0f;
		float scaleAcc = 0.0f;
		float scale = 1.0f;

		for (int octave = 0; octave < octaves; octave++) {
			int pitch = size >> octave;
			int pointA = (pos / pitch) * pitch;		// Integer division
			int pointB = (pointA + pitch) % size;

			float blend = (float)(pos - pointA) / pitch;
			float sample = Lerp(pointA, pointB, blend, randomMap);

			scaleAcc += scale;
			noise += sample * scale;
			scale = scale / bias;
		}

		output[pos] = noise / scaleAcc;
	}

}


void perlin2D(int sizeX, int sizeY, int octaves, float bias, float** output) {

	float* randomMap = new float[sizeY * sizeX];

	for (int y = 0; y < sizeY; y++) 
		for (int x = 0; x < sizeX; x++) randomMap[y * sizeY + x] = (float)rand() / (float)RAND_MAX;

	for (int posY = 0; posY < sizeY; posY++) {
		for (int posX = 0; posX < sizeX; posX++) {

			float noise = 0.0f;
			float scaleAcc = 0.0f;
			float scale = 1.0f;

			for (int octave = 0; octave < octaves; octave++) {

				int pitch = sizeX >> octave;

				int pointXA = (posX / pitch) * pitch;
				int pointYA = (posY / pitch) * pitch;

				int pointXB = (pointXA + pitch) % sizeX;
				int pointYB = (pointYA + pitch) % sizeY;

				float blendX = (float)(posX - pointXA) / (float)pitch;
				float blendY = (float)(posY - pointYA) / (float)pitch;

				float sampleX = Lerp(pointYA * sizeY + pointXA, pointYA * sizeY + pointXB, blendX, randomMap);
				float sampleY = Lerp(pointYB * sizeY + pointXA, pointYB * sizeY + pointXB, blendY, randomMap);

				scaleAcc += scale;
				noise += (blendY * (sampleY - sampleX) + sampleX) * scale;
				scale = scale / bias;
			}

			output[posY][posX] = noise / scaleAcc;
		}
	}

}



void PerlinNoise1D(int nCount, float* fSeed, int nOctaves, float fBias, float* fOutput)
{
	// Used 1D Perlin Noise
	for (int x = 0; x < nCount; x++)
	{
		float fNoise = 0.0f;
		float fScaleAcc = 0.0f;
		float fScale = 1.0f;

		for (int o = 0; o < nOctaves; o++)
		{
			int nPitch = nCount >> o;
			int nSample1 = (x / nPitch) * nPitch;
			int nSample2 = (nSample1 + nPitch) % nCount;

			float fBlend = (float)(x - nSample1) / (float)nPitch;

			float fSample = (1.0f - fBlend) * fSeed[nSample1] + fBlend * fSeed[nSample2];

			fScaleAcc += fScale;
			fNoise += fSample * fScale;
			fScale = fScale / fBias;
		}

		// Scale to seed range
		fOutput[x] = fNoise / fScaleAcc;
	}
}

void PerlinNoise2D(int nWidth, int nHeight, float* fSeed, int nOctaves, float fBias, float* fOutput)
{
	// Used 1D Perlin Noise
	for (int y = 0; y < nHeight; y++)
		for (int x = 0; x < nWidth; x++)
		{
			float fNoise = 0.0f;
			float fScaleAcc = 0.0f;
			float fScale = 1.0f;

			for (int o = 0; o < nOctaves; o++)
			{
				int nPitch = nWidth >> o;
				int nSampleX1 = (x / nPitch) * nPitch;
				int nSampleY1 = (y / nPitch) * nPitch;

				int nSampleX2 = (nSampleX1 + nPitch) % nWidth;
				int nSampleY2 = (nSampleY1 + nPitch) % nWidth;

				float fBlendX = (float)(x - nSampleX1) / (float)nPitch;
				float fBlendY = (float)(y - nSampleY1) / (float)nPitch;

				float fSampleT = (1.0f - fBlendX) * fSeed[nSampleY1 * nHeight + nSampleX1] + fBlendX * fSeed[nSampleY1 * nHeight + nSampleX2];
				float fSampleB = (1.0f - fBlendX) * fSeed[nSampleY2 * nHeight + nSampleX1] + fBlendX * fSeed[nSampleY2 * nHeight + nSampleX2];

				fScaleAcc += fScale;
				fNoise += (fBlendY * (fSampleB - fSampleT) + fSampleT) * fScale;
				fScale = fScale / fBias;
			}

			// Scale to seed range
			fOutput[y * nHeight + x] = fNoise / fScaleAcc;
		}

}

void regenerateSprite(int sizeX, int sizeY, int octaves, float bias, sf::Uint8* pixelGrid) {
	
	float* terrainN = new float[sizeX * sizeY];
	float* seed = new float[sizeX * sizeY];
	for (int i = 0; i < sizeY; i++) 
		for (int j = 0; j < sizeX; j++) 
			seed[i * sizeY + j] = (float)rand() / (float)RAND_MAX;

	PerlinNoise2D(sizeX, sizeY, seed, octaves, bias, terrainN);

	for (int y = 0; y < sizeY; y++) {
		for (int x = 0; x < sizeX; x++) {
			sf::Uint8 currentValue = (sf::Uint8)(terrainN[y * sizeY + x] * 255.0f);
			pixelGrid[(y * sizeY + x) * 4] = currentValue;
			pixelGrid[(y * sizeY + x) * 4 + 1] = currentValue;
			pixelGrid[(y * sizeY + x) * 4 + 2] = currentValue;
			pixelGrid[(y * sizeY + x) * 4 + 3] = (sf::Uint8)255;
		}
	}

	delete[] terrainN;
	delete[] seed;

	//return pixelGrid;
}

void generateHeightMap(int sizeX, int sizeY, int octaves, float bias, float* heightMap) {

	float* terrainN = new float[sizeX * sizeY];
	float* seed = new float[sizeX * sizeY];
	for (int i = 0; i < sizeY; i++)
		for (int j = 0; j < sizeX; j++)
			seed[i * sizeY + j] = (float)rand() / (float)RAND_MAX;

	PerlinNoise2D(sizeX, sizeY, seed, octaves, bias, terrainN);

	for (int y = 0; y < sizeY; y++) 
		for (int x = 0; x < sizeX; x++) 
			heightMap[(y * sizeY + x) ] = terrainN[y * sizeY + x] * 255.0f;

	delete[] terrainN;
	delete[] seed;
}