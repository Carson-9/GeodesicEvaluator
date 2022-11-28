#pragma once
#include "pseudoPerlin.hpp"
#include "utilLib.hpp"

class Terrain {
	public:
		
		Terrain(int sizeX, int SizeY, int octaves, float bias, float blend);
		~Terrain();

		void generateTerrain(int octaves, float bias);
		void generateColorMap();
		int getSizeX();
		int getSizeY();
		void setBlend(float newBlend);

		float* getHeightMap();
		sf::Uint8* getColorMap();
		void makeSprite(sf::Sprite sprite);


	private:
		int sizeX;
		int sizeY;
		float* heightMap;
		float blend;
		sf::Uint8* colorMap;
		sf::Image img;
		sf::Texture tex;
};



void colorTerrain(int sizeX, int sizeY, sf::Uint8* pixelArray, float sigmoidBlend);
sf::Color GetCorrespondingColor(float height, float sigmoidBlend);

const sf::Color DEEP_SEA(0, 26, 51, 255);
const sf::Color LOW_SEA(0, 105, 146, 255);
const sf::Color MID_SEA(41, 138, 170, 255);
const sf::Color UP_SEA(81, 172, 194, 255);
const sf::Color TOP_SEA(122, 205, 218, 255);

const sf::Color SAND(194, 178, 128, 255);
const sf::Color LIGHT_GRASS(126, 200, 80, 255);
const sf::Color DARK_GRASS(86, 158, 12, 255);
const sf::Color ROCK(151, 124, 83, 255);
const sf::Color STONE(45, 44, 44, 255);
const sf::Color SNOW(255, 255, 255, 255);

const int COLOR_NUMBER = 11;
const sf::Color COLOR_LIST[] = { DEEP_SEA, LOW_SEA, MID_SEA, UP_SEA, TOP_SEA, SAND, LIGHT_GRASS, DARK_GRASS, ROCK, STONE, SNOW };
const float COLOR_HEIGHT_LIST[] = {0.0f, 20.5f, 40.8f, 61.2f, 71.6f, 76.0f, 86.5f, 125.5f, 168.5f, 210.0f, 234.6f};