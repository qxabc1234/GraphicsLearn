#pragma once
#include "mesh.h"
#include "CommonData.h"

class Pipeline
{

typedef struct XET
	{
		float x;
		float dx;
		float ymax;
		XET* next;
	}AET, NET;

typedef struct Edge
{
	float x;
	float ymax;
	float dx;
	bool isValid = false;
};

private:
	PipelineData* data;
	std::vector<VertexOut> VertexOutBuffer;

public:
	Pipeline() 
	{
		data = nullptr;
	}
	void Initialize(const unsigned int width, const unsigned int height);
	~Pipeline();

	void DrawCall(const vector<Vertex>& vertices, const vector<unsigned int>& indices, const ShadingConstants& constants);
	void Present();
	void Clear();

private:
	void Rasterize(const VertexOut& v0, const VertexOut& v1, const VertexOut& v2, const ShadingConstants& constants);
};

