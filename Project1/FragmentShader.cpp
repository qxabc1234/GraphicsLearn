#include "FragmentShader.h"
#include <cmath>

Vector4 FragmentShader::ShaderProc(VertexOut v, const ShadingConstants& constants)
{
    Vector4 color = constants.mainTex->Sample(v.uv.x, v.uv.y);
    Vector4 view = (constants.cameraPos - v.worldPos).Normalize();
    Vector4 h = (view + constants.lightDir) / (view + constants.lightDir).length();

    float ambientStrength = 0.1;
    float diff = std::max(0.0f, v.worldNormal * constants.lightDir);
    float spec = pow(std::max(0.0f, v.worldNormal * h), 200);
    Vector4 diffuse = constants.lightColor * diff;
    Vector4 ambient = constants.lightColor * ambientStrength;
    Vector4 specular = constants.lightColor * spec;

	return color.ComponentWiseMultiply(diffuse);
}
