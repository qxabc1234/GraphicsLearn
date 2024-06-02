#pragma once
#include "Vector4.h"

class Texture2D
{
private:
	int width, height;
	unsigned char* imagedata;

public:
	int GetWidth()
	{
		return width;
	}
	int GetHeight()
	{
		return height;
	}
	Texture2D(char const* filename);
	~Texture2D();
	Vector4 Sample(float u, float v) const;

};

