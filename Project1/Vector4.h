#pragma once
#include <glm/glm.hpp>

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

	Vector4(const glm::vec3& v, float w = 0)
		:x(v.x), y(v.y), z(v.z), w(w)
	{
	}

	Vector4(const glm::vec2& v, float z = 0, float w = 0)
		:x(v.x), y(v.y), z(z), w(w)
	{
	}

	Vector4 operator()(float xyzw[4])
	{
		return Vector4(xyzw[0], xyzw[1], xyzw[2], xyzw[3]);
	}

	float length() 
	{
		return std::sqrt(x * x + y * y + z * z + w * w);
	};
	friend Vector4 operator*(float a, const Vector4& vec);

	Vector4 operator*(float a) const;
	Vector4 operator/(float a) const;
	Vector4 operator-(float a) const;

	Vector4 operator+(const Vector4& other) const;
	float operator*(const Vector4& other) const;
	Vector4 operator-(const Vector4& other) const;


	Vector4 Normalize();

	Vector4 Cross(const Vector4& other) const;
	Vector4 ComponentWiseMultiply(const Vector4& other) const;
};

