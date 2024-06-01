#pragma once
#include<iostream>
#include<vector>
#include<stack>
#include<queue>
#include <glm/glm.hpp>
#include "Vector4.h"

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

typedef struct XET
{
	float x;
	float dx;
	float ymax;
	XET* next;
}AET,NET;

struct Color32
{
	float r;
	float g;
	float b;
	float a;
};
void scan(Vector4 verticeNDC[], Vector4 verticesClip[], Vector4 verticesNormal[], Vector4 verticesWorld[], int size, unsigned char* data, unsigned char* imagedata, Vector4 verticeuv[], int width, int height, float* zbuffer, float intensity, Vector4 lightDir, Vector4 lightColor, Vector4 cameraPos);
Vector4 search(float u, float v, int width, int height, unsigned char* imagedata);




