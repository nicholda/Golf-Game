#include "Vector2.h"

// allow the class to be initialised with nothing or a position
Vector2::Vector2() {
	x = 0.0f;
	y = 0.0f;
}

Vector2::Vector2(float x, float y) {
	this->x = x;
	this->y = y;
}

Vector2::~Vector2() {

}

Vector2& Vector2::add(const Vector2& vec) {
	this->x += vec.x;
	this->y += vec.y;

	return *this;
}

Vector2& Vector2::subtract(const Vector2& vec) {
	this->x -= vec.x;
	this->y -= vec.y;

	return *this;
}

Vector2& Vector2::multiply(const Vector2& vec) {
	this->x *= vec.x;
	this->y *= vec.y;

	return *this;
}

Vector2& Vector2::divide(const Vector2& vec) {
	this->x /= vec.x;
	this->y /= vec.y;

	return *this;
}

// override the +,-,*,/ operators
Vector2& operator+(Vector2& vec1, const Vector2& vec2) {
	return vec1.add(vec2);
}

Vector2& operator-(Vector2& vec1, const Vector2& vec2) {
	return vec1.subtract(vec2);
}

Vector2& operator*(Vector2& vec1, const Vector2& vec2) {
	return vec1.multiply(vec2);
}

Vector2& operator/(Vector2& vec1, const Vector2& vec2) {
	return vec1.divide(vec2);
}

// override the * operator
Vector2& Vector2::operator*(const int& val) {
	this->x *= val;
	this->y *= val;

	return *this;
}

// override the +=,-=,*=,/= operators
Vector2& Vector2::operator+=(const Vector2& vec) {
	return this->add(vec);
}

Vector2& Vector2::operator-=(const Vector2& vec) {
	return this->subtract(vec);
}

Vector2& Vector2::operator*=(const Vector2& vec) {
	return this->multiply(vec);
}

Vector2& Vector2::operator/=(const Vector2& vec) {
	return this->divide(vec);
}

// override the +=,-=,*=,/= operators
Vector2& Vector2::operator+=(const float val) {
	this->x += val;
	this->y += val;

	return *this;
}

Vector2& Vector2::operator-=(const float val) {
	this->x -= val;
	this->y -= val;

	return *this;
}

Vector2& Vector2::operator*=(const float val) {
	this->x *= val;
	this->y *= val;

	return *this;
}

Vector2& Vector2::operator/=(const float val) {
	this->x /= val;
	this->y /= val;

	return *this;
}

Vector2& Vector2::zero() {
	this->x = 0;
	this->y = 0;

	return *this;
}

// performs linear interpolation
Vector2& Vector2::lerp(const Vector2& vec1, const Vector2& vec2, float t) {
	t = std::max(0.0f, std::min(t, 1.0f));

	this->x = vec1.x + t * (vec2.x - vec1.x);
	this->y = vec1.y + t * (vec2.y - vec1.y);

	return *this;
}

// return the magnitude of the vector
float Vector2::magnitude() {
	return sqrt(pow(this->x, 2) + pow(this->y, 2));
}