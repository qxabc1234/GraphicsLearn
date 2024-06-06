#include "VertexShader.h"


VertexOut VertexShader::ShaderProc(Vertex v, const ShadingConstants& constants)
{

    VertexOut vo;
    vo.worldPos = constants.modelMatrix * Vector4(v.Position , 1);
    vo.clipPos = constants.projectionMatrix * (constants.viewMatrix * vo.worldPos);
    vo.worldNormal = (constants.modelMatrix * Vector4(v.Normal, 0)).Normalize();
    vo.uv = Vector4(v.TexCoords);
    vo.ndcPos = vo.clipPos / vo.clipPos.w;
    vo.tangent = (constants.modelMatrix * Vector4(v.Tangent, 0)).Normalize();
    vo.bitangentnt = (constants.modelMatrix * Vector4(v.Bitangent)).Normalize();
  
    return vo;
}
