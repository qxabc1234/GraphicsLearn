#pragma once
class Vector4
{
public:
	union {
		struct {
			float x, y, z, w;
		};
		struct {
			float r, g, b, a;
		};
	};

public:
	Vector4()
	{
		x = y = z = w = 0;
	}


	Vector4(float x, float y, float z, float w)
		:x(x), y(y), z(z), w(w)
	{
	}

	Vector4 operator()(float xyzw[4])
	{
		return Vector4(xyzw[0], xyzw[1], xyzw[2], xyzw[3]);
	}

	Vector4 operator*(float a) const;
	Vector4 operator/(float a) const;
	Vector4 operator+(const Vector4& other) const;
	float operator*(const Vector4& other) const;


	Vector4 Normalize();

	Vector4 Cross(const Vector4& other) const;

};

