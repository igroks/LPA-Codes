#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct evento_t {
    int tipo;
    int alvo;
    double tempo;
}evento_t;

typedef struct lista_eventos_t{
    evento_t* evento;
    struct lista_eventos_t* prox;
}lista_eventos_t;

bool lista_eventos_adicionar_inicio(evento_t *evento, lista_eventos_t **lista){
    lista_eventos_t *item_novo = (lista_eventos_t*)malloc(sizeof(lista_eventos_t));
    if(item_novo == NULL) return false;
    item_novo->evento = evento;
    item_novo->prox = *lista;
    *lista = item_novo;
    return true;
}

bool lista_eventos_adicionar_fim(evento_t *evento, lista_eventos_t **lista){
    lista_eventos_t *item_novo = (lista_eventos_t*)malloc(sizeof(lista_eventos_t));
    lista_eventos_t *aux = *lista;

    if(item_novo == NULL) return false;
    item_novo->evento = evento;
    item_novo->prox = NULL;

    if(aux == NULL){
        *lista = item_novo;
    }else{
        while (aux->prox) {
        aux = aux->prox;
        }
        aux->prox = item_novo;
    }
    
    return true;
}

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

void lista_eventos_listar(lista_eventos_t *lista){
    lista_eventos_t *eventos = lista;
    int i = 1;

    while(eventos){
        printf("---- Evento (%d) ----\n",i);
        printf("Tipo: %d\n", eventos->evento->tipo);
        printf("Alvo: %d\n", eventos->evento->alvo);
        printf("Tempo: %3.6f\n", eventos->evento->tempo);
        printf("---------------------\n");
        eventos = eventos->prox;
        i++;
    }
}


void main(int argc, char const *argv[]){
    FILE *arquivo_entrada = fopen(argv[1],"r");
    evento_t *evento;
    lista_eventos_t *lista_eventos = NULL;
    
    if(arquivo_entrada == NULL){
        printf("Erro ao abrir o arquivo\n");
        exit(0);
    }
    while(feof(arquivo_entrada) == 0){
        evento = (evento_t*)malloc(sizeof(evento_t));
        fscanf(arquivo_entrada,"%lf\t%d\t%d\n",&evento->tempo, &evento->alvo, &evento->tipo);
        lista_eventos_adicionar_inicio(evento,&lista_eventos);
    }

    lista_eventos_listar(lista_eventos);

    fclose(arquivo_entrada);
}
