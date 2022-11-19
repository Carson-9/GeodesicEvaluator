#pragma once
#include <iostream>
#include <math.h>

template <typename T>

class Vector3 {

	public:
		T x;
		T y;
		T z;

		Vector3(T x = 0, T y = 0, T z = 0) {
			this->x = x;
			this->y = y;
			this->z = z;
		}

		void Normalize();

		inline T MagnitudeSquared() {
			return ((this->x * this->x) + (this->y * this->y) + (this->z * this->z));
		}

		inline Vector3<T> operator + (Vector3<T> const& obj) {
			return Vector3<T>(this->x + obj.x, this->y + obj.y, this->z + obj.z);
		}

		inline Vector3<T> operator - (Vector3<T> const& obj) {
			return Vector3<T>(this->x - obj.x, this->y - obj.y, this->z - obj.z);
		}

		void operator *(T const& obj);

		inline T operator * (Vector3<T> const& obj) {
			return (this->x * obj.x + this->y * obj.y + this->z * obj.z);
		}

		inline Vector3<T> operator ^ (Vector3<T> const& obj) {
			return Vector3<T>(this->y * obj.z - this->z * obj.y, this->z * obj.x - this->x * obj.z, this->x * obj.y - this->y * obj.x);
		}

		inline void log() {
			std::cout << "Vect : x = " << this->x << ", y = " << this->y << ", z = " << this->z << std::endl;
		}

	private:

};


template <typename T>
void Vector3<T>::Normalize() {
	T mag = sqrt(this->MagnitudeSquared());
	this->x = (this->x / mag);
	this->y = (this->y / mag);
	this->z = (this->z / mag);
}

template <typename T>
void Vector3<T>::operator*(T const& obj) {
	this->x *= obj;
	this->y *= obj;
	this->z *= obj;
}

template <typename T>
class Vector2 {

public:
	T x;
	T y;

	Vector2(T x = 0, T y = 0, T z = 0) {
		this->x = x;
		this->y = y;
	}

	void Normalize();

	inline T MagnitudeSquared() {
		return ((this->x * this->x) + (this->y * this->y));
	}

	inline Vector2<T> operator + (Vector2<T> const& obj) {
		return Vector2<T>(this->x + obj.x, this->y + obj.y);
	}

	inline Vector2<T> operator - (Vector2<T> const& obj) {
		return Vector2<T>(this->x - obj.x, this->y - obj.y);
	}

	void operator *(T const& obj);

	inline T operator * (Vector2<T> const& obj) {
		return (this->x * obj.x + this->y * obj.y);
	}

	inline void log() {
		std::cout << "Vect : x = " << this->x << ", y = " << this->y << std::endl;
	}

private:

};


template <typename T>
void Vector2<T>::Normalize() {
	T mag = sqrt(this->MagnitudeSquared());
	this->x = (this->x / mag);
	this->y = (this->y / mag);
}

template <typename T>
void Vector2<T>::operator*(T const& obj) {
	this->x *= obj;
	this->y *= obj;
}


template <class T>
class Matrix {
	public:

		Matrix(int size) {
			this->size = size;
			this->matrix = new T * [size];
			for (int i = 0; i < size; i++) this->matrix[i] = new T[size];
		}

		void Destroy();

		inline int getSize() { return this->size; }
		inline T getElement(int col, int row) { return this->matrix[row][col]; }
		inline void setElement(int col, int row, T e) { this->matrix[row][col] = e; }

		Matrix<T> operator+(const Matrix<T>& obj);
		Matrix<T> operator-(const Matrix<T>& obj);
		Matrix<T> operator*(const T& obj);
		//Matrix<T> operator*(const Matrix<T>& obj);


	private:
		int size;
		T** matrix;
};

class MatrixWrongSize : public std::exception {
	public:
		const char* what() {
			return "Matrices are not of the same size";
		}

};

template <class T>
void Matrix<T>::Destroy() {
	for (int i = 0; i < this->size; i++) delete this->matrix[i];
	delete this->matrix;
}

template <class T>
Matrix<T> Matrix<T>::operator+(const Matrix<T> &obj) {

	int selfSize = this->getSize();

	try { if (selfSize != obj->getSize()) throw(MatrixWrongSize); }

	catch (MatrixWrongSize& e) { printf("Error, cannot add matrices, these do not have the same size"); }
	
	for (int i = 0; i < selfSize; i++)
		for (int j = 0; j < selfSize; j++)
			this->setElement(i, j, this->getElement(i, j) + obj->getElement(i, j));
}

template <class T>
Matrix<T> Matrix<T>::operator-(const Matrix<T> &obj) {

	int selfSize = this->getSize();

	try { if (selfSize != obj->getSize()) throw(MatrixWrongSize); }

	catch (MatrixWrongSize& e) { printf("Error, cannot add matrices, these do not have the same size"); }

	for (int i = 0; i < selfSize; i++)
		for (int j = 0; j < selfSize; j++)
			this->setElement(i, j, this->getElement(i, j) - obj->getElement(i, j));
}

template <class T>
Matrix<T> Matrix<T>::operator*(const T &obj) {

	int selfSize = this->getSize();

	for (int i = 0; i < selfSize; i++)
		for (int j = 0; j < selfSize; j++)
			this->setElement(i, j, this->getElement(i, j) * obj);
}

