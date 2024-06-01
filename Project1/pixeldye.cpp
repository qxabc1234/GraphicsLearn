#include "pixeldye.h"
#include <map>
#include <vector>


void scan(Vector4 verticeNDC[], Vector4 verticesClip[], Vector4 verticesNormal[], Vector4 verticesWorld [], int size, unsigned char* data, unsigned char* imagedata, Vector4 verticeuv[], int width, int height, float* zbuffer, float intensity, Vector4 lightDir, Vector4 lightColor, Vector4 cameraPos) {
    int Max_Y = 0;
    int Min_Y = (int)SCR_HEIGHT;

    std::vector<std::vector<int>> vertices;
    std::vector <std::vector<float>> veticeDivideW;
    std::vector <std::vector<float>> normal;
    std::vector<std::vector<float>> world;

    for (int i = 0; i < size; i++) {
        std::vector<int> v;
        v.push_back(int(verticeNDC[i].x + 0.5));
        v.push_back(int(verticeNDC[i].y + 0.5));
        vertices.push_back(v);
        std::vector<float> v1;
        v1.push_back(verticeuv[i].x / verticesClip[i].w);
        v1.push_back(verticeuv[i].y / verticesClip[i].w);
        v1.push_back(1 / verticesClip[i].w);
        v1.push_back(verticeNDC[i].z / verticesClip[i].w);
        veticeDivideW.push_back(v1);
        std::vector<float> v2;
        v2.push_back(verticesNormal[i].x / verticesClip[i].w);
        v2.push_back(verticesNormal[i].y / verticesClip[i].w);
        v2.push_back(verticesNormal[i].z / verticesClip[i].w);
        normal.push_back(v2);
        std::vector<float> v3;
        v3.push_back(verticesWorld[i].x / verticesClip[i].w);
        v3.push_back(verticesWorld[i].y / verticesClip[i].w);
        v3.push_back(verticesWorld[i].z / verticesClip[i].w);
        world.push_back(v3);

    }

    for (int i = 0; i < size; i++) {
        if (vertices[i][1] > Max_Y)
            Max_Y = vertices[i][1];
        if (vertices[i][1] < Min_Y)
            Min_Y = vertices[i][1];
            
    }


    AET* pAET = new AET;
    pAET->next = NULL;

    std::map<int, NET*> m;

    for (int j = 0; j < size; j++) {

          if (vertices[(j - 1 + size) % size][1] > vertices[j][1])
                {
                    NET* p = new NET;
                    p->x = vertices[j][0];
                    p->ymax = vertices[(j - 1 + size) % size][1];
                    float DX = vertices[(j - 1 + size) % size][0] - vertices[j][0];
                    float DY = vertices[(j - 1 + size) % size][1] - vertices[j][1];
                    p->dx = DX / DY;
                    NET* q = m[vertices[j][1]];
                    if (q == NULL) {
                        q = new NET;
                        q->next = p;
                        p->next = NULL;
                        m[vertices[j][1]] = q;

                    }
                    else {
                        p->next = q->next;
                        q->next = p;
                    }
                   
                }
          if (vertices[(j + 1 + size) % size][1] > vertices[j][1])
                {
                    NET* p = new NET;
                    p->x = vertices[j][0];
                    p->ymax = vertices[(j + 1 + size) % size][1];
                    float DX = vertices[(j + 1 + size) % size][0] - vertices[j][0];
                    float DY = vertices[(j + 1 + size) % size][1] - vertices[j][1];
                    p->dx = DX / DY;
                    
                    NET* q = m[vertices[j][1]];
                    if (q == NULL) {
                        q = new NET;
                        q->next = p;
                        p->next = NULL;
                        m[vertices[j][1]] = q;
                    }
                    else {
                        p->next = q->next;
                        q->next = p;
                    }
                }
   
    }


    for (int i = Min_Y; i <= Max_Y; i++) {

        NET* p = pAET->next;

        while (p) {
            p->x = p->x + p->dx;
            p = p->next;
        }

        AET* tq = pAET;
        NET* ap = m[i];
        if (ap) ap = ap->next;
        while (ap) {
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
        while (p) {
            if (p->ymax == i)  {
                q->next = p->next;
                delete p;
                p = q->next;
            }
            else {
                q = q->next;
                p = q->next;
            }
        }

        p = pAET->next;
        while (p && p->next != NULL) {
            for (int j = p->x; j <= p->next->x; j++) {
                // p 0 1
                float s1 = abs((j - vertices[1][0]) * (vertices[0][1] - vertices[1][1]) - (vertices[0][0] - vertices[1][0]) * (i - vertices[1][1]));
                // p 1 2
                float s2 = abs((j - vertices[2][0]) * (vertices[1][1] - vertices[2][1]) - (vertices[1][0] - vertices[2][0]) * (i - vertices[2][1]));
                // p 0 2
                float s3 = abs((j - vertices[2][0]) * (vertices[0][1] - vertices[2][1]) - (vertices[0][0] - vertices[2][0]) * (i - vertices[2][1]));
                float total = s1 + s2 + s3;
                float r1 = s1 / total;
                float r2 = s2 / total;
                float r3 = s3 / total;
                float u = (r1 * veticeDivideW[2][0] + r2 * veticeDivideW[0][0] + r3 * veticeDivideW[1][0]);
                float v = (r1 * veticeDivideW[2][1] + r2 * veticeDivideW[0][1] + r3 * veticeDivideW[1][1]);
                float inversedW = (r1 * veticeDivideW[2][2] + r2 * veticeDivideW[0][2] + r3 * veticeDivideW[1][2]);
                float zDivideW = (r1 * veticeDivideW[2][3] + r2 * veticeDivideW[0][3] + r3 * veticeDivideW[1][3]);
                float z = zDivideW / inversedW;
                float normalx = (r1 * normal[2][0] + r2 * normal[0][0] + r3 * normal[1][0]) / inversedW;
                float normaly = (r1 * normal[2][1] + r2 * normal[0][1] + r3 * normal[1][1]) / inversedW;
                float normalz = (r1 * normal[2][2] + r2 * normal[0][2] + r3 * normal[1][2]) / inversedW;

                float worldx = (r1 * world[2][0] + r2 * world[0][0] + r3 * world[1][0]) / inversedW;
                float worldy = (r1 * world[2][1] + r2 * world[0][1] + r3 * world[1][1]) / inversedW;
                float worldz = (r1 * world[2][2] + r2 * world[0][2] + r3 * world[1][2]) / inversedW;

                Vector4 n(normalx, normaly, normalz, 0.0f);
                Vector4 view(cameraPos.x - worldx, cameraPos.y - worldy, cameraPos.z - worldz, 0.0f);
                int index = i * SCR_WIDTH + j;
                int t = index * 3;

                if ( z < zbuffer[index]) {
                    Vector4 color = search(u / inversedW, v / inversedW, width, height, imagedata);
                    float ambientStrength = 0.1;
                    float diff = std::max(0.0f, n * lightDir);
                    Vector4 diffuse = lightColor * diff;
                    Vector4 ambient = lightColor * ambientStrength;
                    data[t] = color.r * 255 * (diffuse.r + ambient.r);
                    data[t + 1] = color.g * 255 * (diffuse.g + ambient.g);
                    data[t + 2] = color.b * 255 * (diffuse.b + ambient.b);

                    zbuffer[index] = z;
                }

            }
            p = p->next->next;
        }
    }

}

Vector4 search(float u, float v, int width, int height, unsigned char* imagedata) {
    float x = u * width;
    float y = v * height;

    float top = int(y) + 0.5f;
    float bottom = top - 1.0f;
    float right = int(x) + 0.5f;
    float left = right - 1.0f;
    float xToLeft = x - left;
    float xToRight = right - x;
    float yToTop = top - y;
    float yToBottom = y - bottom;
 

    int leftbottom = (int(bottom) * width + int(left)) * 3;
    int rightbottom = (int(bottom) * width + int(right)) * 3;
    int lefttop = (int(top) * width + int(left)) * 3;
    int righttop = (int(top) * width + int(right)) * 3;
    int c0 = (int(top) * width + int(x)) * 3;
    int c1 = (int(bottom) * width + int(x)) * 3;
    
    float redInBottom = xToLeft * imagedata[rightbottom] + xToRight * imagedata[leftbottom];
    float greenInBottom = xToLeft * imagedata[rightbottom + 1] + xToRight * imagedata[leftbottom + 1];
    float blueInBottom = xToLeft * imagedata[rightbottom + 2] + xToRight * imagedata[leftbottom + 2];

    float redInTop = xToLeft * imagedata[righttop] + xToRight * imagedata[lefttop];
    float greenInTop = xToLeft * imagedata[righttop + 1] + xToRight * imagedata[lefttop + 1];
    float blueInTop = xToLeft * imagedata[righttop + 2] + xToRight * imagedata[lefttop + 2];
        
    float red = yToTop * imagedata[c1] + yToBottom * imagedata[c0];
    float green = yToTop * imagedata[c1 + 1] + yToBottom * imagedata[c0 + 1];
    float blue = yToTop * imagedata[c1 + 2] + yToBottom * imagedata[c0 + 2];

    return Vector4(red/255.0f, green/255.0f, blue/255.0f, 0.0f);
}



