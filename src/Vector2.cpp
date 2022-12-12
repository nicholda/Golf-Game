#include "Vector2.h"

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

Vector2& Vector2::Add(const Vector2& vec) {
	this->x += vec.x;
	this->y += vec.y;

	return *this;
}

Vector2& Vector2::Subtract(const Vector2& vec) {
	this->x -= vec.x;
	this->y -= vec.y;

	return *this;
}

Vector2& Vector2::Multiply(const Vector2& vec) {
	this->x *= vec.x;
	this->y *= vec.y;

	return *this;
}

Vector2& Vector2::Divide(const Vector2& vec) {
	this->x /= vec.x;
	this->y /= vec.y;

	return *this;
}

Vector2& operator+(Vector2& vec1, const Vector2& vec2) {
	return vec1.Add(vec2);
}

Vector2& operator-(Vector2& vec1, const Vector2& vec2) {
	return vec1.Subtract(vec2);
}

Vector2& operator*(Vector2& vec1, const Vector2& vec2) {
	return vec1.Multiply(vec2);
}

Vector2& operator/(Vector2& vec1, const Vector2& vec2) {
	return vec1.Divide(vec2);
}

Vector2& Vector2::operator*(const int& val) {
	this->x *= val;
	this->y *= val;

	return *this;
}

Vector2& Vector2::operator+=(const Vector2& vec) {
	return this->Add(vec);
}

Vector2& Vector2::operator-=(const Vector2& vec) {
	return this->Subtract(vec);
}

Vector2& Vector2::operator*=(const Vector2& vec) {
	return this->Multiply(vec);
}

Vector2& Vector2::operator/=(const Vector2& vec) {
	return this->Divide(vec);
}

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

Vector2& Vector2::Zero() {
	this->x = 0;
	this->y = 0;

	return *this;
}

Vector2& Vector2::lerp(const Vector2& vec1, const Vector2& vec2, float t) {
	this->x = vec1.x + t * (vec2.x - vec1.x);
	this->y = vec1.y + t * (vec2.y - vec1.y);

	return *this;
}