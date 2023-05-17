#pragma once

#include "terrain/terrain.hpp"
#include "utilitaries/types.hpp"
#include "Path/Path.hpp"

#define FRECHET_EPSILON 50


i32 find_index_with_parameter_length(Path* p, f64 dist);
f64 computeFlattenedLength(Path* p);
bool are_path_close(Path* a, Path* b, f32 delta_distance);