#include "Solver/solver.hpp"


Point terrain_du(Terrain* terrain, Point p) {
	Point curP = Point(p.x - (SOLVER_DERIVATIVE_PRECISION / 2), p.y, 0);
	Point curPdu = Point(p.x + (SOLVER_DERIVATIVE_PRECISION / 2), p.y, 0);
	terrain->fillPoint(&curP);
	terrain->fillPoint(&curPdu);
	return ((curPdu - curP) * (1 / SOLVER_DERIVATIVE_PRECISION));
}


Point terrain_dv(Terrain* terrain, Point p) {
	Point curP = Point(p.x, p.y - (SOLVER_DERIVATIVE_PRECISION / 2), 0);
	Point curPdv = Point(p.x, p.y + (SOLVER_DERIVATIVE_PRECISION / 2), 0);
	terrain->fillPoint(&curP);
	terrain->fillPoint(&curPdv);
	return ((curPdv - curP) * (1 / SOLVER_DERIVATIVE_PRECISION));
}

Point terrain_duu(Terrain* terrain, Point p) {
	Point curP = Point(p.x, p.y, terrain->surfaceFunction(&p));
	Point curPdu = Point(p.x + SOLVER_DERIVATIVE_PRECISION, p.y, 0);
	Point curPneg = Point(p.x - SOLVER_DERIVATIVE_PRECISION, p.y, 0);
	terrain->fillPoint(&curPdu);
	terrain->fillPoint(&curPneg);

	return ((curPdu - (curP * 2) + curPneg) * (1 / (SOLVER_DERIVATIVE_PRECISION * SOLVER_DERIVATIVE_PRECISION)));
}

Point terrain_dvv(Terrain* terrain, Point p) {
	Point curP = Point(p.x, p.y, terrain->surfaceFunction(&p));
	Point curPdv = Point(p.x, p.y + SOLVER_DERIVATIVE_PRECISION, 0);
	Point curPneg = Point(p.x, p.y - SOLVER_DERIVATIVE_PRECISION, 0);
	terrain->fillPoint(&curPdv);
	terrain->fillPoint(&curPneg);

	return ((curPdv - (curP * 2) + curPneg) * (1 / (SOLVER_DERIVATIVE_PRECISION * SOLVER_DERIVATIVE_PRECISION)));
}

Point terrain_dvu(Terrain* terrain, Point p) {
	Point curPneg = Point(p.x - SOLVER_DERIVATIVE_PRECISION, p.y - SOLVER_DERIVATIVE_PRECISION, 0);
	Point curPdu = Point(p.x + SOLVER_DERIVATIVE_PRECISION, p.y - SOLVER_DERIVATIVE_PRECISION, 0);
	Point curPdv = Point(p.x - SOLVER_DERIVATIVE_PRECISION, p.y + SOLVER_DERIVATIVE_PRECISION, 0);
	Point curPdudv= Point(p.x + SOLVER_DERIVATIVE_PRECISION, p.y + SOLVER_DERIVATIVE_PRECISION, 0);
	terrain->fillPoint(&curPneg);
	terrain->fillPoint(&curPdu);
	terrain->fillPoint(&curPdv);
	terrain->fillPoint(&curPdudv);

	return ((curPdudv - curPdv - curPdu + curPneg) * (1 / (4 * SOLVER_DERIVATIVE_PRECISION * SOLVER_DERIVATIVE_PRECISION)));
}


vec<f64>* get_derivative(Terrain* terrain, const vec<f64>& current_value) {
	
	vec<f64>* result = new vec<f64>(4);
	result->coord[0] = current_value.coord[2];
	result->coord[1] = current_value.coord[3];

	Point partial_u = terrain_du(terrain, Point(current_value.coord[0], current_value.coord[1], 0));
	Point partial_v = terrain_dv(terrain, Point(current_value.coord[0], current_value.coord[1], 0));
	
	f64 norm_du_squared = dot(partial_u, partial_u);
	f64 norm_dv_squared = dot(partial_v, partial_v);

	Point partial_uu = terrain_duu(terrain, Point(current_value.coord[0], current_value.coord[1], 0));
	Point partial_vu = terrain_dvu(terrain, Point(current_value.coord[0], current_value.coord[1], 0));
	Point partial_vv = terrain_dvv(terrain, Point(current_value.coord[0], current_value.coord[1], 0));

	f64 alpha_1 = dot(partial_uu, partial_u) / norm_du_squared;
	f64 beta_1 = dot(partial_vu, partial_u) / norm_du_squared;
	f64 gamma_1 = dot(partial_vv, partial_u) / norm_du_squared;

	f64 alpha_2 = dot(partial_uu, partial_v) / norm_dv_squared;
	f64 beta_2 = dot(partial_vu, partial_v) / norm_dv_squared;
	f64 gamma_2 = dot(partial_vv, partial_v) / norm_dv_squared;

	//printf("Alpha, beta, gamma : %f, %f, %f, %f, %f, %f\n", alpha_1, beta_1, gamma_1, alpha_2, beta_2, gamma_2);

	result->coord[2] = -(result->coord[0] * result->coord[0] * alpha_1
		+ result->coord[1] * result->coord[1] * gamma_1
		+ 2 * result->coord[0] * result->coord[1] * beta_1);

	result->coord[3] = -(result->coord[0] * result->coord[0] * alpha_2
		+ result->coord[1] * result->coord[1] * gamma_2
		+ 2 * result->coord[0] * result->coord[1] * beta_2);

	return result;
}


Path* linked_list_to_path(std::vector<vec<f64>> l, Terrain* terrain) {
	
	Path* p = new Path();
	p->linkedTerrain = terrain;
	i32 size = l.size();
	p->precision = size;
	p->points = new Point[size];

	for (int i = 0; i < size; i++) {
		p->points[i] = Point(l[i].coord[0], l[i].coord[1], 0.0f);
		terrain->fillPoint(&p->points[i]);
	}

	p->endPoint = p->points[size - 1];
	return p;

}


std::vector<vec<f64>> RK4(Terrain* terrain, Point A, Point B, f64 initial_u_speed, f64 initial_v_speed, f64 delta_time) {
	
	std::vector<vec<f64>> values = std::vector<vec<f64>>();

	vec<f64> initial = vec<f64>(4);

	initial.coord[0] = A.x;
	initial.coord[1] = A.y;
	initial.coord[2] = initial_u_speed;
	initial.coord[3] = initial_v_speed;

	values.push_back(initial);

	i16 WIDTH = terrain->getSizeX();
	i16 HEIGHT = terrain->getSizeY();

	vec<f64>* k1;
	vec<f64>* k2;
	vec<f64>* k3;
	vec<f64>* k4;
	vec<f64> entire_derivative = vec<f64>(4);
	vec<f64> new_value = vec<f64>(4);
	vec<f64> b_vect = vec<f64>(4);

	new_value = initial;

	for (int i = 0; i < SOLVER_ITERATION_LIMIT; i++) {

		k1 = get_derivative(terrain, new_value);
		k2 = get_derivative(terrain, new_value + (*k1 * (delta_time / 2)));
		k3 = get_derivative(terrain, new_value + (*k2 * (delta_time / 2)));
		k4 = get_derivative(terrain, new_value + (*k3 * delta_time));

		entire_derivative = (*k1 + *k2 * 2 + *k3 * 2 + *k4) * (delta_time / 6);
		

		new_value = new_value + entire_derivative;

		f64 speed_norm = sqrt(new_value.coord[2] * new_value.coord[2] + 
			new_value.coord[3] * new_value.coord[3]);
		new_value.coord[2] /= speed_norm;
		new_value.coord[3] /= speed_norm;

		if (speed_norm >= SOLVER_BREAK) break;

		if (new_value.coord[0] < 0 || new_value.coord[0] >= WIDTH ||
			new_value.coord[1] < 0 || new_value.coord[1] >= HEIGHT || 
			isnan(new_value.coord[0]) || isnan(new_value.coord[1])) break;

		Point new_point = Point(new_value.coord[0], new_value.coord[1], 0);
		terrain->fillPoint(&new_point);
		if (new_point.z <= CUTTING_HEIGHT) return values;

		/*if (speed_norm > 10000.0f) {
			printf("%f\n", speed_norm);
			printf("Début\n");
			k1->log();
			k2->log();
			k3->log();
			k4->log();
		}*/

		values.push_back(new_value);

		if (norm(B - new_point) < SOLVER_EPSILON_VALUE) {

			b_vect.coord[0] = B.x;
			b_vect.coord[1] = B.y;
			b_vect.coord[2] = new_value.coord[2];
			b_vect.coord[3] = new_value.coord[3];

			values.push_back(b_vect);
			break;
		}

		delete k1;
		delete k2;
		delete k3;
		delete k4;
	}

	return values;
}

Path* try_solution(Terrain* terrain, Point A, Point B) {

	terrain->generateColorMap();

	Terrain* smoothed_terrain = new Terrain(terrain);

	i32 WIDTH = smoothed_terrain->getSizeX();
	i32 HEIGHT = smoothed_terrain->getSizeY();

	for (int y = 0; y < HEIGHT; y++) {
		for (int x = 0; x < WIDTH; x++) {

			if (x == 0 || y == 0 || x == WIDTH - 1 || y == HEIGHT - 1) 
				smoothed_terrain->heightMap[y * WIDTH + x] = terrain->heightMap[y * WIDTH + x];

			else smoothed_terrain->heightMap[y * WIDTH + x] = (1 / 9.0f) * (
				terrain->heightMap[(y - 1) * WIDTH + (x - 1)] +
				terrain->heightMap[(y - 1) * WIDTH + x] +
				terrain->heightMap[(y - 1) * WIDTH + (x + 1)] +
				terrain->heightMap[y * WIDTH + (x - 1)] +
				terrain->heightMap[y * WIDTH + x] +
				terrain->heightMap[y * WIDTH + (x + 1)] +
				terrain->heightMap[(y + 1) * WIDTH + (x - 1)] +
				terrain->heightMap[(y + 1) * WIDTH + x] +
				terrain->heightMap[(y + 1) * WIDTH + (x + 1)]);
		} 
	}

	Path* best_solution = nullptr;
	std::vector<vec<f64>> current_solution;
	f64 best_length = -1.0f;
	vec<f64> last_vect = vec<f64>(4);

	Path* current_path;
	
	for (f64 theta = 0; theta < 6.28f; theta += SOLVER_CIRCLE_RESOLUTION) {
		
		current_solution = RK4(smoothed_terrain, A, B, cos(theta), sin(theta), SOLVER_STEP);
		current_path = linked_list_to_path(current_solution, terrain);
		terrain->drawPath((void*)current_path);
		last_vect = current_solution.back();
		
		if (abs(last_vect.coord[0] - B.x) <= SOLVER_EPSILON_VALUE && abs(last_vect.coord[1] - B.y) <= SOLVER_EPSILON_VALUE) {

			current_path = linked_list_to_path(current_solution, terrain);
			f64 current_length = computePathLength(current_path);
			if (best_length < 0 || current_length < best_length) {
				best_length = current_length;
				best_solution = current_path;
			}
		}
	}

	return best_solution;
}