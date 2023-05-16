#include "Solver/solver.hpp"


Point terrain_du(Terrain* terrain, Point p) {
	Point curP = Point(p.x, p.y, terrain->surfaceFunction(&p));
	Point curPdu = Point(p.x + SOLVER_DERIVATIVE_PRECISION, p.y, 0);
	terrain->fillPoint(&curPdu);
	return ((curPdu - curP) * (1 / SOLVER_DERIVATIVE_PRECISION));
}


Point terrain_dv(Terrain* terrain, Point p) {
	Point curP = Point(p.x, p.y, terrain->surfaceFunction(&p));
	Point curPdv = Point(p.x, p.y + SOLVER_DERIVATIVE_PRECISION, 0);
	terrain->fillPoint(&curPdv);
	return ((curPdv - curP) * (1 / SOLVER_DERIVATIVE_PRECISION));
}

Point terrain_duu(Terrain* terrain, Point p) {
	Point curP = Point(p.x, p.y, terrain->surfaceFunction(&p));
	Point curPdu = Point(p.x + SOLVER_DERIVATIVE_PRECISION, p.y, 0);
	terrain->fillPoint(&curPdu);

	Point derivative_1 = terrain_du(terrain, curP);
	Point derivative_2 = terrain_du(terrain, curPdu);

	return ((derivative_2 - derivative_1) * (1 / SOLVER_DERIVATIVE_PRECISION));
}

Point terrain_dvu(Terrain* terrain, Point p) {
	Point curP = Point(p.x, p.y, terrain->surfaceFunction(&p));
	Point curPdu = Point(p.x + SOLVER_DERIVATIVE_PRECISION, p.y, 0);
	terrain->fillPoint(&curPdu);

	Point derivative_1 = terrain_dv(terrain, curP);
	Point derivative_2 = terrain_dv(terrain, curPdu);

	return ((derivative_2 - derivative_1) * (1 / SOLVER_DERIVATIVE_PRECISION));
}

Point terrain_dvv(Terrain* terrain, Point p) {
	Point curP = Point(p.x, p.y, terrain->surfaceFunction(&p));
	Point curPdv = Point(p.x, p.y + SOLVER_DERIVATIVE_PRECISION, 0);
	terrain->fillPoint(&curPdv);

	Point derivative_1 = terrain_du(terrain, curP);
	Point derivative_2 = terrain_du(terrain, curPdv);

	return ((derivative_2 - derivative_1) * (1 / SOLVER_DERIVATIVE_PRECISION));
}


vec<f32>* get_derivative(Terrain* terrain, const vec<f32>& current_value) {
	
	vec<f32>* result = new vec<f32>(4);
	result->coord[0] = current_value.coord[2];
	result->coord[1] = current_value.coord[3];

	Point partial_u = terrain_du(terrain, Point(current_value.coord[0], current_value.coord[1], 0)).normalize();
	Point partial_v = terrain_dv(terrain, Point(current_value.coord[0], current_value.coord[1], 0)).normalize();
	
	Point partial_uu = terrain_duu(terrain, Point(current_value.coord[0], current_value.coord[1], 0));
	Point partial_vu = terrain_dvu(terrain, Point(current_value.coord[0], current_value.coord[1], 0));
	Point partial_vv = terrain_dvv(terrain, Point(current_value.coord[0], current_value.coord[1], 0));

	f32 alpha_1 = dot(partial_uu, partial_u);
	f32 beta_1 = dot(partial_vu, partial_u);
	f32 gamma_1 = dot(partial_vv, partial_u);

	f32 alpha_2 = dot(partial_uu, partial_v);
	f32 beta_2 = dot(partial_vu, partial_v);
	f32 gamma_2 = dot(partial_vv, partial_v);

	result->coord[2] = -(result->coord[0] * result->coord[0] * alpha_1
		+ result->coord[1] * result->coord[1] * gamma_1
		+ 2 * result->coord[0] * result->coord[1] * beta_1);

	result->coord[3] = -(result->coord[0] * result->coord[0] * alpha_2
		+ result->coord[1] * result->coord[1] * gamma_2
		+ 2 * result->coord[0] * result->coord[1] * beta_2);

	return result;
}


Path* linked_list_to_path(std::vector<vec<f32>> l, Terrain* terrain) {
	
	Path* p = new Path();
	p->linkedTerrain = terrain;
	i16 size = l.size();
	p->precision = size;
	p->points = new Point[size];

	for (int i = 0; i < size; i++) {
		p->points[i] = Point(l[i].coord[0], l[i].coord[1], 0.0f);
		terrain->fillPoint(&p->points[i]);
	}

	p->endPoint = p->points[size - 1];
	return p;

}


std::vector<vec<f32>> RK4(Terrain* terrain, Point A, Point B, f32 initial_u_speed, f32 initial_v_speed, f32 delta_time) {
	
	std::vector<vec<f32>> values = std::vector<vec<f32>>();

	vec<f32> initial = vec<f32>(4);

	initial.coord[0] = A.x;
	initial.coord[1] = A.y;
	initial.coord[2] = initial_u_speed;
	initial.coord[3] = initial_v_speed;

	values.push_back(initial);

	i16 WIDTH = terrain->getSizeX();
	i16 HEIGHT = terrain->getSizeY();


	vec<f32>* k1;
	vec<f32>* k2;
	vec<f32>* k3;
	vec<f32>* k4;
	vec<f32> entire_derivative = vec<f32>(4);
	vec<f32> new_value = vec<f32>(4);
	vec<f32> b_vect = vec<f32>(4);

	new_value = initial;

	for (int i = 0; i < SOLVER_ITERATION_LIMIT; i++) {

		k1 = get_derivative(terrain, new_value);
		k2 = get_derivative(terrain, new_value + (*k1 * (delta_time / 2)));
		k3 = get_derivative(terrain, new_value + (*k2 * (delta_time / 2)));
		k4 = get_derivative(terrain, new_value + (*k3 * delta_time));

		entire_derivative = (*k1 + *k2 * 2 + *k3 * 2 + *k4) * (delta_time / 6);
		

		new_value = new_value + entire_derivative;

		f32 speed_norm = sqrt(new_value.coord[2] * new_value.coord[2] + 
			new_value.coord[3] * new_value.coord[3]);
		new_value.coord[2] /= speed_norm;
		new_value.coord[3] /= speed_norm;

		if (new_value.coord[0] < 0 || new_value.coord[0] >= WIDTH ||
			new_value.coord[1] < 0 || new_value.coord[1] >= HEIGHT) break;

		Point new_point = Point(new_value.coord[0], new_value.coord[1], 0);
		terrain->fillPoint(&new_point);
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

std::vector<vec<f32>> solve_for_boundaries(Terrain* terrain, Point A, Point B, f32 initial_u_speed, f32 initial_v_speed, f32 delta_time) {
	
	//printf("Solving for Boundaries!\n");
	std::vector<vec<f32>> values = std::vector<vec<f32>>();

	vec<f32> initial = vec<f32>(4);
	
	initial.coord[0] = A.x;
	initial.coord[1] = A.y;
	initial.coord[2] = initial_u_speed;
	initial.coord[3] = initial_v_speed;

	values.push_back(initial);

	i16 WIDTH = terrain->getSizeX();
	i16 HEIGHT = terrain->getSizeY();

	vec<f32>* derivative;
	vec<f32> new_value = vec<f32>(4);
	vec<f32> b_vect = vec<f32>(4);

	new_value = initial;

	for (int i = 0; i < SOLVER_ITERATION_LIMIT; i++) {

		derivative = get_derivative(terrain, new_value);
		//derivative->log();
		new_value = (new_value + ((*derivative) * delta_time));

		if (new_value.coord[0] < 0 || new_value.coord[0] >= WIDTH || 
			new_value.coord[1] < 0 || new_value.coord[1] >= HEIGHT) break;

		Point new_point = Point(new_value.coord[0], new_value.coord[1], 0);
		terrain->fillPoint(&new_point);
		values.push_back(new_value);

		if (norm(B - new_point) < SOLVER_EPSILON_VALUE) {

			b_vect.coord[0] = B.x;
			b_vect.coord[1] = B.y;
			b_vect.coord[2] = new_value.coord[2];
			b_vect.coord[3] = new_value.coord[3];

			values.push_back(b_vect);
			break;
		}

		delete derivative;
	}

	return values;
}


Path* try_solution(Terrain* terrain, Point A, Point B) {

	terrain->generateColorMap();

	Path* best_solution = nullptr;
	std::vector<vec<f32>> current_solution;
	f64 best_length = -1.0f;
	vec<f32> last_vect = vec<f32>(4);

	Path* current_path;
	
	for (f32 theta = 0; theta < 6.28f; theta += SOLVER_CIRCLE_RESOLUTION) {
		
		current_solution = RK4(terrain, A, B, cos(theta), sin(theta), SOLVER_STEP);
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