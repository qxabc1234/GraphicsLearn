#include "FragmentShader.h"
#include <cmath>
#include <algorithm>

Vector4 FragmentShader::ShaderProc(VertexOut v, const ShadingConstants& constants)
{
    Vector4 color = constants.mainTex->Sample(v.uv.x, v.uv.y);
    Vector4 normal = (constants.normalTex->Sample(v.uv.x, v.uv.y) * 2.0f - 1.0f).Normalize();
    Matrix TBN{ v.tangent, v.bitangentnt, v.worldNormal, {0, 0, 0, 0} };
    Matrix TTBN = Matrix::transpose(TBN);

    Vector4 tangentLightPos = TTBN * constants.lightPos;
    Vector4 tangentCameraPos = TTBN * constants.cameraPos;
    Vector4 tangentWorldPos = TTBN * v.worldPos;
    Vector4 view = (tangentCameraPos - tangentWorldPos).Normalize();
    Vector4 lightDir = (tangentLightPos - tangentWorldPos).Normalize();
    Vector4 h = (view + lightDir).Normalize();

    float ambientStrength = 0.1f;
    float specularStrength = 0.5f;
    float diff = std::max(0.0f, normal * lightDir);
    float spec = (float)pow(std::max(0.0f, normal * h), 32.0);
    Vector4 diffuse = constants.lightColor * diff * constants.intensity;
    Vector4 ambient = constants.lightColor * ambientStrength;
    Vector4 specular = constants.lightColor * spec * specularStrength;

	return color.ComponentWiseMultiply(diffuse + specular + ambient);
}
