#pragma once

#include <iostream>
#include <cmath>

class Vector2 {
public:
	float x;
	float y;

	// allow the class to be initialised with nothing or a position
	Vector2();
	Vector2(float x, float y);

	~Vector2();

	Vector2& add(const Vector2& vec);
	Vector2& subtract(const Vector2& vec);
	Vector2& multiply(const Vector2& vec);
	Vector2& divide(const Vector2& vec);

	// override the +,-,*,/ operators
	friend Vector2& operator+(Vector2& vec1, const Vector2& vec2);
	friend Vector2& operator-(Vector2& vec1, const Vector2& vec2);
	friend Vector2& operator*(Vector2& vec1, const Vector2& vec2);
	friend Vector2& operator/(Vector2& vec1, const Vector2& vec2);

	// override the * operator
	Vector2& operator*(const int& val);
	
	// override the +=,-=,*=,/= operators
	Vector2& operator+=(const Vector2& vec);
	Vector2& operator-=(const Vector2& vec);
	Vector2& operator*=(const Vector2& vec);
	Vector2& operator/=(const Vector2& vec);

	// override the +=,-=,*=,/= operators
	Vector2& operator+=(const float val);
	Vector2& operator-=(const float val);
	Vector2& operator*=(const float val);
	Vector2& operator/=(const float val);

	Vector2& zero();

	// performs linear interpolation
	Vector2& lerp(const Vector2& vec1, const Vector2& vec2, float t);

	// return the magnitude of the vector
	float magnitude();
};