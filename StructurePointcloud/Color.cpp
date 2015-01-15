////////////////////////////////////////////////////////////////////////////////////
// Copyright © Charalambos "Charis" Poullis, charalambos@poullis.org    	  //
// This work can only be used under an exclusive license of the author.           //
////////////////////////////////////////////////////////////////////////////////////
#ifndef __COLOR_CPP__
#define __COLOR_CPP__

#include "Color.h"

Color::Color() {
	data(0) = 0.0f;
	data(1) = 0.0f;
	data(2) = 0.0f;
	data(3) = 1.0f;
}

Color::Color(Color const &_color) {
	data(0) = _color.r();
	data(1) = _color.g();
	data(2) = _color.b();
	data(3) = _color.a();
}

Color::Color(float _grayscale) {
	data(0) = _grayscale;
	data(1) = _grayscale;
	data(2) = _grayscale;
	data(3) = 1.0f;
}

Color::Color(float _r, float _g, float _b) {
	data(0) = _r;
	data(1) = _g;
	data(2) = _b;
	data(3) = 1.0f;
}

Color::Color(float _r, float _g, float _b, float _a) {
	data(0) = _r;
	data(1) = _g;
	data(2) = _b;
	data(3) = _a;
}

Color::~Color() {
}

float Color::r() const {
	return data(0);
}

float &Color::r() {
	return data(0);
}

float Color::g() const {
	return data(1);
}

float &Color::g() {
	return data(1);
}

float Color::b() const {
	return data(2);
}

float &Color::b() {
	return data(2);
}

float Color::a() const {
	return data(3);
}

float &Color::a() {
	return data(3);
}

float& Color::operator()(int elementNumber) {
	return data(elementNumber);
}

float Color::operator()(int elementNumber) const {
	return data(elementNumber);
}

Color operator*(Color const &c1, Color const &c2) {
	Color result;
	for (int i = 0; i < 3; i++)
		result(i) = c1(i) * c2(i);
	return result;
}

Color operator*(Color const &c1, float scalar) {
	Color result;
	for (int i = 0; i < 3; i++)
		result(i) = c1(i) * scalar;
	return result;
}

Color operator*(float scalar, Color const &c1) {
	Color result;
	for (int i = 0; i < 3; i++)
		result(i) = c1(i) * scalar;
	return result;
}

Color operator+(Color const &c1, Color const &c2) {
	Color result;
	for (int i = 0; i < 3; i++)
		result(i) = c1(i) + c2(i);
	return result;
}

Color operator-(Color const &c1, Color const &c2) {
	Color result;
	for (int i = 0; i < 3; i++)
		result(i) = c1(i) - c2(i);
	return result;
}

Color operator/(Color const &c1, Color const &c2) {
	Color result;
	for (int i = 0; i < 3; i++)
		result(i) = c1(i) / std::max(float(COLOR_EPSILON), c2(i));
	return result;
}

Color operator/(Color const &c1, float scalar) {
	Color result;
	for (int i = 0; i < 3; i++)
		result(i) = c1(i) / std::max(float(COLOR_EPSILON), scalar);
	return result;
}

bool operator==(Color const &c1, Color const &c2) {
	for (int i = 0; i < 3; i++)
		if (fabs(c1(i) - c2(i)) > COLOR_EPSILON)
			return false;
	return true;
}

bool operator!=(Color const &c1, Color const &c2) {
	for (int i = 0; i < 3; i++)
		if (fabs(c1(i) - c2(i)) > COLOR_EPSILON)
			return true;
	return false;
}

Color &Color::operator+=(Color const &vec) {
	for (int i = 0; i < 3; i++) {
		data(i) += vec.data(i);
	}
	return *this;
}

Color &Color::operator*=(float scalar) {
	for (int i = 0; i < 3; i++) {
		data(i) *= scalar;
	}
	return *this;
}

Color &Color::operator/=(float scalar) {
	for (int i = 0; i < 3; i++) {
		data(i) /= scalar;
	}
	return *this;
}

#endif
