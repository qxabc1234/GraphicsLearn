#pragma once
#include "CommonData.h"
#include "mesh.h"

class VertexShader
{
public:
	VertexOut ShaderProc(Vertex v, const ShadingConstants& constants);
};

