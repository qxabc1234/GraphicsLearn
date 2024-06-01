#include "Pipeline.h"
#include "VertexShader.h"
#include "FragmentShader.h"
#include <map>


void Pipeline::Initialize(unsigned int width, unsigned int height)
{
    if (data != nullptr) 
    {
        delete data;
    }
    data = new PipelineData(width, height);

}

Pipeline::~Pipeline()
{
    delete data;
    VertexOutBuffer.clear();
}

void Pipeline::Rasterize(const VertexOut& v0, const VertexOut& v1, const VertexOut& v2, const ShadingConstants& constants)
{
    int Max_Y = 0;
    int Min_Y = data->height;
    int voLen = sizeof(v0.data) / sizeof(Vector4);
    FragmentShader fs;
    
    int imagePos[3][2] = {{int((v0.clipPos.x / v0.clipPos.w) * 2 / data->width + 2 / data->width + 0.5), int((v0.clipPos.y / v0.clipPos.w) * 2 / data->height + 2 / data->height + 0.5)},
                          {int((v1.clipPos.x / v1.clipPos.w) * 2 / data->width + 2 / data->width + 0.5), int((v1.clipPos.y / v1.clipPos.w) * 2 / data->height + 2 / data->height + 0.5)},
                          {int((v2.clipPos.x / v2.clipPos.w) * 2 / data->width + 2 / data->width + 0.5), int((v2.clipPos.y / v2.clipPos.w) * 2 / data->height + 2 / data->height + 0.5)}};

    for (int i = 0; i < 3; i++) {
        if (imagePos[i][1] > Max_Y)
            Max_Y = imagePos[i][1];
        if (imagePos[i][1] < Min_Y)
            Min_Y = imagePos[i][1];
    }

    AET* pAET = new AET;
    pAET->next = NULL;
    std::map<int, NET*> m;

    for (int j = 0; j < 3; j++) 
    {
        if (imagePos[(j - 1 + 3) % 3][1] > imagePos[j][1])
        {
            NET* p = new NET;
            p->x = imagePos[j][0];
            p->ymax = imagePos[(j - 1 + 3) % 3][1];
            float DX = imagePos[(j - 1 + 3) % 3][0] - imagePos[j][0];
            float DY = imagePos[(j - 1 + 3) % 3][1] - imagePos[j][1];
            p->dx = DX / DY;
            NET* q = m[imagePos[j][1]];
            if (q == NULL) {
                q = new NET;
                q->next = p;
                p->next = NULL;
                m[imagePos[j][1]] = q;
            }
            else {
                p->next = q->next;
                q->next = p;
            }
        }
        if (imagePos[(j + 1 + 3) % 3][1] > imagePos[j][1])
        {
            NET* p = new NET;
            p->x = imagePos[j][0];
            p->ymax = imagePos[(j + 1 + 3) % 3][1];
            float DX = imagePos[(j + 1 + 3) % 3][0] - imagePos[j][0];
            float DY = imagePos[(j + 1 + 3) % 3][1] - imagePos[j][1];
            p->dx = DX / DY;

            NET* q = m[imagePos[j][1]];
            if (q == NULL) {
                q = new NET;
                q->next = p;
                p->next = NULL;
                m[imagePos[j][1]] = q;
            }
            else {
                p->next = q->next;
                q->next = p;
            }
        }
    }

    for (int i = Min_Y; i <= Max_Y; i++) 
    {

        NET* p = pAET->next;
        while (p)
        {
            p->x = p->x + p->dx;
            p = p->next;
        }

        AET* tq = pAET;
        NET* ap = m[i];
        if (ap) ap = ap->next;
        while (ap) 
        {
            while (tq->next != NULL && (tq->next->x < ap->x || (tq->next->x == ap->x && tq->next->dx < ap->dx)))
                tq = tq->next;
            NET* t = ap->next;
            ap->next = tq->next;
            tq->next = ap;
            ap = t;
            tq = pAET;
        }

        AET* q = pAET;
        p = q->next;
        while (p) 
        {
            if (p->ymax == i) 
            {
                q->next = p->next;
                delete p;
                p = q->next;
            }
            else 
            {
                q = q->next;
                p = q->next;
            }
        }

        p = pAET->next;
        while (p && p->next != NULL) 
        {
            for (int j = p->x; j <= p->next->x; j++) 
            {
                // p 0 1
                float s1 = abs((j - imagePos[1][0]) * (imagePos[0][1] - imagePos[1][1]) - (imagePos[0][0] - imagePos[1][0]) * (i - imagePos[1][1]));
                // p 1 2
                float s2 = abs((j - imagePos[2][0]) * (imagePos[1][1] - imagePos[2][1]) - (imagePos[1][0] - imagePos[2][0]) * (i - imagePos[2][1]));
                // p 0 2
                float s3 = abs((j - imagePos[2][0]) * (imagePos[0][1] - imagePos[2][1]) - (imagePos[0][0] - imagePos[2][0]) * (i - imagePos[2][1]));
                float total = s1 + s2 + s3;
                float r1 = s1 / total;
                float r2 = s2 / total;
                float r3 = s3 / total;
                float inversedW = (r1 / v2.clipPos.w + r2 / v0.clipPos.w + r3 / v1.clipPos.w);
                VertexOut fragmentIn;
                for (int k = 0; k < voLen; k++) 
                {
                    fragmentIn.data[k] = (r1 / v2.clipPos.w * v2.data[k] + r2 / v0.clipPos.w * v0.data[k] + r3 / v1.clipPos.w * v1.data[k]) / inversedW;
                }
                Vector4 color = fs.ShaderProc(fragmentIn, constants);
                      
                int index = i * (data->width) + j;
                int t = index * 3;
                int z = fragmentIn.ndcPos.z ;
                if ( z < data->zbuffer[index])
                {
                    data->colors[t] = color.r * 255;
                    data->colors[t + 1] = color.g * 255;
                    data->colors[t + 2] = color.b * 255;
                    data->zbuffer[index] = z;
                }

            }
            p = p->next->next;
        }
    }

}



void Pipeline::DrawCall(const vector<Vertex>& vertices, const vector<unsigned int>& indices, const ShadingConstants& constants)
{
    VertexShader vs;

    for (int i = 0; i < vertices.size(); i++) 
    {
        VertexOut vo = vs.ShaderProc(vertices[i], constants);
        VertexOutBuffer.push_back(vo);
    }


    int size = indices.size();
    for (int i = 0; i < size / 3; i++) 
    {
        int index = i * 3;
        Rasterize(VertexOutBuffer[indices[index]], VertexOutBuffer[indices[index + 1]], VertexOutBuffer[indices[index + 2]], constants);
    }


}
