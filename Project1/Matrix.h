#pragma once

#include "Vector4.h"

class Matrix
{
private:
	union
	{
		Vector4 v[4];
		float m[4][4];
	};

public:
	Matrix()
	{

	}

	Matrix(const Vector4& r0, const Vector4& r1, const Vector4& r2, const Vector4& r3)
	{
		v[0] = r0;
		v[1] = r1;
		v[2] = r2;
		v[3] = r3;
	}

	Matrix operator*(const Matrix& other) const;
	Matrix operator*(float x) const;
	Vector4 operator*(const Vector4& v) const;
	Matrix operator+(const Matrix& other) const;
	static Matrix Translate(float x, float y, float z);
	static Matrix Scale(float x, float y, float z);
	static Matrix Rotate(float degree, float x, float y, float z);
	static Matrix View(Vector4 pos, Vector4 target, Vector4 up);
	static Matrix Ortho(float l, float r, float t, float b, float n, float f);
	static Matrix Persp(float fov, float n, float f, float ratio);

};

