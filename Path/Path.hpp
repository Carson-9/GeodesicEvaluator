#pragma once

#include "utilitaries/types.hpp"
#include "terrain/terrain.hpp"

Path* initializePath(i16 precision, Point start, Point end, Terrain* terrain);

f64 computePathLength(Path* p);

b8 destroyPath(Path* p);

