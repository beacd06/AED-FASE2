#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "healkristin.h"
#include "tasks.h"



/*
 * Nome: preencher_tl
 * Descricao: preenche a tabela das ligacoes com uso do weigthed quick union com path compresion atribuindo um pai a cada cidade
 * e calcular simultaneamente o numero de clusters
 * Argumentos:
 *   - FILE *f_saida: ponteiro para o ficheiro de saida criado no main
 *   - ligacoes *tabela: Array de estruturas que guarda as os pais de cada cidade 
 *   - int nr_cidades: Número total de cidades no sistema
 * Retorno: int nr_clusters
 */
int preencher_tl(FILE *fp2, ligacoes *tabela, int nr_cidades){  //preenche a tabela das ligações

    for (int i = 0; i <= nr_cidades; i++){ //inicializa a tabela
        tabela[i].pai = i;
        tabela[i].sz = 1;
    }

    int p=0;
    int q=0;
    int i,j,t,x;

    int nr_clusters = nr_cidades;
        while (fscanf(fp2, "%d %d", &p, &q) == 2){

            for (i = p; i!=tabela[i].pai; i = tabela[i].pai); // sobe na árvore até ao pai de p
            for (j = q; j != tabela[j].pai; j = tabela[j].pai); //sobe na árvore até o pai de q

            if (i == j) continue; //se já tiverem o mesmo pai é porque já estão conectados, avançamos
            nr_clusters --;
        if(tabela[i].sz < tabela[j].sz){ //aplica o weigthed, verifica qual o caminho mais comprido e troca pais se for necessário

            tabela[i].pai = j;
            tabela[j].sz += tabela[i].sz;
            t = j;

        }else{ //caso seja ao contrário

            tabela[j].pai = i;
            tabela[i].sz += tabela[j].sz;
            t = i;
        }
        for(i=p; i != tabela[i].pai; i=x) { //aplica o path compresion, ignora as ligações diretas 
            x = tabela[i].pai;
            tabela[i].pai=t;
            //enquanto i não for o proprio pai, continua para o de cima
        }
        for(j=q; j!=tabela[j].pai; j=x){
            x =tabela[j].pai;
            tabela[j].pai = t;
        }

    }
    
    return nr_clusters;
}

/*
 * Nome: preencher_tc
 * Descricaoo: Guarda as coordenadas no ficheiro .map numa estrutura previamente criada
 * Argumentos:
 *   - FILE *fp3: file .map onde se faz a verificação se as coordenadas de cada cidade estãod entro do limite máximo definido
 *   - coordenadas *tabela: Estrutura onde guardamos as coordenadas
 *   - int cidref: cidade de referencia lida do .quests
 *   - int xmax: limite máximo definido para o eixo x
 *   - int ymax: limite máximo definido para o eixo y
 * Retorno: 1(se as coordenadas da cidade não estiverem dentro do limite das coordenadas); 0(se todas as coordenadas estiverme dentro do limite)
 */
int preencher_tc(FILE *fp3, coordenadas *tabela, int nr_cidades, int xmax, int ymax){  
    
    int cid = 0;//cidade a ser lida

    for (int i =0; i<= nr_cidades; i++){//incialização a zero
        tabela[i].x = 0;
        tabela[i].y = 0;
    }

    for(int i =1; i<= nr_cidades; i++){
        if(fscanf(fp3,"%d %d %d", &cid, &tabela[i].x, &tabela[i].y)==3){
            if(tabela[i].x > xmax || tabela[i].y>ymax || tabela[i].x < 0 || tabela[i].y < 0 || cid > nr_cidades || cid < 1){ //verifica se a cidade é válida
                return 1;
            }
        }else{
            return 1; 
        };        
    }
    return 0;
}



/*
 * Nome: main
 * Descricao: funcao principal do projeto, onde lemos as quests e chamamos as funcoes correspondentes
 * Argumentos:
 *   - int argc: nr de argumentos na compilacao
 *   - char *argv[]: ficheiros enviados na compilacao incluindo os ficheiros necessarios 
 * Retorno: int 0 se tudo correr bem
 */
int main(int argc, char *argv[]){ 
    if(argc != 4){ //verifica se nao ha ficheiros em falta
        return 0;
    }

    //arg[0] - executavel
    //arg[1] - ficheiro quest
    //arg[2] - ficheiro map
    //arg[3] - ficheiro position

    int nr_cidade = 0;
    int nr_ligacoes = 0;

    int ymax=0;
    int xmax=0;
    
    int cidref; //guarda a cidade extra caso haja
    int cidmin;

    int nr_clusters;
    

    char *ext2 = strrchr(argv[2], '.'); //verifica se o ficheiro map tem a extensao certa
    if (ext2 != NULL) {
        if (strcmp(ext2, ".map") != 0) {
            return 0;
        } 
    }
    FILE *fp2 = fopen(argv[2], "r"); //abre o ficheiro map
    if(fp2==NULL){
        return 0;
    }

    if (fscanf(fp2, "%d %d", &nr_cidade, &nr_ligacoes) != 2) { //guarda o nr de cidades e o nr de ligacoes
        fclose(fp2);
        return 0;
    }
    ligacoes *tabela_ligacoes = (ligacoes*) malloc ((nr_cidade+1) * sizeof(ligacoes)); //cria e aloca espaço para a tabela
    if (tabela_ligacoes == NULL){
        exit(1);
    }
    nr_clusters = preencher_tl(fp2, tabela_ligacoes, nr_cidade); //preenche a tabela e devolve o numero de clusters
    fclose(fp2); 


    char *ext3 = strrchr(argv[3], '.'); //verifica se o ficheiro positions tem a extensao certa
    if (ext3 != NULL) {
        if (strcmp(ext3, ".position") != 0) {
            return 0;
        } 
    }


    FILE *fp3 = fopen(argv[3], "r"); 
    if(fp3==NULL){
        return 0;
    }
    if(fscanf(fp3, "%d %d", &xmax, &ymax)!=2){ //guarda as dimensoes da matriz
        fclose(fp3);
        return 0;
    }

    coordenadas *tabela_coordenadas = (coordenadas*) malloc ((nr_cidade+1) * sizeof(coordenadas)); 
    if(tabela_coordenadas==NULL){
        exit(1);
    }
    int erro = preencher_tc(fp3, tabela_coordenadas, nr_cidade, xmax, ymax); //preenche a tabela das coordenadas
    fclose(fp3);

    if (erro == 1) {
        free(tabela_ligacoes);
        free(tabela_coordenadas);
        return 0;
    }

    FILE *f_saida; //cria o ficheiro de saida
    char *nome =  malloc(strlen(argv[1]) + strlen(".results") + 1); //cria o nome do ficheiro de saida
    if(nome == NULL){
        exit(1);
    }

    strcpy ( nome, argv[1] );
    char *ponto = strrchr(nome, '.'); //elimina o extensao anterior
    if(ponto == NULL){
        exit(1);
    }
    if(ponto != NULL){
        *ponto ='\0';
    }

    strcat ( nome, ".results" ); //adiciona a extensao certa
    f_saida = fopen(nome, "w"); 
    if(f_saida==NULL){
        return 0;
    }

    char *ext1 = strrchr(argv[1], '.'); //verifica se o ficheiro quests tem a extensao certa
    if (ext1 != NULL) {
        if (strcmp(ext1, ".quests") != 0) {
            return 0;
        } 
    }

    FILE *fp1 = fopen(argv[1], "r"); 
    if(fp1==NULL){
        return 0;
    }
    
    char task [10]; 

    no **clusters_head = (no **) calloc ((nr_cidade+1), sizeof(no *)); //array para ir guardando um ponteiro  para o incio de cada lista de clusters
    no **clusters_cauda = (no **)calloc ((nr_cidade+1), sizeof(no *)); //array para guardar um ponteiro para o fim de cada lista de cada cluster
    if(clusters_head == NULL || clusters_cauda == NULL){
        exit(1);
    }
    int *ordem_clusters = (int *)calloc(nr_clusters, sizeof(int)); //array para guadar a ordem a imprimir dos clusters
    int pai_atual;
    task2(tabela_ligacoes, f_saida, nr_cidade, nr_clusters, clusters_head, clusters_cauda, ordem_clusters );
    int *dMinCluster = (int*)calloc((nr_cidade+1),sizeof(int)); //vetor de verificacao para a task4 se já foi calculada a dmin para aquele cluster
    if(dMinCluster==NULL){exit(1);}

    /*--------------------------alterações feitas por bia para a task5 -----------------------------------------------------------*/

    int max_lig = (nr_cidade*(nr_cidade-1))/2; //calculo combinatório para calcular o máximo de ligações possiveis
    Aresta* vetor_arestas = (Aresta*)malloc(max_lig * sizeof(Aresta));
    int total_arestas = preencher_grafo(tabela_coordenadas, nr_cidade, clusters_head, vetor_arestas);
    int quant_cabo =0;
    double cabo_total =0.0;
    Aresta* usadas = task5(tabela_coordenadas, nr_cidade, tabela_ligacoes, vetor_arestas, total_arestas, &quant_cabo, &cabo_total);

    /*-----------------------------------------------------------------------------------------------------------------------------*/


    while (fscanf(fp1, "%5s", task) == 1){ // le a task a executar e escolhe a task
        if(strcmp(task, "Task1") == 0){ //para a task 1 imprime apenas o numero de clusters
            fprintf(f_saida, "Task1 %d\n\n", nr_clusters);

        }else if(strcmp(task, "Task2") == 0){
            fprintf(f_saida, "Task2 %d\n", nr_clusters); //começa por imprimir logo o nome da task e o nr de clusters
            for(int i =0; i<nr_clusters; i++){ //imprime cada cluster e as cidades correspondentes por ordem crescente
                fprintf(f_saida, "Cluster:");
                pai_atual = ordem_clusters[i];
                for(no *atual = clusters_head[pai_atual]; atual!= NULL; atual = atual->next){
                    fprintf(f_saida, " %d", atual->nr_cid);
                }
                fprintf(f_saida, "\n");
            } 
            fprintf(f_saida, "\n");

        }else if(strcmp(task, "Task3") == 0){
            if(fscanf(fp1, "%d", &cidref)!=1){
                break;
            } //le a cidade de referencia
            if(cidref>nr_cidade || cidref<1 || nr_clusters==1){ //verifica se a cidade existe, e se ha mais que um clsuter
                fprintf(f_saida, "Task3 %d -2\n\n", cidref);}
            else{    
                cidmin = task3(tabela_coordenadas, nr_cidade, tabela_ligacoes, cidref, clusters_head);
                fprintf(f_saida, "Task3 %d %d\n\n", cidref, cidmin);
            }

        }else if(strcmp(task, "Task4") == 0){
            if (fscanf(fp1, "%d", &cidref)!=1){
                break;;
            }
            if(cidref>nr_cidade || cidref<1 || nr_clusters==1){ //mesma verificacao que a anterior
                fprintf(f_saida, "Task4 %d -2\n\n", cidref);}
            else {
                cidmin = task4(tabela_coordenadas, nr_cidade, tabela_ligacoes, cidref, clusters_head, nr_clusters, dMinCluster);
                fprintf(f_saida, "Task4 %d %d\n\n", cidref, cidmin);
            }
        }else if(strcmp(task, "Task5") == 0){

            fprintf(f_saida, "Task5 %.2f\n", cabo_total);
            for (int i=0; i<quant_cabo; i++){
                fprintf(f_saida, "%d %d %.2f\n", usadas[i].cid1, usadas[i].cid2, usadas[i].dist);  
            }

        }


    }

    for(int c = 0; c < nr_clusters; c++) { //libertacao de memoria das listas
        pai_atual = ordem_clusters[c];
        no *atual = clusters_head[pai_atual];
    
        while(atual != NULL) {
            no *temp = atual;        
            atual = atual->next;     
            free(temp);              
        }
    }

    free(clusters_head);
    free(clusters_cauda);
    free(ordem_clusters);

    free(dMinCluster);
    fclose(fp1); 
    free(tabela_ligacoes);
    free(tabela_coordenadas);
    free(nome);

    /*----------------------------------------alteraçoes------------------------------*/
    free(vetor_arestas); 
    vetor_arestas=NULL;
    free(usadas);
    usadas = NULL;
    /*---------------------------------------------------------------------------*/
    return 0;
}