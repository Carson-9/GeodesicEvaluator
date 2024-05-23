#pragma once
#include <math.h>
#include "types.hpp"


#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))
#define CLAMP(value, min, max) (MIN((MAX(value, min)), max))

float cheapExp(float x);
float sigmoid(float x, float max, float midpoint, float steepness);
void SWAP(float* a, float* b);
void SWAP(int* a, int* b);

// while(0) is a dirty hack to include multiple statements (a block)
//#define SWAP(x, y) (do { typeof(x) SWAP = x; x = y; y = SWAP; } while (0))


/// <summary>
/// Copies an array into another
/// </summary>
/// <typeparam name="T"> : Data type</typeparam>
/// <param name="a"> : Array to copy </param>
/// <param name="b"> : Array to receive</param>
/// <param name="size"> : Number of items to copy</param>
template <typename T>
void copy_array(T* a, T* b, i32 size) {
	for (int i = 0; i < size; i++) b[i] = a[i];
}