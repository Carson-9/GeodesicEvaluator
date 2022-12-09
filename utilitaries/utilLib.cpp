#include "utilitaries/utilLib.hpp"

float cheapExp(float x) {
	return (1 + x + (x * x) / 2.0f + (x * x * x) / 6.0f + (x * x * x * x) / 24.0f + (x * x * x * x * x) / 120.0f);
}

float sigmoid(float x, float max, float midpoint, float steepness) {
	return (float)(max / (1 + exp(-steepness * (x - midpoint))));
}