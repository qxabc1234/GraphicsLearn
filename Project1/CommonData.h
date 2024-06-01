#pragma once

#include "Vector4.h"
#include "Matrix.h"
#include "Texture.h"

struct VertexOut
{
	union
	{
		struct 
		{
			Vector4 ndcPos;
			Vector4 clipPos;
			Vector4 worldPos;
			Vector4 worldNormal;
			Vector4 uv;
		};
		Vector4 data[5];
		
	};

	VertexOut()
	{
	}
};

struct ShadingConstants
{
	Matrix modelMatrix;
	Matrix viewMatrix;
	Matrix projectionMatrix;
	float intensity;
	Vector4 lightDir;
	Vector4 lightColor;
	Vector4 cameraPos;
	Texture mainTex;
};

struct PipelineData
{
	unsigned int width;
	unsigned int height;
	float* zbuffer;
	unsigned char* colors;

	PipelineData(unsigned int width, unsigned int height)
	{
		this->width = width;
		this->height = height;

		unsigned char* colors = new unsigned char[3 * width * height * sizeof(unsigned char)];
		float* zbuffer = new float[width * height * sizeof(float)];

		for (unsigned int i = 0; i < width * height; i++)
		{
			colors[i * 3] = (unsigned char)(0.0f);
			colors[i * 3 + 1] = (unsigned char)(255.0f);
			colors[i * 3 + 2] = (unsigned char)(0.0f);
			zbuffer[i] = 1.0;
		}
		this->colors = colors;
		this->zbuffer = zbuffer;
	}

	~PipelineData() 
	{
		delete[] colors;
		delete[] zbuffer;
	}
};
