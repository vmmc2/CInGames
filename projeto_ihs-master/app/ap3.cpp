#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>
#include "campoClass.h"
enum io{HEXPORT, INPORT, GREEN_LEDS, RED_LEDS, BUTTONS, SWITCHES, DISPLAY_7SEG};
int main(){
    int key = 1;
    srand(time(NULL));
    int dev = open("/dev/de2i150_altera", O_RDWR);
    CampoMinado jogo(464, 464, 60, (char*)"Campo Minado");
    jogo.coreInit();
    jogo.windowInit();
    jogo.fontInit(16);
    int tecla, loop=1;
    int loose = 0;
    jogo.printAllegroMatrix(0);
    jogo.printPosition();
    //jogo.flipAndClear();
    //jogo.printMatrixI();
    int k;
    int flag = 1;
    uint32_t teclax;
    uint32_t swReader;
    bool sw1, sw2;
    bool sw3, sw4;
    while (loop){
        jogo.startTimer();
        k = read(dev, &teclax, BUTTONS);
        k = read(dev, &swReader, SWITCHES);
        if(swReader & 1)sw1 = true;
        else sw1 = false;
        if(swReader & 2)sw3 = true;
        else sw3 = false;
        if(key){
            jogo.flipAndClear();
            key = 0;
            sw2 = !sw1;
            sw4 = !sw3;
        }
        if(teclax==15){
            flag = 1;
        }
        if(flag==0){
            teclax = 20;
        }
      //  0111 7
      //  1011 11
      //  1101 13
      //  1110 14
        if(teclax!=20){
            switch(teclax){
                case 7:
                    tecla = 1;
                    flag = 0;
                    break;

                case 11:
                    tecla = 2;
                    flag = 0;
                    break;

                case 13:
                    tecla = 3;
                    flag = 0;
                    break;

                case 14:
                    tecla = 4;
                    flag = 0;
                    break;
            }        
        }
        if(sw1==sw2){
            sw2 = !sw1;
            tecla = 6;
        }
        if(sw3==sw4){
            sw4 = !sw3;
            tecla = 7;
        }
        switch (tecla){
        case 1:
            jogo.updatePosition(0,1);
            jogo.printAllegroMatrix(0);
            jogo.printPosition();
            jogo.flipAndClear();
            break;
        case 2:
            jogo.updatePosition(0,-1);
            jogo.printAllegroMatrix(0);
            jogo.printPosition();
            jogo.flipAndClear();
            break;
        case 3:
            jogo.updatePosition(-1,0);
            jogo.printAllegroMatrix(0);
            jogo.printPosition();
            jogo.flipAndClear();
            break;
        case 4:
            jogo.updatePosition(1,0);
            jogo.printAllegroMatrix(0);
            jogo.printPosition();
            jogo.flipAndClear();
            break;
        case 6:
            if(!jogo.selectSquare()){
                loop = 0;
                loose = 1;
            }
            if(jogo.endGame())loop = 0;
            jogo.printAllegroMatrix(0);
            jogo.printPosition();
            jogo.flipAndClear();
            break;
        case 7:
            jogo.suspect();
            jogo.printAllegroMatrix(0);
            jogo.printPosition();
            jogo.flipAndClear();
        default:
            break; 
        }
        tecla = 0;
        //fprintf(stderr, "x->%d y->%d\n", jogo.posX, jogo.posY);
        
    }
    jogo.printAllegroMatrix(0);
    if(loose){
        jogo.printSquare(jogo.posX*58, (7-jogo.posY)*58, (char*)"BombaVermelha");
        jogo.flipAndClear();
        jogo.wait(0.5);
        jogo.drawText((char*)"Perdeu, que pena :(");
    }
    else{
        jogo.flipAndClear();
        jogo.wait(0.5);
        jogo.drawText((char*)"Parabéns, voce venceu :)");
    }
	return 0;
    
}