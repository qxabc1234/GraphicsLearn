#pragma once
#include<iostream>
#include<vector>
#include<stack>
#include<queue>
#include <glm/glm.hpp>

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

void scan(glm::vec3 verticesn[], int size, unsigned char* data);

