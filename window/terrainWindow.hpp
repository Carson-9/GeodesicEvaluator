#pragma once
#include "../window/window.hpp"
#include "../terrain/terrain.hpp"
#include "../utilitaries/utilLib.hpp"
#include "../utilitaries/bezier.hpp"
#include "../Path/Path.hpp"

typedef struct {
	int WIDTH;
	int HEIGHT;
	Terrain* terrain;
	//threadpool* threads;
} terrainWindowArgs;

typedef struct {
	windowHierarchy* win;
	Terrain* terrain;
} drawTerrainArgs;

void multiThreadDrawTerrain(void* args);
void drawTerrain(windowHierarchy* win, Terrain* terrain, sf::Text text);

void multiThreadLaunchTerrainWindow(void* args);
void launchTerrainWindow(int width, int height, Terrain* mainTerrain);