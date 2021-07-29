#include <stdio.h>
#include <string.h>
#include <math.h>

int main(int argc, char const *argv[]){
    int conjunto_tam = strlen(argv[2]), tam_palavra, i, j, k, chave;
    char *conjunto, ch;
    sscanf(argv[1], "%d", &tam_palavra);
    sscanf(argv[2], "%s", conjunto);

    for(i = 1; i <= tam_palavra; i++){
        for(j = 0; j < pow(conjunto_tam, i); j++){
            chave = j;
            for(k = 0; k < i; k++){
                printf("%c", conjunto[chave%conjunto_tam]);
                chave = chave/conjunto_tam;
            }
            printf("\n");
        }    
    }   
}