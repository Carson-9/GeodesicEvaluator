#include "Path.hpp"

Path* emptyPath(Terrain* terrain) {
	Path* path = (Path*)malloc(sizeof(Path));
	if (!path) return nullptr;
	path->linkedTerrain = terrain;
	path->precision = 0;

	return path;
}

Path* initializePath(i16 precision, Point start, Point end, Terrain* terrain) {
	Path* path = (Path*)malloc(sizeof(Path));
	if (!path) return nullptr;
	if (precision < 2) {
		free(path);
		return nullptr;
	}
	path->precision = precision;
	path->points = new Point[precision];
	path->points[0] = start;
	path->points[precision - 1] = end;
	path->linkedTerrain = terrain;

	terrain->fillPoint(&path->points[0]);
	terrain->fillPoint(&path->points[precision - 1]);

	f32 xSlope = (end.x - start.x) / (float)precision;
	f32 ySlope = (end.y - start.y) / (float)precision;

	for (int i = 1; i < precision - 1; i++) {
		path->points[i].x = start.x + xSlope * i;
		path->points[i].y = start.y + ySlope * i;
		terrain->fillPoint(&path->points[i]);
	}

	return path;
}

void projectOnSurface(Path* p) {
	for (int point = 0; point < p->precision - 1; point++) {
		p->linkedTerrain->fillPoint(&p->points[point]);
	}
}


f64 euclideanLength(Point a, Point b) {
	return sqrt((b.x - a.x) * (b.x - a.x) + 
		(b.y - a.y) * (b.y - a.y) +
		(b.z - a.z) * (b.z - a.z));
}

f64 computePathLength(Path* p) {
	if (!p) return 0;
	f64 length = 0.0f;
	for (int i = 0; i < p->precision - 1; i++) {
		length += euclideanLength(p->points[i], p->points[i + 1]);
	}

	return length;
}


void normalizePoint(Point* p) {
	f64 length = sqrt(p->x * p->x +
		p->y * p->y +
		p->z * p->z);

	p->x = p->x / length;
	p->y = p->y / length;
	p->z = p->z / length;

}

void resizePoint(Point* p, f64 lambda) {
	p->x *= lambda;
	p->y *= lambda;
	p->z *= lambda;
}


b8 destroyPath(Path* p) {
	delete[] p->points;
	delete p;
	return true;
}

bool isInScreen(Point* p, i16 WIDTH, i16 HEIGHT) {
	return (p->x >= 0 && p->x < (WIDTH - 1) && p->y >= 0 && p->y < (HEIGHT - 1));
}

f64 naive_optimisation_recursion(pointList points, i16 depth, Terrain* terrain) {

	printf("Depth : %d\n", depth);

	if (depth <= 0) {
		if (points != nullptr && points->next != nullptr)
			return computeSurfaceLength(&points->p, &points->next->p, DISTANCE_CALCULATION_PRECISION, terrain);
		else return 0.0f;
	}

	// Else create a point on the midLine
	
	if (points == nullptr || points->next == nullptr) {
		printf("Empty List!\n");
		return 0.0f;
	}

	Point a = points->p;
	Point b = points->next->p;

	Point midPoint = (a + b) * 0.5f;
	terrain->fillPoint(&midPoint);
	Point normalDirection = (b - a);
	if (normalDirection.x == 0 && normalDirection.y == 0) return 0.0f;
	SWAP(&normalDirection.x, &normalDirection.y);
	normalDirection.x *= -1;
	normalizePoint(&normalDirection);

	Point bestPoint;
	Point currentPoint;

	bestPoint = midPoint;
	currentPoint = midPoint;

	f32 minDist = euclideanLength(bestPoint, points->p) + 
		computeSurfaceLength(&bestPoint, &points->next->p, DELTA_PRECISION * depth, terrain);
	f32 curDist;

	i16 WIDTH = terrain->getSizeX();
	i16 HEIGHT = terrain->getSizeY();

	while (isInScreen(&currentPoint, WIDTH, HEIGHT)) {
		terrain->fillPoint(&currentPoint);
		curDist = computeSurfaceLength(&currentPoint, &points->p, DELTA_PRECISION * depth, terrain)
			+ computeSurfaceLength(&currentPoint, &points->next->p, DELTA_PRECISION * depth, terrain);
		if (curDist < minDist) {
			minDist = curDist;
			bestPoint = currentPoint;
		}
		currentPoint = currentPoint + normalDirection;
	}

	currentPoint = midPoint;

	while (isInScreen(&currentPoint, WIDTH, HEIGHT)) {
		terrain->fillPoint(&currentPoint);
		curDist = computeSurfaceLength(&currentPoint, &points->p, DELTA_PRECISION * depth, terrain)
			+ computeSurfaceLength(&currentPoint, &points->next->p, DELTA_PRECISION * depth, terrain);
		if (curDist < minDist) {
			minDist = curDist;
			bestPoint = currentPoint;
		}
		currentPoint = currentPoint - normalDirection;
	}

	terrain->fillPoint(&bestPoint);

	pointList newPoints = pointListAddHead(points->next, bestPoint);
	f32 leftLength = naive_optimisation_recursion(points, depth / 2, terrain);
	f32 rightLength = naive_optimisation_recursion(newPoints, depth / 2, terrain);
	return leftLength + rightLength;
}


f64 naive_optimisation(pointList points, Terrain* terrain) {
	return naive_optimisation_recursion(points, PATH_PRECISION, terrain);
}




f64 computeSurfaceLength(Point* a, Point* b, i16 res, Terrain* terrain) {
	Path* path = initializePath(res, *a, *b, terrain);
	return computePathLength(path);
}


f64 bezier_approximation(Path* path, Point a, Point b, Terrain* terrain) {

	path = initializePath(PATH_PRECISION, a, b, terrain);
	Path* new_path = initializePath(PATH_PRECISION, a, b, terrain);

	f64 best_distance = computePathLength(path);
	f64 curdist = best_distance;

	Point delta = b - a;

	bezier3 curve(a, b);

	i16 WIDTH = terrain->getSizeX();
	i16 HEIGHT = terrain->getSizeY();


	for (int ca_coord = 0; ca_coord < (WIDTH * HEIGHT) / BEZIER_CHUNKING; ca_coord++) {

		printf("Iteration : %d\n", ca_coord);

		for (int cb_coord = 0; cb_coord < (WIDTH * HEIGHT) / BEZIER_CHUNKING; cb_coord++) {

			i16 aCoord = ca_coord * BEZIER_CHUNKING;
			i16 bCoord = cb_coord * BEZIER_CHUNKING;

			curve.Control_A = Point(aCoord % WIDTH, aCoord / WIDTH, 0.0f);
			curve.Control_B = Point(bCoord % WIDTH, bCoord / WIDTH, 0.0f);

			//printf("Dot : %f", dot((curve.Control_B - curve.Control_A), delta));

			//If condition is met, then the path is making a loop : Not optimal
			if (dot((curve.Control_B - curve.Control_A), delta) < 0) {
				//printf("Snipp!\n");
				continue;
			}

			curve.updatePolynomial();

			for (int i = 1; i < PATH_PRECISION - 1; i++) {
				new_path->points[i] = curve.evaluate(i / PATH_PRECISION);
				terrain->fillPoint(&new_path->points[i]);
			}

			curdist = computePathLength(new_path);
			if (curdist < best_distance) {
				best_distance = curdist;
				for (int i = 1; i < PATH_PRECISION - 1; i++) path->points[i] = new_path->points[i];
			}

		}
	}

	for (int i = 1; i < PATH_PRECISION - 1; i++) {
		path->points[i] = new_path->points[i];
	}

	return best_distance;

}