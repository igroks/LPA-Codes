#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <arpa/inet.h>

typedef struct {
    uint8_t daddr[6];
    uint8_t saddr[6];
    uint16_t protocol;
}ethernet_hdr_t;

typedef struct {
    uint8_t hdr_len:4, version:4;
    uint8_t tos;
    uint16_t tot_len;
    uint16_t id;
    uint16_t frag_off;
    uint8_t ttl;
    uint8_t protocol;
    uint16_t check;
    uint8_t saddr[4];
    uint8_t daddr[4];
}ip_hdr_t;

typedef struct {
    uint16_t sport;
    uint16_t dport;
    uint32_t seq;
    uint32_t ack_seq;
    uint8_t reservado:4, hdr_len:4;
    uint8_t flags;
    uint16_t window;
    uint16_t check;
    uint16_t urg_ptr;
}tcp_hdr_t;

int main(int argc, char const *argv[]) {
    FILE *package_file = fopen(argv[1], "rb");
    ethernet_hdr_t ethernet_hdr;
    ip_hdr_t ip_hdr;
    tcp_hdr_t tcp_hdr;
    int i, tam_dados;

    printf("Lendo Ethernet ..\n");
    fread(&ethernet_hdr, sizeof(ethernet_hdr_t), 1, package_file);
    printf("  --> MAC de Origem:  ");
    for(i = 0; i < 6; i++){
        if(i == 5){
            printf("%.2x\n", ethernet_hdr.saddr[i]);
        }else{
            printf("%.2x:", ethernet_hdr.saddr[i]);
        }
    }
    printf("  --> MAC de Destino: ");
    for(i = 0; i < 6; i++){
        if(i == 5){
            printf("%.2x\n", ethernet_hdr.daddr[i]);
        }else{
            printf("%.2x:", ethernet_hdr.daddr[i]);
        }
    }

    printf("Lendo IP ..\n");
    fread(&ip_hdr, sizeof(ip_hdr_t), 1, package_file);
    printf("  --> Versão do IP: %d\n", ip_hdr.version);
    printf("  --> Tamanho do cabeçalho: %d bytes\n", ip_hdr.hdr_len*4);
    printf("  --> Tamanho do pacote: %d bytes\n", ntohs(ip_hdr.tot_len));
    printf("  --> Endereço IP de Origem:  ");
    for(i = 0; i < 4; i++){
        if(i == 3){
            printf("%d\n", ip_hdr.saddr[i]);
        }else{
            printf("%d.", ip_hdr.saddr[i]);
        }
    }
    printf("  --> Endereço IP de Destino: ");
    for(i = 0; i < 4; i++){
        if(i == 3){
            printf("%d\n", ip_hdr.daddr[i]);
        }else{
            printf("%d.", ip_hdr.daddr[i]);
        }
    }

    fseek(package_file, ip_hdr.hdr_len*4 - sizeof(ip_hdr_t), SEEK_CUR);
    fread(&tcp_hdr, sizeof(tcp_hdr_t), 1, package_file);

    printf("Lendo TCP ..\n");
    printf("  --> Porta de Origem:  %d\n", ntohs(tcp_hdr.sport));
    printf("  --> Porta de Destino: %d\n", ntohs(tcp_hdr.dport));
    printf("  --> Tamanho do cabeçalho: %d bytes\n", tcp_hdr.hdr_len*4);

    fseek(package_file, tcp_hdr.hdr_len*4 - sizeof(tcp_hdr_t), SEEK_CUR);
    tam_dados = ntohs(ip_hdr.tot_len) - (ip_hdr.hdr_len*4) - (tcp_hdr.hdr_len*4);

    printf("Lendo Dados (HTTP) ..\n");
    printf("  --> Tamanho dos dados: %d bytes\n", tam_dados);
    printf("  --> Dados:\n");
    for(i = 0; i < tam_dados; i++){
        printf("%c", fgetc(package_file));
    }
    
    fclose(package_file);
}
