#include <stdio.h>
#include <string.h>

int main(int argc, char const *argv[]){

    FILE *mpeg_file = fopen(argv[1], "rb");
    unsigned char byte[4];

    fread(byte, sizeof(unsigned char), 4, mpeg_file);

    while(feof(mpeg_file) == 0){
        if(memcmp(byte, "\x00\x00\x01", 3) == 0){
            if(byte[3] == 0xba){
                printf("--> Código: %.2x -- Pack\n", byte[3]);

            }else if(byte[3] == 0xbb){
                printf("--> Código: %.2x -- System\n", byte[3]);

            }else if(byte[3] == 0xb3){
                unsigned char byte1 = fgetc(mpeg_file);
                unsigned char byte2 = fgetc(mpeg_file);
                unsigned char byte3 = fgetc(mpeg_file);
                unsigned char byte4 = fgetc(mpeg_file);
                unsigned int width = byte1 * 16 + (byte2 >> 4);
                unsigned int height = ( byte2 & 0x0F) * 256 + byte3;
                unsigned int frame_rate = byte4 & 0x0f;
                float frame_rate_value;

                if(frame_rate == 1){
                    frame_rate_value = 23.976;
                }else if(frame_rate == 2){
                    frame_rate_value = 24.000;
                }else if(frame_rate == 3){
                    frame_rate_value = 25.000;
                }else if(frame_rate == 4){
                    frame_rate_value = 29.970;
                }else if(frame_rate == 5){
                    frame_rate_value = 30.000;
                }else if(frame_rate == 6){
                    frame_rate_value = 50.000;
                }else if(frame_rate == 7){
                    frame_rate_value = 59.940;
                }else{
                    frame_rate_value = 60.000;
                }

                printf("--> Código: %.2x -- Sequence Header -- Width = %d, Height = %d -- Frame rate = %.3ffps\n", byte[3], width, height, frame_rate_value);

            }else if(byte[3] == 0xb8){
                printf("--> Código: %.2x -- Group of Pictures\n", byte[3]);

            }else if(byte[3] == 0x00){
                fseek(mpeg_file, 1, SEEK_CUR);
                unsigned char byte2 = fgetc(mpeg_file);
                unsigned int picture_type = (byte2 >> 3) & 0x07;
                char type;
                
                if(picture_type == 1){
                    type = 'I';
                }else if(picture_type == 2){
                    type = 'P';
                }else{
                    type = 'B';
                }

                printf("--> Código: %.2x -- Picture -- Tipo = %c\n", byte[3], type);

            }else if(byte[3] >= 0x01 && byte[3] <= 0xaf){
                printf("--> Código: %.2x -- Slice\n", byte[3]);

            }else if(byte[3] >= 0xC0 && byte[3] <= 0xdf){
                printf("--> Código: %.2x -- Packet Video\n", byte[3]);

            }else if(byte[3] >= 0xE0 && byte[3] <= 0xff){
                printf("--> Código: %.2x -- Packet Audio\n", byte[3]);

            }else{
                printf("--> Código: %.2x -- Tipo de stream não implementado\n", byte[3]);
            }
            
        }else{
            fseek(mpeg_file, -3, SEEK_CUR);
        }        
        fread(byte, sizeof(unsigned char), 4, mpeg_file);
    }
    fclose(mpeg_file);
}

