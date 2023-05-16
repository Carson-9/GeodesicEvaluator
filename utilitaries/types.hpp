#pragma once
#include <stdarg.h>
#include <assert.h> 
#include <iostream>
#include <stdio.h>
#include <math.h>

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned long u32;
typedef unsigned long long u64;

typedef signed char i8;
typedef signed short i16;
typedef signed long i32;
typedef signed long long i64;

typedef float f32;
typedef double f64;
typedef long double f128;

typedef bool b8;


struct Point {
	f32 x;
	f32 y;
	f32 z;
	
	Point() {
		x = 0.0f;
		y = 0.0f;
		z = 0.0f;
	}

	Point(f32 nx, f32 ny, f32 nz)
		:x(nx), y(ny), z(nz)
	{

	}

	Point operator +(const Point& b) {
		return Point(x + b.x, y + b.y, z + b.z);
	}

	Point operator -(const Point& b) {
		return Point(x - b.x, y - b.y, z - b.z);
	}

	Point operator *(const float& b) {
		return Point(x * b, y * b, z * b);
	}

	Point operator *(const int& b) {
		return Point(x * b, y * b, z * b);
	}

	Point& operator =(const Point& b) {
		x = b.x;
		y = b.y;
		z = b.z;
		return *this;
	}

	void log() {
		printf("%f, %f, %f\n", x, y, z);
	}

	Point normalize() {
		f32 norm = sqrt(x * x + y * y + z * z);
		return Point(x / norm, y / norm, z / norm);
	}

};

f32 dot(Point a, Point b);
f32 norm(Point a);

struct pointNode {
	Point p;
	struct pointNode* next;
	struct pointNode* prev;
};

typedef struct pointNode* pointList;


void printPoint(Point* p);
pointList pointListAddHead(pointList list, Point p);
void deletePointList(pointList list);
pointList makePointList();
i16 getPointListSize(pointList list);
Point* pointListToArray(pointList list);
void copyPoint(Point* a, Point* b);

template <class T, class U>
class vect {
public:

	T* coord;
	i16 dim;

	vect() {
		coord = new T[1];
		coord[0] = T();
		dim = 0;
	}

	vect(const vect& parent) { // BY DEFAULT JUST DELETES THE ITEM, WTF
		dim = parent.dim;
		coord = new T[dim];
		for (int i = 0; i < dim; i++) coord[i] = parent.coord[i];
	}

	vect(i16 dimension) {

		coord = new T[dimension];
		dim = dimension;
	}

	vect(i16 dimension, T* coordList) {

		coord = new T[dimension]; // Copy the coordinates to not rely on external table
		dim = dimension;

		for (int item = 0; item < dimension; item++) coord[item] = coordList[item];
	}

	~vect() {
		delete[] coord;
	}

	vect& operator=(const vect& a) {

		if (a.dim != dim) {
			//printf("Vector : Cannot assign a vector with a different dimension\n");
			delete[] coord;
			coord = new T[a.dim];
			for (int i = 0; i < dim; i++) this->coord[i] = a.coord[i];
			return *this;
		}

		for (int i = 0; i < dim; i++) this->coord[i] = a.coord[i];

		return *this;
	}

	vect operator+(const vect& a) const {
		if (a.dim != dim) {
			printf("Vector : Cannot add two vectors with different dimensions\n");
			return *this;
		}

		T* new_coords = new T[dim];
		for (int i = 0; i < dim; i++) new_coords[i] = coord[i] + a.coord[i];

		vect result = vect(dim, new_coords);

		delete[] new_coords;
		return result;
	}

	vect operator-(const vect& a) const {
		if (a.dim != dim) {
			printf("Vector : Cannot subtract two vectors with different dimensions\n");
			return *this;
		}

		T* new_coords = new T[dim];
		for (int i = 0; i < dim; i++) new_coords[i] = coord[i] - a.coord[i];

		vect result = vect(dim, new_coords);

		delete[] new_coords;
		return result;
	}

	vect operator*(const U lambda) {

		T* new_coords = new T[dim];
		for (int i = 0; i < dim; i++) new_coords[i] = coord[i] * lambda;

		vect result = vect(dim, new_coords);

		delete[] new_coords;
		return result;
	}

	vect& clear(i16 dim) {
		this->dim = dim;
		delete[] this->coord;
		this->coord = new T[dim];
		return *this;
	}

	bool operator==(const vect& a) const {
		if (a.dim != dim) return false;
		for (int c = 0; c < dim; c++) {
			if (a.coord[c] != coord[c]) return false;
		}
		return true;
	}

	void log() {
		std::cout << "Logging Vector : " << std::endl;
		for (int item = 0; item < dim; item++) std::cout << item << " : " << this->coord[item] << ", ";
		std::cout << std::endl;
	}

};

template<class T, class U>
inline T dot(vect<T, U> a, vect<T, U> b) {
	assert(a.dim == b.dim);
	T result = 0;
	for (int i = 0; i < a.dim; i++) result += (a.coord[i] * b.coord[i]);
	return result;
}

template<class T, class U>
inline T norm(vect<T, U> a) {
	T result = 0;
	for (int i = 0; i < a.dim; i++) result += (a.coord[i] * a.coord[i]);
	return sqrt(result);
}


template<class T>
using vec = vect<T, T>;


class vec2f {

public:

	f32 x;
	f32 y;

	vec2f(f32 nx = 0.0f, f32 ny = 0.0f) {
		x = nx;
		y = ny;
	}

	vec2f& operator=(const vec2f& a) {
		x = a.x;
		y = a.y;
		return *this;
	}

	vec2f operator+(const vec2f& a) const {
		return vec2f(x + a.x, y + a.y);
	}

	vec2f operator-(const vec2f& a) const {
		return vec2f(x - a.x, y - a.y);
	}

	vec2f operator*(const f32 lambda) {
		return vec2f(x * lambda, y * lambda);
	}

	bool operator==(const vec2f& a) const {
		return (x == a.x && y == a.y);
	}

	vec2f& rotate_90(void) {
		f32 temp = x;
		x = -y;
		y = temp;
		return *this;
	}

	void log() {
		printf("Logging vector : x = %f, y = %f\n", x, y);
	}

	vec<float> to_vect() const {
		f32 args[] = { x, y };
		return vect<float, float>(2, args);
	}

};

f32 dot(vec2f a, vec2f b);


class vec3f {

public:

	f32 x;
	f32 y;
	f32 z;

	vec3f(f32 nx = 0.0f, f32 ny = 0.0f, f32 nz = 0.0f) {
		x = nx;
		y = ny;
		z = nz;
	}

	vec3f& operator=(const vec3f& a) {
		x = a.x;
		y = a.y;
		z = a.z;
		return *this;
	}

	vec3f operator+(const vec3f& a) const {
		return vec3f(x + a.x, y + a.y, z + a.z);
	}

	vec3f operator-(const vec3f& a) const {
		return vec3f(x - a.x, y - a.y, z - a.z);
	}

	vec3f operator*(const f32 lambda) {
		return vec3f(x * lambda, y * lambda, z * lambda);
	}

	bool operator==(const vec3f& a) const {
		return (x == a.x && y == a.y && z == a.z);
	}

	void log() {
		printf("Logging vector : x = %f, y = %f, z = %f\n", x, y, z);
	}

	vec<float> to_vect() const {
		f32 args[] = { x, y, z };
		return vect<float, float>(3, args);
	}

};

f32 dot(vec3f a, vec3f b);
vec3f cross(vec3f a, vec3f b);


template <class T, class U>
class matrix {
public:

	i16 row_num;
	i16 col_num;
	T** mat;

	matrix() {

	}

	matrix(i16 row_dimension, i16 col_dimension) {
		row_num = row_dimension;
		col_num = col_dimension;

		this->mat = new T * [row_dimension];
		for (int i = 0; i < row_dimension; i++) this->mat[i] = new T[col_dimension];
	}

	matrix(i16 dimension) {
		row_num = dimension;
		col_num = dimension;

		this->mat = new T * [dimension];
		for (int i = 0; i < dimension; i++) this->mat[i] = new T[dimension];
	}

	matrix(i16 row, i16 col, T** coordList) {

		this->mat = new T * [row]; // Copy the coordinates to not rely on external table
		for (int i = 0; i < row; i++) this->mat[i] = new T[col];


		row_num = row;
		col_num = col;

		for (int r = 0; r < row; r++) {
			for (int c = 0; c < col; c++) this->mat[r][c] = coordList[r][c];
		}
	}

	matrix(i16 dim, T** coordList) {

		this->mat = new T * [dim]; // Copy the coordinates to not rely on external table
		for (int i = 0; i < dim; i++) this->mat[i] = new T[dim];

		row_num = dim;
		col_num = dim;

		for (int r = 0; r < dim; r++) {
			for (int c = 0; c < dim; c++) this->mat[r][c] = coordList[r][c];
		}
	}

	~matrix() {
		for (int r = 0; r < row_num; r++) delete[] this->mat[r];

		delete[] this->mat;
	}

	matrix& operator=(const matrix& a) {

		if (a.row_num != row_num || a.col_num != col_num) {
			printf("Matrix : Cannot assign a matrix with different dimension(s)\n");
			return *this;
		}

		for (int row = 0; row < row_num; row++) {
			for (int col = 0; col < col_num; col++) mat[row][col] = a.mat[row][col];
		}

		return *this;
	}

	matrix operator+(const matrix& a) const {
		if (a.row_num != row_num || a.col_num != col_num) {
			printf("Matrix : Cannot assign a matrix with different dimension(s)\n");
			return this;
		}

		T** new_coords = new T * [row_num];
		for (int i = 0; i < row_num; i++) mat[i] = new T[col_num];

		for (int r = 0; r < row_num; r++) {
			for (int c = 0; c < col_num; c++) new_coords[r][c] = mat[r][c] + a.mat[r][c];
		}

		matrix result = matrix(row_num, col_num, new_coords);

		for (int r = 0; r < row_num; r++) delete[] new_coords[r];
		delete[] new_coords;

		return result;
	}

	matrix operator-(const matrix& a) const {
		if (a.row_num != row_num || a.col_num != col_num) {
			printf("Matrix : Cannot assign a matrix with different dimension(s)\n");
			return this;
		}

		T** new_coords = new T[row_num][col_num];

		for (int r = 0; r < row_num; r++) {
			for (int c = 0; c < col_num; c++) new_coords[r][c] = mat[r][c] - a.mat[r][c];
		}

		matrix result = matrix(row_num, col_num, new_coords);

		for (int r = 0; r < row_num; r++) delete[] new_coords[r];
		delete[] new_coords;

		return result;
	}

	matrix& operator*(const U lambda) {
		for (int r = 0; r < row_num; r++) {
			for (int c = 0; c < col_num; c++) mat[r][c] *= lambda;
		}
		return *this;
	}

	matrix operator*(const matrix& a) const {

		// Naïve matrix multiplication algorithm

		// TODO : Implement a better version if possible

		assert(col_num == a.row_num);
		T** new_coords = new T * [row_num];
		for (int i = 0; i < row_num; i++) mat[i] = new T[a.col_num];

		for (int r = 0; r < row_num; r++) {
			for (int c = 0; c < a.col_num; c++) {
				for (int item = 0; item < col_num; item++) new_coords[r][c] += mat[r][item] + a.mat[item][c];
			}
		}

		matrix result = matrix(row_num, a.col_num, new_coords);

		for (int r = 0; r < row_num; r++) delete[] new_coords[r];
		delete[] new_coords;

		return result;
	}

	matrix& transpose() {

		T** new_mat = new T * [col_num];
		for (int c = 0; c < col_num; c++) new_mat[c] = new T[row_num];

		for (int r = 0; r < row_num; r++) {
			for (int c = 0; c < col_num; c++) new_mat[c][r] = mat[r][c];
		}

		i16 temp = row_num;
		this->row_num = col_num;
		this->col_num = temp;

		for (int r = 0; r < col_num; r++) delete[] this->mat[r];
		delete[] this->mat;
		this->mat = new_mat;
		return *this;

	}


	bool operator==(const matrix& a) const {
		if (row_num != a.row_num || col_num != a.col_num) return false;

		for (int r = 0; r < row_num; r++) {
			for (int c = 0; c < a.col_num; c++) {
				if (mat[r][c] != a.mat[r][c]) return false;
			}
		}

		return true;
	}

	bool is_square() const {
		return row_num == col_num;
	}

	vec2f operator*(const vec2f& v) const {
		assert(col_num == 2 && this->is_square());

		return vec2f(this->mat[0][0] * v.x + this->mat[0][1] * v.y,
			this->mat[1][0] * v.x + this->mat[1][1] * v.y);
	}

	vec3f operator*(const vec3f& v) const {
		assert(this->col_num == 3 && this->is_square());

		return vec3f(this->mat[0][0] * v.x + this->mat[0][1] * v.y + this->mat[0][2] * v.z,
			this->mat[1][0] * v.x + this->mat[1][1] * v.y + this->mat[1][2] * v.z,
			this->mat[2][0] * v.x + this->mat[2][1] * v.y + this->mat[2][2] * v.z);
	}

	vect<T, U> operator*(const vect<T, U>& v) const {
		assert(this->col_num == v.dim);

		T* coords = new T[this->row_num];

		for (int r = 0; r < this->row_num; r++) {
			coords[r] = 0;
			for (int c = 0; c < this->col_num; c++) {
				coords[r] += this->mat[r][c] * v.coord[c];
			}
		}

		vect<T, U> res = vect<T, U>(this->row_num, coords);
		delete[] coords;
		return res;

	}

	/// <summary>
	/// Creates the submatrix where row and col have been removed	
	/// </summary>
	/// <param name="row">The index !(starts at 1) of the row to remove</param>
	/// <param name="col">The index !(starts at 1) of the column to remove</param>
	/// <returns>A pointer to the newly created submatrix</returns>
	matrix* submatrix(i16 row, i16 col) {

		// Returns the sub matrix, where row[row] and col[col] are missing 
		// (indexed from 1 : 1 <= row <= row_num)

		assert(row_num >= 2 && col_num >= 2
			&& row <= row_num && col <= col_num
			&& row >= 1 && col >= 1);

		matrix* sub = new matrix(row_num - 1, col_num - 1);
		//printf("Sub : %d x %d\n", sub->row_num, sub->col_num);

		i16 proper_row = row - 1;
		i16 proper_col = col - 1;

		for (int r = 0; r < proper_row; r++) {
			for (int c = 0; c < proper_col; c++) sub->mat[r][c] = this->mat[r][c];
			for (int c = proper_col; c < col_num - 1; c++) sub->mat[r][c] = this->mat[r][c + 1];
		}

		for (int r = proper_row; r < row_num - 1; r++) {
			for (int c = 0; c < proper_col; c++) sub->mat[r][c] = this->mat[r + 1][c];
			for (int c = proper_col; c < col_num - 1; c++) sub->mat[r][c] = this->mat[r + 1][c + 1];
		}

		return sub;
	}


	void log() {
		for (int r = 0; r < row_num; r++) {
			for (int c = 0; c < col_num; c++) {
				std::cout << " | " << mat[r][c];
			}
			std::cout << " |" << std::endl;
		}
		std::cout << std::endl;
	}

};

/// <summary>
/// Returns the determinant of the matrix
/// </summary>
/// <typeparam name="T"></typeparam>
/// <param name="m"> : Matrix whose determinant must be calculated</param>
/// <returns>The determinant of said matrix</returns>
template <class T, class U>
U determinant(matrix<T, U>* m) {
	// Naïve determinant calculation (Leibniz formula)
	// TODO : Find a better algorithm

	assert(m->row_num == m->col_num);
	if (m->row_num == 1) return m->mat[0][0];
	if (m->row_num == 2) return m->mat[0][0] * m->mat[1][1] - m->mat[0][1] * m->mat[1][0];

	T det = 0;
	i8 sign = 1;


	for (int r = 0; r < m->row_num; r++) {
		det += sign * m->mat[0][r] * determinant(m->submatrix(1, r + 1));
		sign *= -1;
	}

	return det;
}

template<class T>
using Matrix = matrix<T, T>;


/*
template<class T>
struct linked_list {
	T data;
	linked_list<T>* next;
	linked_list<T>* previous;


	linked_list() {
		data = T();
		next = nullptr;
		previous = nullptr;
	}

	linked_list(T init)
	{
		next = nullptr;
		previous = nullptr;
	}

	~linked_list() {
		if (next == nullptr) {
			delete this;
			return;
		}

		if (previous != nullptr) delete previous;
		next->~linked_list();
	}

	linked_list& operator+(const T& d) {
		linked_list* ptr = this;
		while (ptr->next != nullptr) ptr = ptr->next;
		ptr->next = new linked_list(d);
		ptr->next->previous = ptr;

		return *this;
	}

	T get_i(i16 index) {
		if (index <= 0) return data;
		if (next == nullptr) {
			printf("Trying to read after the list!\n");
			return T();
		}
		next->get_i(index - 1);
	}

	T get_last_el() {
		if (next == nullptr) return data;
		return next->get_last_el();
	}

	void insert(const T& d) {
		if (next != nullptr) next->insert(d);
		next = new linked_list(d);
		next->previous = this;
	}

	i16 get_size() {
		if (next == nullptr) return 1;
		return 1 + next->get_size();
	}

	void log() {
		std::cout << " -> " << data;
		if(next != nullptr) next->log();
	}
};

*/