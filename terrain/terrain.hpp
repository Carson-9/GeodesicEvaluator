#pragma once
#include "utilitaries/pseudoPerlin.hpp"
#include "utilitaries/utilLib.hpp"
#include "utilitaries/stringManipulation.hpp"
#include "utilitaries/types.hpp"
#include "utilitaries/bezier.hpp"
#include <sstream>
#include <cmath>


#define DOWN_OFFSET 100000
#define PATH_PRECISION 500
#define DISTANCE_CALCULATION_PRECISION 500
#define DELTA_PRECISION 1


class Terrain {
	public:
		
		int			sizeX;
		int			sizeY;
		float*		heightMap;

		Terrain(int sizeX, int SizeY, int octaves, float bias, float blend);
		Terrain(Terrain* t);
		~Terrain();

		void generateTerrain();
		void generateColorMap();
		void generateFromFile(const char* fileName);
		void generateOBJFile(const char* fileName, float heightScale);
		void generatePath();
		int getSizeX();
		int getSizeY();
		int getOctaves();
		float getBias();
		float getBlend();
		float getBlendResKeyboard();
		bool isInScreen(Point* a);

		float* getHeightMap();
		sf::Uint8* getColorMap();
		Point* getPointA();
		Point* getPointB();
		void drawLine(Point* a, Point* b);
		void drawPath(void* p);

		void setOctaves(int newOctaves);
		void setBias(float newBias);
		void setBlend(float newBlend);
		void setHeightMap(float* newMap, int sizeX, int sizeY);
		void setBlendResKeyboard(float newBlendRes);
		void setIndicatorList(int* list, int size);
		void setPoints(Point* a, Point* b);

		void naivePath();

		void drawBezier(bezier3 b);
		void bezierPath();

		void best_path();

		void toggleIndicatorDraw();

		void makeSprite(sf::Sprite sprite);


		f32 surfaceFunction(Point* p);
		void fillPoint(Point* p);


	private:
		int			octaves;
		float		bias;
		float		blend;
		float		blendResolKeyboard;
		bool		drawHeightIndicators;
		bool*		heightIndicatorList; //Faster checkup with a boolean associated to the height
		sf::Uint8*	colorMap;
		sf::Image	img;
		sf::Texture	tex;

		Point*		pointA;
		Point*		pointB;
};


struct Path {
	Point	 startingPoint;
	Point	 endPoint;
	Point* points;
	i16		 precision;
	Terrain* linkedTerrain;
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
const float COLOR_HEIGHT_LIST[] = {
	0.0f, 
	20.5f, 
	40.8f,
	61.2f, 
	71.6f, 
	76.0f, 
	86.5f, 
	125.5f, 
	168.5f, 
	210.0f, 
	234.6f};


const f32 CUTTING_HEIGHT = ((COLOR_HEIGHT_LIST[5] + COLOR_HEIGHT_LIST[4]) / 2.0f);

const sf::Color PATH_COLOR(255, 0, 0, 255);