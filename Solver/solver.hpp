#pragma once
#include <vector>
#include <cmath>
#include "../terrain/terrain.hpp"
#include "../utilitaries/types.hpp"
#include "../Path/Path.hpp"

#define SOLVER_DERIVATIVE_PRECISION 0.01f
#define SOLVER_STEP 0.1f
#define SOLVER_ITERATION_LIMIT 20000
#define SOLVER_EPSILON_VALUE 35
#define SOLVER_CIRCLE_RESOLUTION 0.005f // DELTA THETA
#define SOLVER_BREAK 1000.0f

Point terrain_du(Terrain* terrain, Point p);
Point terrain_dv(Terrain* terrain, Point p);

Point terrain_duu(Terrain* terrain, Point p);
Point terrain_dvu(Terrain* terrain, Point p);
Point terrain_dvv(Terrain* terrain, Point p);

Path* linked_list_to_path(std::vector<vec<f64>> l, Terrain* terrain);

vec<f64>* get_derivative(Terrain* terrain, const vec<f64>& current_value);

std::vector<vec<f64>> RK4(Terrain* terrain, Point A, Point B, f64 initial_u_speed, f64 initial_v_speed, f64 delta_time);

Path* try_solution(Terrain* terrain, Point A, Point B);