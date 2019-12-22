#ifndef CAMPOCLASS_H_INCLUDED
#define CAMPOCLASS_H_INCLUDED
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_native_dialog.h>
#include <stdbool.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <string>
class CampoMinado{
    //data
    int **matInformation;
    char **matTemplate;
    int **visited;
    int numBomb = 0;
    int tam;
    //position
   
    //FPS
    int FPS;
    int startingTime;
    //windows
    int width;
    int height;
    char *title;
    //Allegro
    ALLEGRO_FONT *score;
    ALLEGRO_DISPLAY *mainWindow;
    ALLEGRO_EVENT_QUEUE *eventQueue;
    ALLEGRO_EVENT allegroEvent;

    //image
    ALLEGRO_BITMAP *imagem;
    
public:
     int posX, posY;
    CampoMinado(int n, int width, int height, int fps, char *title);

    CampoMinado(int width, int height, int fps, char *title);

    ~CampoMinado();

    //init
    bool coreInit();
    bool windowInit();
    bool fontInit(int size);
    //Getters
    ALLEGRO_DISPLAY* getWindow();
    ALLEGRO_EVENT_QUEUE* getEventQueue();
    ALLEGRO_EVENT getEvent();

    // allegro utils
    void waitForEvent();
    void flipAndClear();
    // checks
    bool isEventQueueEmpty();
    void makeMatrix();

    // fps control 
    void startTimer();
    double getTimer();
    void FPSLimit();

    void printMatrixI();

    void printMatrixT();

    void printMatrixGame();

    void DFS(int i, int j);
    
    int selectSquare();

    int endGame();

    void printAllegroMatrix(int alive);

    void updatePosition(int x, int y);

    void foundBombMessage(char *m1, char *m2, char*m3);

    int fibonacci(int n);

    void testPosition();

    void printSquare(int x, int y, char* file);

    void wait(double tempo);

    void drawText(char*);

    void printPosition();

    void suspect();

};


#endif