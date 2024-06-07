#include "FragmentShader.h"
#include <cmath>
#include <algorithm>

Vector4 FragmentShader::ShaderProc(VertexOut v, const ShadingConstants& constants)
{
    Vector4 color = constants.mainTex->Sample(v.uv.x, v.uv.y);
    Vector4 normal = (constants.normalTex->Sample(v.uv.x, v.uv.y) * 2.0f - 1.0f);
    normal = Vector4(normal.x, normal.y, normal.z, 0.0f).Normalize();
    Vector4 specularColor = constants.specularTex->Sample(v.uv.x, v.uv.y);
    Matrix TBN{ v.tangent, v.bitangent, v.worldNormal, {0, 0, 0, 0} };
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
    float spec = (float)pow(std::max(0.0f, normal * h), 32);

    Vector4 diffuse = color.ComponentWiseMultiply(constants.lightColor * diff * constants.intensity);
    Vector4 ambient = color.ComponentWiseMultiply(constants.lightColor * ambientStrength);
    Vector4 specular = specularColor.ComponentWiseMultiply(constants.lightColor * spec * specularStrength);


	return diffuse + specular + ambient;
}
