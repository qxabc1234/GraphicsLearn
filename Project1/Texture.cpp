#include "Texture.h"
#include <stb_image.h>

Texture::Texture(char const* filename)
{
	int nrChannels;
	imagedata = stbi_load(filename, &width, &height, &nrChannels, 0);

}

Texture::~Texture()
{
	if (imagedata != nullptr)
	{
		delete[] imagedata;
	}
	
}

Vector4 Texture::Sample(float u, float v) const
{
    float x = u * width;
    float y = v * height;

    float top = int(y) + 0.5f;
    float bottom = top - 1.0f;
    float right = int(x) + 0.5f;
    float left = right - 1.0f;
    float xToLeft = x - left;
    float xToRight = right - x;
    float yToTop = top - y;
    float yToBottom = y - bottom;


    int leftbottom = (int(bottom) * width + int(left)) * 3;
    int rightbottom = (int(bottom) * width + int(right)) * 3;
    int lefttop = (int(top) * width + int(left)) * 3;
    int righttop = (int(top) * width + int(right)) * 3;
    int c0 = (int(top) * width + int(x)) * 3;
    int c1 = (int(bottom) * width + int(x)) * 3;

    float redInBottom = xToLeft * imagedata[rightbottom] + xToRight * imagedata[leftbottom];
    float greenInBottom = xToLeft * imagedata[rightbottom + 1] + xToRight * imagedata[leftbottom + 1];
    float blueInBottom = xToLeft * imagedata[rightbottom + 2] + xToRight * imagedata[leftbottom + 2];

    float redInTop = xToLeft * imagedata[righttop] + xToRight * imagedata[lefttop];
    float greenInTop = xToLeft * imagedata[righttop + 1] + xToRight * imagedata[lefttop + 1];
    float blueInTop = xToLeft * imagedata[righttop + 2] + xToRight * imagedata[lefttop + 2];

    float red = yToTop * imagedata[c1] + yToBottom * imagedata[c0];
    float green = yToTop * imagedata[c1 + 1] + yToBottom * imagedata[c0 + 1];
    float blue = yToTop * imagedata[c1 + 2] + yToBottom * imagedata[c0 + 2];

	return Vector4(red / 255, green / 255, blue / 255, 0);
}
