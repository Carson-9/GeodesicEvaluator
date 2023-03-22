#include "Path.hpp"

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


f64 euclideanLength(Point a, Point b) {
	f64 dx = (b.x - a.x);
	f64 dy = (b.y - a.y);
	f64 dz = (b.z - a.z);
	return sqrt(dx * dx + dy * dy + dz * dz);
}

f64 computePathLength(Path* p) {
	f64 length = 0.0f;
	for (int i = 0; i < p->precision - 1; i++) {
		length += euclideanLength(p->points[i], p->points[i + 1]);
	}

	return length;
}





b8 destroyPath(Path* p) {
	delete[] p->points;
	delete p;
	return true;
}