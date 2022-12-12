#pragma once

#include "window/windowObjects.hpp"
#include "multithreading/threadpool.hpp"

const char DEFAULT_FONT[] = "Resources/ALBold.ttf";

void baseWindow(int width, int height);
void multiThreadedBaseWindow(void* args);
