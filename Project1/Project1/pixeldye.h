#pragma once
#include<iostream>
#include<vector>
#include<stack>
#include<queue>
#include <glm/glm.hpp>
#include "../Vector4.h"

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

void scan(Vector4 verticesn[], Vector4 verticesPersp[], int size, unsigned char* data, unsigned char* imagedata, Vector4 verticeuv[], int width, int height);
std::vector<int> search(float u, float v, int width, int height, unsigned char* imagedata);

