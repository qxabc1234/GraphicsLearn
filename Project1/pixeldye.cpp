#include "pixeldye.h"



void scan(glm::vec3 verticesn[], int size, unsigned char* data) {
    int Max_Y = 0;
    int Min_Y = (int)SCR_HEIGHT;
    for (int i = 0; i < size; i++) {
        if (verticesn[i].y > Max_Y)
            Max_Y = verticesn[i].y;
        if (verticesn[i].y < Min_Y)
            Min_Y = verticesn[i].y;
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

            if (i == verticesn[j].y) {
                if (verticesn[(j - 1 + size) % size].y > verticesn[j].y)
                {
                    NET* p = new NET;
                    p->x = verticesn[j].x;
                    p->ymax = verticesn[(j - 1 + size) % size].y;
                    float DX = verticesn[(j - 1 + size) % size].x - verticesn[j].x;
                    float DY = verticesn[(j - 1 + size) % size].y - verticesn[j].y;
                    p->dx = DX / DY;
                    p->next = pNET[i]->next;
                    pNET[i]->next = p;
                }
                if (verticesn[(j + 1 + size) % size].y > verticesn[j].y)
                {
                    NET* p = new NET;
                    p->x = verticesn[j].x;
                    p->ymax = verticesn[(j + 1 + size) % size].y;
                    float DX = verticesn[(j + 1 + size) % size].x - verticesn[j].x;
                    float DY = verticesn[(j + 1 + size) % size].y - verticesn[j].y;
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
            while (tq->next != NULL && tq->next->x <= ap->x) 
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