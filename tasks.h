#ifndef TASKS_H
#define TASKS_H 
#include <stdio.h>
#include "healkristin.h"


void task2(ligacoes *tabela, FILE *f_saida, int nr_cid, int nr_clusters, no **clusters_head, no **clusters_cauda, int *ordem_clusters);
int task3(coordenadas *pos, int nr_cidades, ligacoes *mapa, int cidref, no **clusters_head);
int task4(coordenadas *pos, int nr_cidades, ligacoes *mapa, int cidref, no **clusters_head, int nr_clusters, int *dMinCluster);

#endif