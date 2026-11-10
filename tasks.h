#ifndef TASKS_H
#define TASKS_H 
#include <stdio.h>
#include "healkristin.h"


void task2(ligacoes *tabela, FILE *f_saida, int nr_cid, int nr_clusters, no **clusters_head, no **clusters_cauda, int *ordem_clusters);
int task3(coordenadas *pos, int nr_cidades, ligacoes *mapa, int cidref, no **clusters_head);
int task4(coordenadas *pos, int nr_cidades, ligacoes *mapa, int cidref, no **clusters_head, int nr_clusters, int *dMinCluster);
int preencher_grafo(coordenadas *pos, int nr_cidades, no **clusters_head, Aresta* vetor_arestas);
int comparar2(const void *a, const void *b);
int comparar1(const void *a, const void *b);
int UFfind(ligacoes *tabela, int i);
void UFunion(ligacoes *tabela, int raiz1, int raiz2);
Aresta* task5(coordenadas *pos, int nr_cidades, ligacoes *mapa, Aresta* vetor_aresta, int nr_arestas, int *quant_cabo, int *total_cabo);

#endif