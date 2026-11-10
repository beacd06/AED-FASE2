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

//secalhar criamos um outro ficheiro só para estas funções auxiliares
//ver se queremos usar este grafo para a task4
int preencher_grafo(coordenadas *pos, int nr_cidades, no **clusters_head, Aresta* vetor_arestas){
  
    int nr_arestas = 0;

    // Percorrer todos os pares de clusters (i e j)
    for (int i = 1; i <= nr_cidades; i++) {
        if (clusters_head[i] == NULL) continue; // avanca ate estar na raiz do cluster i
    
        for (int j = i + 1; j <= nr_cidades; j++) {
            if (clusters_head[j] == NULL) continue; // avanca ate estar na raiz do cluster j
        
            double d_min_par = INFINITY; //para guardar a distancia minima 
            int melhor_cid_i = -1;
            int melhor_cid_j = -1;
        
            //calculamos a distancia de todos as cidades do cluster j ao cluster i
            for (no *atual_i = clusters_head[i]; atual_i != NULL; atual_i = atual_i->next) { //percorre a lista do cluster i
                int cid_i = atual_i->nr_cid;
                int xi = pos[cid_i].x;
                int yi = pos[cid_i].y;
            
                for (no *atual_j = clusters_head[j]; atual_j != NULL; atual_j = atual_j->next) { //percorre a lsita do cluster j
                    int cid_j = atual_j->nr_cid;
                    int xj = pos[cid_j].x;
                    int yj = pos[cid_j].y;
                
                    //calcular distancia euclidiana
                    double d = pow((xi - xj), 2) + pow((yi - yj), 2);
                
                    if (d < d_min_par) { //guarda apenas a menor distância entre os clusters
                        d_min_par = d;
                        //faz logo com que a cidade 1 seja a de menor indice como pedido no enunciado
                        if (cid_i < cid_j) {
                            melhor_cid_i = cid_i; 
                            melhor_cid_j = cid_j;
                        } else {
                            melhor_cid_i = cid_j; 
                            melhor_cid_j = cid_i;
                        }
                    } 

                }
            }
        
            
            if (d_min_par != INFINITY) {
                vetor_arestas[nr_arestas].cid1 = melhor_cid_i;
                vetor_arestas[nr_arestas].cid2 = melhor_cid_j;
                vetor_arestas[nr_arestas].dist = sqrt(d_min_par); //queremos a distancia sem ser euclidiana
                nr_arestas++;
            }
        }
    }


    return nr_arestas;
}

int comparar2(const void *a, const void *b) { //para fazer com que a saida fica pela ordem que queremos
    Aresta *arestaA = (Aresta *)a;
    Aresta *arestaB = (Aresta *)b;
    
    if (arestaA->cid1 != arestaB->cid1) {
        return arestaA->cid1 - arestaB->cid1;
    }
    return arestaA->cid2 - arestaB->cid2;
}

int comparar1(const void *a, const void *b){

    Aresta *arestaA = (Aresta *)a;
    Aresta *arestaB = (Aresta *)b;
    
    if (arestaA->dist < arestaB->dist) return -1;
    if (arestaA->dist > arestaB->dist) return 1;
    
    //caso tenham distancia diferentes vemos pela cidade 1
    if (arestaA->cid1 != arestaB->cid1) {
        return arestaA->cid1 - arestaB->cid1;
    }
    //se a cidade 1 tiverem a mesma distancia fazemos pela cidade 2
    return arestaA->cid2 - arestaB->cid2;
}


int UFfind(ligacoes *tabela, int i){
    int raiz = i;
    while (raiz != tabela[raiz].pai){
        raiz = tabela[raiz].pai;
    }

    int atual = i;
    while(atual != raiz){
        int proximo = tabela[atual].pai;
        tabela[atual].pai = raiz;
        atual = proximo;
    }
    return raiz;
}

void UFunion(ligacoes *tabela, int raiz1, int raiz2){
    if(tabela[raiz1].sz < tabela[raiz2].sz){
        tabela[raiz1].pai = raiz2;
        tabela[raiz2].sz += tabela[raiz1].sz;
    }else{
        tabela[raiz2].pai = raiz1;
        tabela[raiz1].sz += tabela[raiz2].sz;
    }
    return;
}

/* ------------------------------------- este comentario ainda nao foi alterado ---------------------------------------------
 * Nome: task5
 * Descricao: Calcula a cidade mais próxima do cluster da cidade de referencia
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

 Aresta* task5(coordenadas *pos, int nr_cidades, ligacoes *mapa, Aresta* vetor_aresta, int nr_arestas, int *quant_cabo, double *cabo_total){
    ligacoes *tabela_kruskal = (ligacoes*)malloc((nr_cidades + 1)*sizeof(ligacoes));

    for(int i = 0; i<=nr_cidades; i++){
        tabela_kruskal[i].pai = mapa[i].pai;
        tabela_kruskal[i].sz = mapa[i].sz;
    }

    Aresta *usadas = (Aresta *)malloc(nr_cidades * sizeof(Aresta));
    int nr_cabos = 0;
    double total_cabo = 0.0;

    qsort(vetor_aresta, nr_arestas, sizeof(Aresta), comparar1);

    for (int i=0; i<nr_arestas; i++){
        int raiz1 = UFfind(tabela_kruskal, vetor_aresta[i].cid1);
        int raiz2 = UFfind(tabela_kruskal, vetor_aresta[i].cid2);

        if(raiz1 != raiz2){ //Verifica se ja ligamos esses clusters
            UFunion(tabela_kruskal, raiz1, raiz2);

            usadas[nr_cabos] = vetor_aresta[i];
            total_cabo += vetor_aresta[i].dist;
            nr_cabos ++;
        }
    }

    qsort(usadas, nr_cabos, sizeof(Aresta), comparar2);


    free(tabela_kruskal);
    *quant_cabo = nr_cabos;
    *cabo_total = total_cabo;
    return usadas;
 }
