#include "frechet.hpp"

i32 find_index_with_parameter_length(Path* p, f64 dist) {

	f64 partial_length = 0.0f;

	for (i32 i = 0; i < p->precision - 1; i++) {
		Point flatten_p = Point(p->points[i].x, p->points[i].y, 0.0f);
		Point flatten_next = Point(p->points[i+1].x, p->points[i+1].y, 0.0f);
		partial_length += euclideanLength(flatten_p, flatten_next);
		if (partial_length >= dist) return i;
	}

	return p->precision - 1;
}

f64 computeFlattenedLength(Path* p) {

	f64 l = 0.0f;

	for (i32 i = 0; i < p->precision - 1; i++) {
		Point flatten_p = Point(p->points[i].x, p->points[i].y, 0.0f);
		Point flatten_next = Point(p->points[i + 1].x, p->points[i + 1].y, 0.0f);
		l += euclideanLength(flatten_p, flatten_next);
	}
	return l;
}


bool are_path_close(Path* a, Path* b, f32 delta_distance) {
	f64 a_length = computeFlattenedLength(a);
	f64 b_length = computeFlattenedLength(b);

	i32 a_end_index = a->precision - 1;
	i32 b_end_index = b->precision - 1;

	if (b_length >= a_length) b_end_index = find_index_with_parameter_length(b, a_length);
	else a_end_index = find_index_with_parameter_length(a, b_length);

	bool** free_space_diagram = new bool* [b_end_index];
	for (int i = 0; i < b_end_index; i++) free_space_diagram[i] = new bool[a_end_index];

	for (int y = 0; y < b_end_index; y++) {
		for (int x = 0; x < a_end_index; x++) {
			Point flattened_b = Point(b->points[y].x, b->points[y].y, 0.0f);
			Point flattened_a = Point(a->points[x].x, a->points[x].y, 0.0f);
			free_space_diagram[y][x] = false;
			if (euclideanLength(flattened_b, flattened_a) <= delta_distance) free_space_diagram[y][x] = true;
		}
	}


	bool** path_existence_table = new bool* [b_end_index];
	for (int i = 0; i < b_end_index; i++) path_existence_table[i] = new bool[a_end_index];
	
	for (int y = 0; y < b_end_index; y++) {
		for (int x = 0; x < a_end_index; x++) {
			path_existence_table[y][x] = false;
			if (x == 0 && y == 0) path_existence_table[y][x] = free_space_diagram[0][0];
			else if (x == 0) path_existence_table[y][x] = (path_existence_table[y - 1][x] && free_space_diagram[y][x]);
			else if (y == 0) path_existence_table[y][x] = (path_existence_table[y][x - 1] && free_space_diagram[y][x]);
			else path_existence_table[y][x] = free_space_diagram[y][x] && (path_existence_table[y - 1][x] || path_existence_table[y][x - 1] || path_existence_table[y - 1][x - 1]);
		}
	}


	return path_existence_table[b_end_index - 1][a_end_index - 1];

}