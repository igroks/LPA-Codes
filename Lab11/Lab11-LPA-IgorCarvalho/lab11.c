#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <GL/glut.h>

typedef struct point_t{
    int x;
    int y;
}point_t;

typedef struct segment_t{
    point_t a;
    point_t b;
}segment_t;

segment_t *segmentos;
int largura, altura, numero_segmentos;

int ccw(point_t a, point_t b, point_t c){
    return (c.y - a.y) * (b.x - a.x) > (b.y - a.y) * (c.x - a.x);
}

int intersect(point_t a, point_t b, point_t c, point_t d){
    return ccw(a,c,d) != ccw(b,c,d) && ccw(a,b,c) != ccw(a,b,d);
}

int is_valid_segment(segment_t segmento, int quantidade){
    int i;

    for(i = 0; i < quantidade; i++){
        if(intersect(segmento.a, segmento.b, segmentos[i].a, segmentos[i].b) == 1){
            return 0;
        }
    }
    return 1;
}

void display(void){
    segment_t novo_segmento;
    int quantidade = 0;

    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0.0, 0.0, 0.0);
    glBegin(GL_LINES);

    while(quantidade < numero_segmentos){

        novo_segmento.a.x = rand() % (largura+1);
        novo_segmento.a.y = rand() % (altura+1);
        novo_segmento.b.x = rand() % (largura+1);
        novo_segmento.b.y = rand() % (altura+1);

        if(is_valid_segment(novo_segmento, quantidade)){
            glVertex2f(novo_segmento.a.x, novo_segmento.a.y);
            glVertex2f(novo_segmento.b.x, novo_segmento.b.y);
            segmentos[quantidade++] = novo_segmento;
        }
        
    }
    glEnd();
    glFlush();
}

int main(int argc, char **argv){
    largura = atoi(argv[1]), altura = atoi(argv[2]), numero_segmentos = atoi(argv[3]);
    segmentos = (segment_t*)malloc(sizeof(segment_t)*numero_segmentos);

    srand(time(NULL));
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
    
    // Cria uma janela de tamanho “largura” x “altura”
    glutInitWindowSize(largura, altura);
    glutCreateWindow ("Segmentos Aleatorios");
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, largura, 0.0, altura, -1.0, 1.0);
    
    // Seta a cor do fundo da janela
    glClearColor(1.0, 1.0, 1.0, 1.0);

    // Seta a função “display” como sendo a função que irá pintar a janela (infinitamente)
    glutDisplayFunc(display);
    glutMainLoop();

}
