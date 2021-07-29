#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

typedef struct lista_vizinhos_t{
    struct lista_vizinhos_t* prox;
    int id;
}lista_vizinhos_t;

typedef struct no_t{
    int id;
    double posX, posY;
    lista_vizinhos_t *vizinhos;
}no_t;

typedef no_t* grafo_t;

bool lista_vizinhos_adicionar(lista_vizinhos_t **lista, int vizinho) {
    lista_vizinhos_t *vizinho_novo = (lista_vizinhos_t*)malloc(sizeof(lista_vizinhos_t));
    if(vizinho_novo == NULL) return false;
    vizinho_novo->id = vizinho;
    vizinho_novo->prox = *lista;
    *lista = vizinho_novo;
    return true;
}

void lista_vizinhos_imprimir(lista_vizinhos_t *lista){
    lista_vizinhos_t *vizinho = lista;
    
    while(vizinho){
        printf("%d ", vizinho->id);
        vizinho = vizinho->prox;
    }
}

grafo_t grafo_criar(int tam){
    return (grafo_t)malloc(sizeof(no_t)*tam);
}

void grafo_atualizar_vizinhos(grafo_t grafo, int tam, double raio_comunicacao){
    int i, j;

    for(i = 0; i < tam; i++){
        for(j = 0; j < tam; j++){
            if(i != j && (sqrt(pow(grafo[i].posX - grafo[j].posX, 2) + pow(grafo[i].posY - grafo[j].posY, 2)) < raio_comunicacao)){
                lista_vizinhos_adicionar(&grafo[j].vizinhos, grafo[i].id);
            }
        }        
    }
}

void grafo_imprimir(grafo_t grafo, int tam){
    int i;

    for(i = 0; i < tam; i++){
        printf("NÓ %d: ",i);
        lista_vizinhos_imprimir(grafo[i].vizinhos);
        printf("\n");
    }
}

int main(int argc, char **argv){
    int i, tam;
    double raio_comunicacao;
    no_t *no;
    grafo_t grafo;
    FILE *arquivo_entrada = fopen(argv[1],"r");
    
    fscanf(arquivo_entrada,"%d\t%lf\n", &tam, &raio_comunicacao);
    grafo = grafo_criar(tam);
    
    for(i = 0; i < tam; i++){
        fscanf(arquivo_entrada,"%d\t%lf\t%lf\n", &grafo[i].id, &grafo[i].posX, &grafo[i].posY);
        grafo[i].vizinhos = NULL;
    }
    grafo_atualizar_vizinhos(grafo, tam, raio_comunicacao);
    grafo_imprimir(grafo, tam);

    fclose(arquivo_entrada);
}
