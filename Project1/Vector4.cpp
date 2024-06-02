#include "Vector4.h"
#include <cmath>

Vector4 Vector4::operator*(float a) const
{
	return Vector4(a * x, a * y , a * z, w * a);
}

Vector4 Vector4::operator/(float a) const
{
	return Vector4(x / a, y / a, z / a, w / a);
}

Vector4 Vector4::operator+(const Vector4& other) const
{
	return Vector4(x + other.x, y + other.y, z + other.z, w + other.w);
}

float Vector4::operator*(const Vector4& other) const
{
	return x * other.x + y * other.y + z * other.z + w * other.w;
}

Vector4 Vector4::operator-(const Vector4& other) const
{
	return Vector4(x - other.x, y - other.y, z - other.z, w - other.w);
}

Vector4 Vector4::Normalize()
{
	float len = std::sqrt(x * x + y * y + z * z + w * w);
	if (len < 1e-6f) len = 1;	
	return Vector4(x/len, y/len, z/len, w/len);
}

Vector4 Vector4::Cross(const Vector4& other) const
{

	return Vector4(x * other.z - z * other.y, z * other.x - x * other.z, x * other.y - y * other.x, 0.0);
}

Vector4 Vector4::ComponentWiseMultiply(const Vector4& other) const
{
	return Vector4(x * other.x, y * other.y, z * other.z, w * other.w);
}

Vector4 operator*(float a, const Vector4& vec)
{
	return vec * a;
}
