#include "terrain.hpp"

sf::Color GetCorrespondingColor(float height, float sigmoidBlend) {

	sf::Color blend;

	for (int i = 0; i < COLOR_NUMBER - 1; i++) {
		if (height < COLOR_HEIGHT_LIST[i + 1]) {
			float percent = (height - COLOR_HEIGHT_LIST[i]) / (COLOR_HEIGHT_LIST[i + 1] - COLOR_HEIGHT_LIST[i]);
			percent = sigmoid(percent, 1.0f, 0.5f, sigmoidBlend);
			//percent *= 1.5f;
			blend.r = (sf::Uint8)((1.0f - percent) * COLOR_LIST[i].r + percent * COLOR_LIST[i + 1].r);
			blend.g = (sf::Uint8)((1.0f - percent) * COLOR_LIST[i].g + percent * COLOR_LIST[i + 1].g);
			blend.b = (sf::Uint8)((1.0f - percent) * COLOR_LIST[i].b + percent * COLOR_LIST[i + 1].b);
			return blend;
			//percent *= 1.5f;
			//float color_interp = 0.5f + 1.5f * percent;
		}
	}

	return SNOW;
	
}

void colorTerrain(int sizeX, int sizeY, sf::Uint8* pixelArray, float sigmoidBlend) {

	sf::Color buffer;

	for (int y = 0; y < sizeY; y++) {
		for (int x = 0; x < sizeX; x++) {
			buffer = GetCorrespondingColor(pixelArray[(y * sizeY + x) * 4], sigmoidBlend);
			pixelArray[(y * sizeY + x) * 4] = buffer.r;
			pixelArray[(y * sizeY + x) * 4 + 1] = buffer.g;
			pixelArray[(y * sizeY + x) * 4 + 2] = buffer.b;
		}
	}
}

Terrain::Terrain(int sizeX, int sizeY, int octaves, float bias, float blend) {
	this->sizeX = sizeX;
	this->sizeY = sizeY;
	this->heightMap = new float[sizeX * sizeY];
	this->colorMap = nullptr;
	this->generateTerrain(octaves, bias);
	this->blend = blend;
}

Terrain::~Terrain() {
	if(this->colorMap != nullptr) delete this->colorMap;
	delete this->heightMap;
}

void Terrain::generateTerrain(int octaves, float bias) {
	generateHeightMap(this->sizeX, this->sizeY, octaves, bias, this->heightMap);
	if (this->colorMap != nullptr) {
		delete this->colorMap;
		this->colorMap = nullptr;
	}
}

float* Terrain::getHeightMap() {
	return this->heightMap;
}

sf::Uint8* Terrain::getColorMap() {
	return this->colorMap;
}

int Terrain::getSizeX() {
	return this->sizeX;
}

int Terrain::getSizeY() {
	return this->sizeY;
}

void Terrain::setBlend(float newBlend) {
	this->blend = newBlend;
}

void Terrain::generateColorMap() {
	if (this->colorMap == nullptr) this->colorMap = new sf::Uint8[this->sizeX * this->sizeY * 4];
	sf::Color buffer;

	for (int y = 0; y < this->sizeY; y++) {
		for (int x = 0; x < this->sizeX; x++) {
			buffer = GetCorrespondingColor(this->heightMap[y * this->sizeX + x], this->blend);
			this->colorMap[(y * sizeX + x) * 4] = buffer.r;
			this->colorMap[(y * sizeX + x) * 4 + 1] = buffer.g;
			this->colorMap[(y * sizeX + x) * 4 + 2] = buffer.b;
			this->colorMap[(y * sizeX + x) * 4 + 3] = 255;
		}
	}
}

void Terrain::makeSprite(sf::Sprite sprite) {
	if (this->colorMap == nullptr) this->generateColorMap();
	this->img.create(this->sizeX, this->sizeY, this->colorMap);
	this->tex.loadFromImage(img);

	sprite.setTexture(tex);
}
