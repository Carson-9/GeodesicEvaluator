#pragma once
#include <vector>
#include "terrain/terrain.hpp"
#include "utilitaries/types.hpp"
#include "Path/Path.hpp"

#define SOLVER_DERIVATIVE_PRECISION 0.01f
#define SOLVER_STEP 0.1f
#define SOLVER_ITERATION_LIMIT 10000
#define SOLVER_EPSILON_VALUE 7
#define SOLVER_CIRCLE_RESOLUTION 0.09f // DELTA THETA

Point terrain_du(Terrain* terrain, Point p);
Point terrain_dv(Terrain* terrain, Point p);

Point terrain_duu(Terrain* terrain, Point p);
Point terrain_dvu(Terrain* terrain, Point p);
Point terrain_dvv(Terrain* terrain, Point p);

Path* linked_list_to_path(std::vector<vec<f32>> l, Terrain* terrain);

vec<f32>* get_derivative(Terrain* terrain, const vec<f32>& current_value);

std::vector<vec<f32>> RK4(Terrain* terrain, Point A, Point B, f32 initial_u_speed, f32 initial_v_speed, f32 delta_time);
std::vector<vec<f32>> solve_for_boundaries(Terrain* terrain, Point A, Point B, f32 initial_u_speed, f32 initial_v_speed, f32 delta_time);

Path* try_solution(Terrain* terrain, Point A, Point B);