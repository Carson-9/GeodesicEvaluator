#pragma once

#include "window/windowObjects.hpp"
#include "multithreading/threadpool.hpp"
#include "terrain/terrain.hpp"
#include "Solver/solver.hpp"

typedef struct {
	int WIDTH;
	int HEIGHT;
	Terrain* terrain;
} controlWindowArgs;

const char DEFAULT_FONT[] = "Resources/ALBold.ttf";

void baseWindow(int width, int height, Terrain* terrain);
void multiThreadedBaseWindow(void* args);
void generateFieldLines(Terrain* terrain, int FieldLineNumber);