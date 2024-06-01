#pragma once
#include "Vector4.h"
#include "CommonData.h"

class FragmentShader
{
public:
	Vector4 ShaderProc(VertexOut v, const ShadingConstants& constants);

};

