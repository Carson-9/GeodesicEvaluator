#include "types.hpp"

pointList makePointList() {
	pointList list = new pointNode;
	list->p = { 0, 0, 0};
	list->prev = nullptr;
	list->next = nullptr;
	return list;
}

void deletePointList(pointList list) {
	if (list == nullptr) return;
	pointList nextList = list->next;
	delete list;
	deletePointList(nextList);
}

pointList pointListAddHead(pointList list, Point p) {
	pointList newPoint = makePointList();
	newPoint->p = p;
	newPoint->next = list;
	newPoint->prev = list->prev;
	if (newPoint->prev != nullptr) newPoint->prev->next = newPoint;
	list->prev = newPoint;

	return newPoint;
}

i16 getPointListSize(pointList list) {
	i16 size = 0;
	pointList pointer = list;
	while (pointer != nullptr) {
		size++;
		pointer = pointer->next;
	}

	return size;
}

Point* pointListToArray(pointList list) {
	i16 size = 0;

	pointList pointer = list;

	while (pointer != nullptr) {
		size++;
		pointer = pointer->next;
	}

	Point* array = new Point[size];

	pointer = list;
	size = 0;

	while (pointer != nullptr) {
		array[size++] = pointer->p;
		pointer = pointer->next;
	}
	return array;
}

void printPoint(Point* p) {
	printf("Point : X = %f, Y = %f, Z = %f\n", p->x, p->y, p->z);
}

void copyPoint(Point* a, Point* b) {
	b->x = a->x;
	b->y = a->y;
	b->z = a->z;
}

f32 dot(Point a, Point b) {
	return (a.x * b.x + a.y * b.y + a.z * b.z);
}

f32 norm(Point a) {
	return sqrt(dot(a, a));
}

f32 dot(vec2f a, vec2f b) {
	return (a.x * b.x + a.y * b.y);
}

f32 dot(vec3f a, vec3f b) {
	return (a.x * b.x + a.y * b.y + a.z * b.z);
}

vec3f cross(vec3f a, vec3f b) {
	return vec3f(
		a.y * b.z - a.z * b.y,
		a.z * b.x - a.x * b.z,
		a.x * b.y - a.y * b.x
	);
}
