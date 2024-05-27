#include "pixeldye.h"
#include <map>
#include <vector>


void scan(Vector4 verticesn[], int size, unsigned char* data, unsigned char* imagedata, Vector4 verticeuv[], int width, int height) {
    int Max_Y = 0;
    int Min_Y = (int)SCR_HEIGHT;
    std::vector<std::vector<int>> vertices;

    for (int i = 0; i < size; i++) {
        std::vector<int> v;
        v.push_back(int(verticesn[i].x + 0.5));
        v.push_back(int(verticesn[i].y + 0.5));
        vertices.push_back(v);
    }

    for (int i = 0; i < size; i++) {
        if (vertices[i][1] > Max_Y)
            Max_Y = vertices[i][1];
        if (vertices[i][1] < Min_Y)
            Min_Y = vertices[i][1];
    }

    // u = x*dx + du
    float dx = (verticeuv[1].x - verticeuv[0].x) / (vertices[1][0] - vertices[0][0]);
    float du = -dx * vertices[0][0];
    // v = y*dy +dv
    float dy = (verticeuv[2].y - verticeuv[1].y) / (vertices[2][1] - vertices[1][1]);
    float dv = -dy * vertices[1][1];
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
            while (tq->next != NULL && (tq->next->x + tq->next->dx )<= (ap->x +ap->dx))
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
            if (p->ymax == i) {
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
                float u = j * dx + du;
                float v = i * dy + dv;
                int t = (i * SCR_WIDTH + j) * 3;
                std::vector<int> color = search(v, u, width, height, imagedata);
                 data[t] = color[0];
                data[t + 1] = color[1];
                data[t + 2] = color[2];
            }
            p = p->next->next;
        }
    }

}

std::vector<int> search(float u, float v, int width, int height, unsigned char* imagedata) {
    float x = u * width;
    float y = v * height;

    float top = int(y) + 0.5;
    float bottom = top - 1.0;
    float right = int(x) + 0.5;
    float left = right - 1.0;

    float distance1 = std::sqrt((x - left) * (x - left) + (y - bottom) * (y - bottom));
    float distance2 = std::sqrt((x - right) * (x - right) + (y - bottom) * (y - bottom));
    float distance3 = std::sqrt((x - left) * (x - left) + (y - top) * (y - top));
    float distance4 = std::sqrt((x - right) * (x - right) + (y - top) * (y - top));

    int leftbottom = (int(left) * width + int(bottom)) * 3;
    int rightbottom = (int(right) * width + int(bottom)) * 3;
    int lefttop = (int(left) * width + int(top)) * 3;
    int righttop = (int(right) * width + int(top)) * 3;
    float total = distance1 + distance2 + distance3 + distance4;
    
    int red = (distance1 * imagedata[leftbottom] + distance2 * imagedata[rightbottom] + distance3 * imagedata[lefttop] + distance4 * imagedata[righttop]) / total;
    int green = (distance1 * imagedata[leftbottom + 1] + distance2 * imagedata[rightbottom + 1] + distance3 * imagedata[lefttop + 1] + distance4 * imagedata[righttop + 1]) / total;
    int blue = (distance1 * imagedata[leftbottom + 2] + distance2 * imagedata[rightbottom + 2] + distance3 * imagedata[lefttop + 2] + distance4 * imagedata[righttop + 2]) / total;

    std::vector<int> color;
    color.push_back(red);
    color.push_back(green);
    color.push_back(blue);

    return color;
}

