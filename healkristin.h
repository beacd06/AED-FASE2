#ifndef HEALKRISTIN_H
#define HEALKRISTIN_H
#include <stdio.h>

//Estrutura em são guardadas as coordenadas de cada cidade
typedef struct coordenadas{ 
    int x;
    int y;
}coordenadas;

//Estrutura onde são guardadas as ligações entre cidades, onde é aplicado o weighted union com path compression
typedef struct ligacoes{
    int pai;
    int sz; 
}ligacoes;

typedef struct Aresta{ //estrutura para a task5
    int cid1;
    int cid2;
    int dist;
}Aresta;



//Estrutura que reresenta o nó da lista ligada utilizada na task2
typedef struct no{ 
    int nr_cid;
    struct no *next;
}no;

int preencher_tl(FILE *fp2, ligacoes *tabela, int nr_cidades);
int preencher_tc(FILE *fp3, coordenadas *tabela, int nr_cidades, int xmax, int ymax);



#endif
