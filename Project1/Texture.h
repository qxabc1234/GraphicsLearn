#pragma once
#include "Vector4.h"

class Texture
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
	Texture(char const* filename);
	~Texture();
	Vector4 Sample(float u, float v) const;

};

