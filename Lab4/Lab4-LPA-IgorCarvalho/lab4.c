#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

typedef struct evento_t {
    int tipo;
    int alvo;
    double tempo;
}evento_t;

typedef struct lista_eventos_t{
    evento_t* evento;
    struct lista_eventos_t* prox;
}lista_eventos_t;

typedef struct lista_vizinhos_t{
    struct lista_vizinhos_t* prox;
    int id;
}lista_vizinhos_t;

typedef struct no_t{
    int id;
    bool pacote_enviado;
    double posX, posY;
    lista_vizinhos_t *vizinhos;
}no_t;

typedef no_t* grafo_t;

bool lista_eventos_adicionar_ordenado(evento_t *evento, lista_eventos_t **lista){
    lista_eventos_t *item_novo = (lista_eventos_t*)malloc(sizeof(lista_eventos_t));
    lista_eventos_t *item_atual = *lista;

    if(item_novo == NULL) return false;
    item_novo->evento = evento;

    if(item_atual == NULL){
        item_novo->prox = item_atual;
        *lista = item_novo;
    
    }else if(item_novo->evento->tempo < item_atual->evento->tempo){
        item_novo->prox = item_atual;
        *lista = item_novo;
    
    }else{
        while(item_atual->prox && item_atual->prox->evento->tempo < item_novo->evento->tempo){
            item_atual = item_atual->prox;
        }
        item_novo->prox = item_atual->prox;
        item_atual->prox = item_novo;

    }
    
    return true;
}

bool lista_vizinhos_adicionar(lista_vizinhos_t **lista, int vizinho) {
    lista_vizinhos_t *vizinho_novo = (lista_vizinhos_t*)malloc(sizeof(lista_vizinhos_t));
    if(vizinho_novo == NULL) return false;
    vizinho_novo->id = vizinho;
    vizinho_novo->prox = *lista;
    *lista = vizinho_novo;

    return true;
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

double umTruncaAi(double num, unsigned int c){
    double pot = pow(10, c);
    double fatorM = num * pot;
    double res = round(fatorM) / pot;
    return res;
} 

void simulacao_iniciar(lista_eventos_t **lista, grafo_t grafo){
    lista_eventos_t *prim;
    lista_vizinhos_t *vizinho;
    evento_t *evento_novo;
    no_t *no;

    while(*lista){
        prim = *lista;
        *lista = prim->prox;
        printf("[%3.6lf] Nó %d recebeu pacote\n", prim->evento->tempo, prim->evento->alvo);

        no = &grafo[prim->evento->alvo];
        if(!(no->pacote_enviado)){
            vizinho = no->vizinhos;

            while(vizinho){
                printf("\t--> Repassando pacote para o nó %d ...\n", vizinho->id);
                evento_novo = (evento_t*)malloc(sizeof(evento_t));
                evento_novo->alvo = vizinho->id;
                evento_novo->tipo = 1;
                evento_novo->tempo = umTruncaAi(prim->evento->tempo + (0.1 + (vizinho->id *0.01)),6);
                lista_eventos_adicionar_ordenado(evento_novo, lista);
                vizinho = vizinho->prox;
            }
            no->pacote_enviado = true;
        }
    }
}

int main(int argc, char const *argv[]){
    int i, tam;
    float raio_comunicacao;
    no_t *no;
    grafo_t grafo;
    lista_eventos_t *lista_eventos = NULL;
    evento_t *evento_zero;
    FILE *arquivo_entrada = fopen(argv[1],"r");
    
    fscanf(arquivo_entrada,"%d\t%f\n", &tam, &raio_comunicacao);
    grafo = grafo_criar(tam);

    for(i = 0; i < tam; i++){
        fscanf(arquivo_entrada,"%d\t%lf\t%lf\n", &grafo[i].id, &grafo[i].posX, &grafo[i].posY);
        grafo[i].vizinhos = NULL;
    }
    grafo_atualizar_vizinhos(grafo, tam, raio_comunicacao);
    
    evento_zero->alvo = 0;
    evento_zero->tipo = 1;
    evento_zero->tempo = 1.0;
    lista_eventos_adicionar_ordenado(evento_zero,&lista_eventos);

    simulacao_iniciar(&lista_eventos, grafo);

    fclose(arquivo_entrada);    
}
