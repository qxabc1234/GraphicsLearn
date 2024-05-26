#include "pixeldye.h"




void scan(Vector4 verticesn[3], int size, unsigned char* data) {
    int Max_Y = 0;
    int Min_Y = (int)SCR_HEIGHT;

    int vertices[3][2] = { {int(verticesn[0].x+0.5),int(verticesn[0].y + 0.5)},
        {int(verticesn[1].x + 0.5),int(verticesn[1].y + 0.5)},
        {int(verticesn[2].x + 0.5),int(verticesn[2].y + 0.5)}};
    
    for (int i = 0; i < size; i++) {
        if (vertices[i][1] > Max_Y)
            Max_Y = vertices[i][1];
        if (vertices[i][1] < Min_Y)
            Min_Y = vertices[i][1];
    }

    AET* pAET = new AET;
    pAET->next = NULL;

    NET* pNET[SCR_HEIGHT];
    for (int i = 0; i <= Max_Y; i++) {
        pNET[i] = new NET;
        pNET[i]->next = NULL;
    }

    for (int i = Min_Y; i <= Max_Y; i++) {
        for (int j = 0; j < size; j++) {

            if (i == vertices[j][1]) {
                if (vertices[(j - 1 + size) % size][1] > vertices[j][1])
                {
                    NET* p = new NET;
                    p->x = vertices[j][0];
                    p->ymax = vertices[(j - 1 + size) % size][1];
                    float DX = vertices[(j - 1 + size) % size][0] - vertices[j][0];
                    float DY = vertices[(j - 1 + size) % size][1] - vertices[j][1];
                    p->dx = DX / DY;
                    p->next = pNET[i]->next;
                    pNET[i]->next = p;
                }
                if (vertices[(j + 1 + size) % size][1] > vertices[j][1])
                {
                    NET* p = new NET;
                    p->x = vertices[j][0];
                    p->ymax = vertices[(j + 1 + size) % size][1];
                    float DX = vertices[(j + 1 + size) % size][0] - vertices[j][0];
                    float DY = vertices[(j + 1 + size) % size][1] - vertices[j][1];
                    p->dx = DX / DY;
                    p->next = pNET[i]->next;
                    pNET[i]->next = p;
                }
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
        NET* ap = pNET[i]->next;
 
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
                int t = (i * SCR_WIDTH + j) * 3;
                data[t] = (unsigned char)(255.0f);
                data[t + 1] = (unsigned char)(0.0f);
                data[t + 2] = (unsigned char)(0.0f);
            }
            p = p->next->next;
        }
    }


}