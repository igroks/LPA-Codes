#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <string.h>

struct png_chunk{
    int length;
    char type[4];
}__attribute__ ((packed));

struct png_chunk_hdr{
    int width;
    int height;
    char bit_depth;
    char colour_type;
    char compression_method;
    char filter_method;
    char interlace_method;  
}__attribute__ ((packed));

int main(int argc, char const *argv[]){
    int i = 1;
    struct png_chunk *chunk = malloc(sizeof(struct png_chunk));
    struct png_chunk_hdr *chunk_hdr = malloc(sizeof(struct png_chunk_hdr));
    FILE *png_file = fopen(argv[1], "rb");
    fseek(png_file, 8, SEEK_CUR);

    while(feof(png_file) == 0){
        fread(chunk, sizeof(struct png_chunk), 1, png_file);
        printf("Lendo chunk %d:\n", i); i++;
        printf("    --> Tamanho: %d\n", ntohl(chunk->length));
        printf("    --> Tipo: %.4s\n", chunk->type);
        if(strcmp(chunk->type,"IHDR") == 0){
            fread(chunk_hdr, sizeof(struct png_chunk_hdr), 1, png_file);
            printf("        --> Largura: %d\n", ntohl(chunk_hdr->width));
            printf("        --> Altura: %d\n", ntohl(chunk_hdr->height));
            fseek(png_file, 4, SEEK_CUR);
        }else if(strcmp(chunk->type,"IEND") == 0){
            break;
        }else{
            fseek(png_file, ntohl(chunk->length)+4, SEEK_CUR);
        }
    }
    free(chunk);
    free(chunk_hdr);
    fclose(png_file);
}
