#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char const *argv[]){

    int file_out_size = 0, i = 1, tamanho_max, tamanho_buffer = 0;
    sscanf(argv[2], "%d", &tamanho_max);
    tamanho_max *= 1048576;
    unsigned char *buffer = (unsigned char*)malloc(sizeof(unsigned char)*tamanho_max), byte[4];
    char file_out_name[30];
    FILE *mpeg_file = fopen(argv[1], "rb");
    FILE *file_out = fopen("video_parte_1.mpg","wb");

    printf("Criando arquivo video_parte_1.mpg ..\n");

    while(1){
        fread(byte, sizeof(unsigned char), 4, mpeg_file);

        if(memcmp(byte,"\x00\x00\x01\xB3",4) != 0 && !feof(mpeg_file)){
            buffer[tamanho_buffer] = byte[0];
            tamanho_buffer++;
            fseek(mpeg_file, -3, SEEK_CUR);
            continue;
        }

        if((file_out_size + tamanho_buffer) > tamanho_max){
            fclose(file_out);
            sprintf(file_out_name,"video_parte_%d.mpg", ++i);
            file_out = fopen(file_out_name,"wb");
            printf("Criando arquivo %s ..\n", file_out_name);
            fwrite(buffer, sizeof(unsigned char), tamanho_buffer, file_out);
            file_out_size = tamanho_buffer;
        }else{
            fwrite(buffer, sizeof(unsigned char), tamanho_buffer, file_out);
            file_out_size += tamanho_buffer;
        }

        if(feof(mpeg_file) != 0){
            fclose(file_out);
            break;
        }

        memcpy(buffer, "\x00\x00\x01\xB3", 4);
        tamanho_buffer = 4;
    }
    free(buffer);
    fclose(mpeg_file);
}
