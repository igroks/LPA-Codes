#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int tabela_hash_tam;

typedef struct pessoa_t{
    char nome[50];
    int idade;
    long long int cpf;
}pessoa_t;

typedef struct lista_pessoas_t{
    pessoa_t *dados_pessoa;
    struct lista_pessoas_t *prox;
}lista_pessoas_t;

typedef lista_pessoas_t** tabela_hash_t;

bool lista_pessoas_adicionar(lista_pessoas_t **lista, pessoa_t *pessoa) {
    lista_pessoas_t *pessoa_nova = (lista_pessoas_t*)malloc(sizeof(lista_pessoas_t));
    if(pessoa_nova == NULL) return false;
    pessoa_nova->dados_pessoa = pessoa;
    pessoa_nova->prox = *lista;
    *lista = pessoa_nova;
    return true;
}

void lista_pessoas_listar(lista_pessoas_t *lista){
    lista_pessoas_t *pessoa = lista;
    
    while(pessoa){
        printf("- %s\t", pessoa->dados_pessoa->nome);
        printf("%lld\t", pessoa->dados_pessoa->cpf);
        printf("%d\n", pessoa->dados_pessoa->idade);
        pessoa = pessoa->prox;
    }
}

tabela_hash_t tabela_hash_pessoas_criar(){
    tabela_hash_t tabela_hash = (tabela_hash_t)malloc(sizeof(tabela_hash_t)*tabela_hash_tam);
    int i;

    for(i = 0; i < tabela_hash_tam; i++){
        tabela_hash[i] = NULL;
    }

    return tabela_hash;
}

int tabela_hash_pessoas_funcao(pessoa_t *pessoa){
    return pessoa->cpf % tabela_hash_tam;
}

bool tabela_hash_pessoas_adicionar(tabela_hash_t tabela_hash, pessoa_t *pessoa){
    return lista_pessoas_adicionar(&tabela_hash[tabela_hash_pessoas_funcao(pessoa)],pessoa);
}

void tabela_hash_pessoas_listar(tabela_hash_t tabela_hash){
    int i;

    for(i = 0; i < tabela_hash_tam; i++){
        printf("POSIÇÃO %d DA TABELA HASH:\n",i);
        lista_pessoas_listar(tabela_hash[i]);
    }
}

int main(int argc, char const *argv[]){
    sscanf(argv[1],"%d",&tabela_hash_tam);
    tabela_hash_t tabela_hash = tabela_hash_pessoas_criar();
    FILE *arquivo_entrada = fopen(argv[2],"r");
    pessoa_t *pessoa;

    while(feof(arquivo_entrada) == 0){
        pessoa = (pessoa_t*)malloc(sizeof(pessoa_t));
        fscanf(arquivo_entrada,"%50[^\t]\t%lld\t%d\n", pessoa->nome, &pessoa->cpf, &pessoa->idade);
        tabela_hash_pessoas_adicionar(tabela_hash, pessoa);
    }

    tabela_hash_pessoas_listar(tabela_hash);

    fclose(arquivo_entrada);
}
