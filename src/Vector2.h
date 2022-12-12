#pragma once

#include <iostream>

class Vector2 {
public:
	float x;
	float y;

	Vector2();
	Vector2(float x, float y);

	~Vector2();

	Vector2& Add(const Vector2& vec);
	Vector2& Subtract(const Vector2& vec);
	Vector2& Multiply(const Vector2& vec);
	Vector2& Divide(const Vector2& vec);

	friend Vector2& operator+(Vector2& vec1, const Vector2& vec2);
	friend Vector2& operator-(Vector2& vec1, const Vector2& vec2);
	friend Vector2& operator*(Vector2& vec1, const Vector2& vec2);
	friend Vector2& operator/(Vector2& vec1, const Vector2& vec2);

	Vector2& operator*(const int& val);

	Vector2& operator+=(const Vector2& vec);
	Vector2& operator-=(const Vector2& vec);
	Vector2& operator*=(const Vector2& vec);
	Vector2& operator/=(const Vector2& vec);

	Vector2& operator+=(const float val);
	Vector2& operator-=(const float val);
	Vector2& operator*=(const float val);
	Vector2& operator/=(const float val);

	Vector2& Zero();

	Vector2& lerp(const Vector2& vec1, const Vector2& vec2, float t);
};