#pragma once
#include "utilLib.hpp"
#include "types.hpp"

#include <assert.h>


template <class T>
struct Polynomial {
	i16 degree;
	T* coefficients;

	// Coefficients are upside-down for Horner : X^n + X^n-1 + ... + X^0
	// Degree 0 results in a constant polynomial

	Polynomial() {
		degree = 0;
		coefficients = new T[1];
		coefficients[0] = T();
	}

	Polynomial(i16 deg)
		:degree(deg)
	{
		coefficients = new T[degree + 1];
		for (int i = 0; i < degree + 1; i++) coefficients[i] = T();
	}

	/// <summary>
	/// Initializes a Polynomial with given coefficients. COEFFICIENTS ARE REVERSED
	/// THE FIRST SHOULD BE THE ONE OF HIGHEST DEGREE
	/// </summary>
	/// <param name="deg">: The intended degree of the polynomial</param>
	/// <param name="coeff">: The list of its coefficients</param>
	Polynomial(i16 deg, T* coeff)
		:degree(deg)
	{
		coefficients = new T[deg + 1];
		for (int i = 0; i < deg + 1; i++) coefficients[i] = coeff[i];
	}


	Polynomial& setCoeff(i16 deg_to_modify, T coeff) {
		assert(deg_to_modify <= degree && deg_to_modify >= 0);
		coefficients[degree - deg_to_modify] = coeff;
		return *this;
	}

	T evaluate(f32 t) {
		T res = coefficients[0];
		for (int i = 1; i < degree + 1; i++) res = (res * t) + coefficients[i];
		return res;
	}

};


struct bezier3 {
	Point A;
	Point B;
	Point Control_A;
	Point Control_B;

	Polynomial<Point> polynomial;

	bezier3() {
		A = Point();
		B = Point();
		Control_A = Point();
		Control_A = Point();
		polynomial = Polynomial<Point>(3);
	}

	bezier3(Point begin, Point end)
		:A(begin), B(end)
	{
		Control_A = Point();
		Control_B = Point();
		polynomial = Polynomial<Point>(3);
	}

	bezier3(Point begin, Point end, Point control1, Point control2)
		:A(begin), B(end), Control_A(control1), Control_B(control2)
	{
		polynomial = Polynomial<Point>(3);
		polynomial.setCoeff(0, begin);
		polynomial.setCoeff(1, (begin * (-3)) + (control1 * 3));
		polynomial.setCoeff(2, (begin * 3) + (control1 * (-6)) + (control2 * 3));
		polynomial.setCoeff(3, (begin * (-1)) + (control1 * 3) + (control2 * (-3)) + end);
	}

	void updatePolynomial() {
		polynomial.setCoeff(0, A);
		polynomial.setCoeff(1, (A * (-3)) + (Control_A * 3));
		polynomial.setCoeff(2, (A * 3) + (Control_A * (-6)) + (Control_B * 3));
		polynomial.setCoeff(3, (A * (-1)) + (Control_A * 3) + (Control_B * (-3)) + B);
	}

	Point evaluate(f32 t) {
		assert(t >= 0 && t <= 1);
		return polynomial.evaluate(t);
	}

};


struct bezier_spline {
	i16 order;
	bezier3* curves;

	bezier_spline() {
		order = 0;
		curves = new bezier3[1];
	}

	bezier_spline(i16 o)
		:order(o)
	{
		curves = new bezier3[o];
	}

	bezier_spline(i16 o, bezier3* list)
		:order(o)
	{
		curves = new bezier3[o];
		for (int i = 0; i < 0; i++) curves[i] = list[i];
	}

	void set_curve(i16 index, bezier3 curve) {
		assert(index >= 0 && index <= order - 1);
		curves[index] = curve;
	}

};