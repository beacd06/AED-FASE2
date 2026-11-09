#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "healkristin.h"

/*
 * Nome: task2
 * Descricao: Preenche a lista de cada cluster e os vetores com os ponteiros da head e da cauda
 * Argumentos:
 *   - ligacoes *tabela: Array de estruturas que guarda as os pais de cada cidade 
 *   - FILE *f_saida: ponteiro para o ficheiro de saida criado no main 
 *   - int nr_cid: Número total de cidades no sistema
 *   - int nr_clsuters: numero de clusters calculado em outra função
 *   - no **clusters_head: vetor que guarda os ponteiros head de cada lista de clusters
 *   - no **clusters_cauda: vetor que guarda os ponterios cauda de cada lista de clusters
 *   - int *ordem_clusters: vetor que guarda a ordem dos clusters de foma crescente pela cidade de menor indice
 * 
 * Retorno: void
 */

void task2(ligacoes *tabela, FILE *f_saida, int nr_cid, int nr_clusters, no **clusters_head, no **clusters_cauda, int *ordem_clusters){
   
    if(clusters_head == NULL || clusters_cauda == NULL){
        exit(1);
    }
   
    if(ordem_clusters == NULL){
        exit(1);
    }

    int pai_atual = 0;
    int j = 0;
    int ordem = 0;

    for (int i =1; i<= nr_cid; i++){ //sobe pela tabela pai a pai ate chegar a raiz do cluster ao qual i pertence
        pai_atual = tabela[i].pai;
        j=i;
        while(pai_atual != j){ 
            j = pai_atual;
            pai_atual = tabela[j].pai;
        }

        no *novo_no = (no *)malloc(sizeof(no)); //cria um novo no na lista para adicionar
        novo_no ->nr_cid =i;
        novo_no->next = NULL;

        if(clusters_head[pai_atual] == NULL) {  //verifica se o array dos ponteiros para os clusters tem algum ponteiro no idx correspondente a raiz que queremos
            ordem_clusters[ordem] = pai_atual; //caso nao tenha adiciona o clsuter ao array com a ordem
            ordem++;
            clusters_head[pai_atual]= novo_no; //caso seja o primeiro numero de cluster adiciona um ponteiro para esse valor, pois esse vai ser o head
        }else {
            clusters_cauda[pai_atual]->next = novo_no; //caso não seja a primeira aparencia a cauda passa a apontar esse novo no
        }
        clusters_cauda[pai_atual] = novo_no; //adiciona sempre o ponteiro do novo no ao ponteiro das caudas
        
    }

    return;
}

/*
 * Nome: task3
 * Descricaoo: Calcula a cidade mais próxima da cidade de refencia fora do seu cluster
 * Argumentos:
 *   - coordenadas *pos: Array de estruturas que guarda as coordenadas de cada cidade
 *   - int nr_cidades: Numero total de cidades
 *   - ligacoes *mapa: Array de estruturas das ligacoes com o weighted union com path compression
 *   - int cidref: cidade de referencia lida do .quests
 *   - no **clusters_head: vetor que guarda os ponteiros head de cada lista de clusters
 * Retorno: cidmin - cidade com distancia minima da cidade de referencia pertencente a outro cluster
 */
int task3(coordenadas *pos, int nr_cidades, ligacoes *mapa, int cidref, no **clusters_head){

    double d_min = INFINITY;
    int xref = pos[cidref].x; //coordenada x da cidade de referencia
    int yref = pos[cidref].y; //coordenada y da cidade de referencia
    int cidmin = -1; //varivel onde se guadara a cidade mais proxima de cidref
    int pairef=cidref; 


    //subimos nas ligações até encontrar o cluster da cidade de referencia
    while(mapa[pairef].pai != pairef){
        pairef = mapa[pairef].pai; 
    }

    for(int i = 1; i <= nr_cidades; i++){
        if(clusters_head[i] == NULL || i == pairef){
            continue;
        }   
            for(no *atual = clusters_head[i]; atual != NULL; atual = atual -> next){ //percorre as cidades da lista que não são a do cluster da cidade de referência
            
                int cid = atual -> nr_cid;
                //coordenadas da cidade a ser analisada
                int xcid = pos[cid].x; 
                int ycid = pos[cid].y;

                //calculo da distancia
                double d = pow((xcid-xref),2) + pow((ycid-yref),2);
                if(d<d_min){
                    d_min = d;
                    cidmin = cid;
                } else if(d==d_min){
                    if(cid<cidmin){cidmin=cid;}
                }
            }
    }

    return cidmin;
}

/*
 * Nome: task4
 * Descricaoo: Calcula a cidade mais próxima do cluster da cidade de referencia
 * Argumentos:
 *   - coordenadas *pos: Array de estruturas que guarda as coordenadas de cada cidade
 *   - int nr_cidades: Numero total de cidades
 *   - ligacoes *mapa: Array de estruturas das ligacoes com o weighted union com path compression
 *   - int cidref: cidade de referencia lida do .quests
 *   - no **clusters_head: vetor que guarda os ponteiros head de cada lista de clusters
 *   - int nr_clsuters: numero de clusters calculado em outra função
 *   - int *dMinCluster : guarda a distancia minima de cada cluster apos a calcular
 * Retorno: cidmin - cidade com distancia minima do cluster da cidade de referencia 
 */
int task4(coordenadas *pos, int nr_cidades, ligacoes *mapa, int cidref, no **clusters_head, int nr_clusters, int *dMinCluster){
    
    
    double d_min = INFINITY;
    int cidmin = -1;

    int pairef = cidref;
    //encontramos o pai da cidade de referencia
    while(mapa[pairef].pai != pairef){
        pairef = mapa[pairef].pai;
    }

    if(dMinCluster[pairef]!= 0 && dMinCluster != NULL){
        return dMinCluster[pairef];
    }

    for(no *atualref = clusters_head[pairef]; atualref != NULL; atualref = atualref -> next){
        //encontrar as coordenadas da cidade dentro do cluster de referência
        int cidClusRef = atualref -> nr_cid;
        int xref = pos[cidClusRef].x;
        int yref = pos[cidClusRef].y;
        
        for(int i=1; i<=nr_cidades;i++){
            if(i == pairef || clusters_head[i] == NULL){continue;}
            //corre as listas dos outros clusters
            for( no *atualcid = clusters_head[i]; atualcid != NULL; atualcid = atualcid -> next){
                int cid = atualcid -> nr_cid;
                int xcid = pos[cid].x;
                int ycid = pos[cid].y;
                
                //calculo da distancia euclidiana
                double d = pow((xref-xcid),2) + pow((yref-ycid),2);
                if (d<d_min){
                    d_min = d;
                    cidmin = cid;
                } else if (d_min == d){ //verificacao para ficar com o menor indice
                    if(cid<cidmin){cidmin = cid;}
                }
            }
        }
    }
    
    
    dMinCluster[pairef] = cidmin;
    

    return cidmin;

}

