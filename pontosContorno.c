/*-----------------------------------------------------------+ 
 |  File:.... contorno.c                                     |
 |  Author:.. Maria José Silva de Carvalho (zezedcarvalho)   |
 |  Email:... mjsc.zeze@gmail.com                            |
 |  Created on 16 de Julho de 2017, 21:23                    |
 +-----------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>

#define DEBUG(x) x
#define TRUE 1
#define FALSE 0

int W = 16;
int in[] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0,
    0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0,
    0, 0, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 0, 0,
    0, 0, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 0, 0,
    0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0,
    0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0,
    0, 0, 0, 0, 0, 1, 1, 1, 0, 1, 1, 1, 0, 0, 0, 0,
    0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0,
    0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

typedef struct {
    int i, j;
} point;

typedef struct no * ptno;

typedef struct no {
    point p;
    ptno prox;
} no;

ptno insere(ptno L, point p) {
    ptno q = (ptno) malloc(sizeof (no));
    q-> p = p;
    q-> prox = L;
    return q;
}

void mostra_contorno(ptno L) {
    printf("Contorno = [");
    while (L) {
        printf("(%d,%d)", L->p.i, L->p.j);
        L = L->prox;
        if (L) printf(",");
    }
    printf("]\n\n");
}

void mostra_imagem(int *img, int h, int w) {
    int i, j;
    printf("Imagem\n   ");
    for (j = 0; j < w; j++)
        printf("%2d ", j);
    printf("\n   ");
    for (j = 0; j < w; j++)
        printf("---");
    for (i = 0; i < h; i++) {
        printf("\n%2d|", i);
        for (j = 0; j < w; j++) {
            printf("%2c ", img[i * w + j] ? 'x' : '.');
        }
    }
    printf("\n");
}

/*----------------------------------------------------------------------------+
 | O vetor de pontos C é a 8-vizinhanca de qualquer ponto                     |
 | [(x-1,y-1),(x-1,y),(x-1,y+1),(x,y-1),(x,j+1),(x+1,j-1),(x+1,j),(x+1,j+1)]  |
 | Comecamos com o primeiro da vizinho da esquerda [(x,y-1)] para percorrer   |
 | o contorno em sentido horario --->                                         |
 | VIZINHOS: quando o ponto do contorno é encontrado, [k] deve retornar       |
 |           para o ponto de fundo anterior:                                  |
 |            PARADA    | CONTINUA                                            |
 |             0 ou 1   |  k = 6                                              |
 |             2 ou 3   |  k = 0                                              |
 |             4 ou 5   |  k = 2                                              |
 |             6 ou 7   |  k = 4                                              |
 +----------------------------------------------------------------------------*/
ptno contorno(int *img, int h, int w) {
    point b0, b1, b, pAnt, c[8] = {0, -1, -1, -1, -1, 0, -1, 1, 0, 1, 1, 1, 1, 0, 1, -1};
    ptno L = NULL;
    int i, j = 0, k = -1;
    //primeiro ponto diferente da cor de fundo (b0)
    for (i = 0; i < w && !img[i * w + j]; i++) {
        for (j = 0; j < w && !img[i * w + j]; j++);
        if (img[i * w + j]) break;
    }
    b0.i = i;
    b0.j = j;
    L = insere(L, b0);
    //segundo ponto diferente (b1)
    do {
        k++;
        i = b0.i + c[k].i;
        j = b0.j + c[k].j;
    } while (!img[i * w + j]);
    b1.i = i;
    b1.j = j;
    b = b1;
    //outros pontos do contorno
    do {
        pAnt = b;
        L = insere(L, b);
        if ((k == 0) || (k == 1)) k = 6;
        else if ((k == 2) || (k == 3)) k = 0;
        else if ((k == 4) || (k == 5)) k = 2;
        else if ((k == 6) || (k == 7)) k = 4;
        do {
            i = b.i + c[k].i;
            j = b.j + c[k].j;
            k++;
            if (k == 8) k = k % 8; //k = 0
        } while (!img[i * w + j]); //ponto do contorno é encontrado
        if (k) k--; //decremento do k após o laço;
        b.i = i;
        b.j = j;
        //condição de parada se ponto anterior é =b0 e o atual é =b1;
    } while (!((b.i == b1.i) && (b.j == b1.j) && (pAnt.i == b0.i) && (pAnt.j == b0.j)));
    L = L->prox;
    return L;
}

int main(void) {
    ptno L = contorno(in, W, W);
    mostra_imagem(in, W, W);
    puts("\n");
    mostra_contorno(L);
    return 0;
}
