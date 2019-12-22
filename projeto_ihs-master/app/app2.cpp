#include "include/game.h"
#include<bits/stdc++.h>
#include<omp.h>
using namespace std;

/*
    g++ main.cpp include/game.cpp -o a -lm -lallegro_image -lallegro -lallegro_ttf -lallegro_font -lallegro_audio -lallegro_acodec -lallegro_primitives -lallegro_ttf -lallegro_font
    ./a
*/

/* defines */
#define WIDTH 640
#define HEIGHT 480
#define FPS 60.0
#define FONT_SIZE 32
char TITLE[] = "IHS Project";

/* game class*/
Game ihs_game = Game(WIDTH, HEIGHT, FPS, TITLE);

/* initial player positions */
int positions[][2] = {
    {10, HEIGHT/2},
    {WIDTH - 10, HEIGHT/2}
};

/* run game */
bool run = true;


bool verif_t(int desj,int botoes);

int main(){
	int dev = open("/dev/de2i150_altera", O_RDWR);
    if(!ihs_game.coreInit()) return -1;
    if(!ihs_game.windowInit()) return -1;
    if(!ihs_game.fontInit(FONT_SIZE)) return -1;
	
	
    for(int x = 0; x < qtPlayers; x++){
        ihs_game.setPlayerPosition(x, positions[x][0], positions[x][1]);
    }
    uint32_t test = 10;
	int tecla=15, l, sw;
	bool t1,t2,t3,t4;
    while(run){
        // FPS timer start
        ihs_game.startTimer();

        // While eventQueue isn't empty
            #pragma omp parallel num_threads(2)
			{
				#pragma omp single
				{
					l = read(dev, &tecla, BUTTONS);
				}
				#pragma omp single
				{
					l = read(dev, &sw, SWITCHES);
					if(sw != 0) ihs_game.setBallRadius(10);
					else ihs_game.setBallRadius(5);
						cout << sw << "\n";
				}
				#pragma omp sections
				{
					#pragma omp section
					{
						t1 = verif_t(1,tecla); 
					}
					#pragma omp section
					{
						t2 = verif_t(2,tecla);
					}
					#pragma omp section
					{
						t3 = verif_t(3,tecla);
					}
					#pragma omp section
					{
						t4 = verif_t(4,tecla);
					}
				}
			}
			
                
            if(t2){
              	ihs_game.setPlayerMovement(1, UP);
            }
            if(t1){
                ihs_game.setPlayerMovement(1, DOWN);
            }
            if(t4){
                ihs_game.setPlayerMovement(0, UP);
            }
            if(t3){
                ihs_game.setPlayerMovement(0, DOWN);
            }

        	if(!t1 && !t2){
            	ihs_game.setPlayerMovement(1, NO_MOVEMENT);
            }
            if(!t3 && !t4){
            	ihs_game.setPlayerMovement(0, NO_MOVEMENT);
            }
          
			if(!ihs_game.isEventQueueEmpty()){
		        if(ihs_game.getEvent().type == ALLEGRO_EVENT_DISPLAY_CLOSE){ // display closed
		            run = false;
		        }
			}

        ihs_game.drawMiddleLine();

        if(ihs_game.getPlayerMovement(0) == UP){
            if(!ihs_game.checkPlayerCollision(0, UP))
                ihs_game.setPlayerPosition(0, ihs_game.getPlayer(0).x, ihs_game.getPlayer(0).y - playerMovement);
        }else if(ihs_game.getPlayerMovement(0) == DOWN){
            if(!ihs_game.checkPlayerCollision(0, DOWN))
                ihs_game.setPlayerPosition(0, ihs_game.getPlayer(0).x, ihs_game.getPlayer(0).y + playerMovement);
        }
        if(ihs_game.getPlayerMovement(1) == UP){
            if(!ihs_game.checkPlayerCollision(1, UP))
                ihs_game.setPlayerPosition(1, ihs_game.getPlayer(1).x, ihs_game.getPlayer(1).y - playerMovement);
        }else if(ihs_game.getPlayerMovement(1) == DOWN){
            if(!ihs_game.checkPlayerCollision(1, DOWN))
                ihs_game.setPlayerPosition(1, ihs_game.getPlayer(1).x, ihs_game.getPlayer(1).y + playerMovement);
        }

        ihs_game.drawScoreBoard();
        ihs_game.drawPlayers();
        ihs_game.drawBall();
        ihs_game.moveBall();
        ihs_game.flipAndClear();
        
        unsigned char hexdigit[] = {0x3F, 0x06, 0x5B, 0x4F,
	0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F};

	uint32_t ans = 0;
	
    int placar1 = ihs_game.getScore(0);
    int placar2 = ihs_game.getScore(1);
    if(placar1 < 10){
		ans = ans | (hexdigit[0]);
		ans = ans << 8;
		ans = ans | (hexdigit[placar1]);
	}else{
    	ans = ans | (hexdigit[1]);
		ans = ans << 8;
		ans = ans | (hexdigit[0]);
	}
	
	if(placar2 < 10){
    	ans = ans << 8;
    	ans = ans | (hexdigit[0]);
		ans = ans << 8;
		ans = ans | (hexdigit[placar2]);
    }else{
    	ans = ans << 8;
    	ans = ans | (hexdigit[1]);
		ans = ans << 8;
		ans = ans | (hexdigit[0]);
    }
    
    ans = ~ans;
    
    write(dev, &ans, DISPLAY_7SEG);
        
    }

    ihs_game.~Game();
///
    return 0;
}

bool verif_t(int desj,int botoes){
	int teste = 1<<(desj-1);
	int ans = teste & botoes;
	if(ans == 0)return true;
	else return false;
}
