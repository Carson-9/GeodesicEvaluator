#pragma once

#include "utilitaries/types.hpp"
#include "utilitaries/bezier.hpp"
#include "terrain/terrain.hpp"


#define BEZIER_CHUNKING 199
#define BEZIER_ITERATIONS 200

void normalizePoint(Point* p);

void resizePoint(Point* p, f64 lambda);

Path* emptyPath(Terrain* terrain);
Path* initializePath(i16 precision, Point start, Point end, Terrain* terrain);
Path* bezierToPath(bezier3 b, Terrain* terrain, i32 precision);

void projectOnSurface(Path* p);

f64 euclideanLength(Point a, Point b);

f64 computePathLength(Path* p);
f64 computeSurfaceLength(Point* a, Point* b, i16 resolution, Terrain* linkedTerrain);

b8 destroyPath(Path* p);

f64 naive_optimisation(pointList points, Terrain* terrain);




// BEZIER Approx
f64 bezier_approximation(Path* path, Point a, Point b, Terrain* terrain);
