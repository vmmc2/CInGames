#include <bits/stdc++.h>
#include <sstream>
#include <cmath>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_native_dialog.h>
#include <stdint.h>
#define pb push_back
#define INF 99999999

using namespace std;

/*
    g++ main.cpp include/game.cpp -o Genius -lm -lallegro_image -lallegro -lallegro_ttf -lallegro_font -lallegro_audio -lallegro_acodec -lallegro_primitives -lallegro_ttf -lallegro_font -lallegro_dialog
    ./Genius
*/

char TITLE[] = "Genius";
enum io{HEXPORT, INPORT, GREEN_LEDS, RED_LEDS, BUTTONS, SWITCHES, DISPLAY_7SEG};

int main(){
	//Inicializando as paradas
	int dev = open("/dev/de2i150_altera", O_RDWR);
	al_init();
	al_init_image_addon();
	al_install_keyboard();
	srand(time(NULL));
	//Variaveis.
	ALLEGRO_DISPLAY *window = NULL;
	ALLEGRO_BITMAP *imagem_red = NULL;
	ALLEGRO_BITMAP *imagem_blue = NULL;
	ALLEGRO_BITMAP *imagem_green = NULL;
	ALLEGRO_BITMAP *imagem_yellow = NULL;
	ALLEGRO_BITMAP *imagem_dred = NULL;
	ALLEGRO_BITMAP *imagem_dblue = NULL;
	ALLEGRO_BITMAP *imagem_dgreen = NULL;
	ALLEGRO_BITMAP *imagem_dyellow = NULL;
	ALLEGRO_EVENT_QUEUE *fila_eventos = NULL;
	vector<int> colors;
	int tamanho = 4;
	int new_color;
	int user_color;
	int ok = 1; //variavel responsavel por dizer se o jogador acertou a sequencia.
	int run = 1; //variavel responsavel por dizer se o jogo ta rodando ou nao.
	for(int i = 0; i < tamanho; i++){
		asm volatile(
			"rdtsc;"
			"movl %%eax, %0;"
			:"=r" (new_color)
		);
		colors.pb(new_color);
	}

	window = al_create_display(720, 480);
	imagem_red = al_load_bitmap("red.bmp");
	imagem_blue = al_load_bitmap("blue.bmp");
	imagem_green = al_load_bitmap("green.bmp");
	imagem_yellow = al_load_bitmap("yellow.bmp");
	imagem_dred = al_load_bitmap("dark_red.bmp");
	imagem_dblue = al_load_bitmap("dark_blue.bmp");
	imagem_dgreen = al_load_bitmap("dark_green.bmp");
	imagem_dyellow = al_load_bitmap("dark_yellow.bmp");
	fila_eventos = al_create_event_queue();
	al_register_event_source(fila_eventos, al_get_keyboard_event_source());
    al_register_event_source(fila_eventos, al_get_display_event_source(window));
	
	al_draw_bitmap(imagem_red, 0, 0, 0);
	al_draw_bitmap(imagem_blue, 0, 240, 0);
	al_draw_bitmap(imagem_green, 360, 0, 0);
	al_draw_bitmap(imagem_yellow, 360, 240, 0);
	al_flip_display();
	al_rest(5.0);
	while(run){
		//Primeira parte: Mostrar para o usuario a sequencia gerada
		//aleatoriamente para que ele possa memoriza-la
		for(int i = 0; i < (int)tamanho; i++){
			if(colors[i] == 0){ //vermelho
				al_draw_bitmap(imagem_dred, 0, 0, 0);
				al_draw_bitmap(imagem_blue, 0, 240, 0);
				al_draw_bitmap(imagem_green, 360, 0, 0);
				al_draw_bitmap(imagem_yellow, 360, 240, 0);
				al_flip_display();
				al_rest(1.0);
				//redesenha tudo como tava
				al_draw_bitmap(imagem_red, 0, 0, 0);
				al_draw_bitmap(imagem_blue, 0, 240, 0);
				al_draw_bitmap(imagem_green, 360, 0, 0);
				al_draw_bitmap(imagem_yellow, 360, 240, 0);
				al_flip_display();
				al_rest(1.0);
			}else if(colors[i] == 1){ //azul
				al_draw_bitmap(imagem_red, 0, 0, 0);
				al_draw_bitmap(imagem_dblue, 0, 240, 0);
				al_draw_bitmap(imagem_green, 360, 0, 0);
				al_draw_bitmap(imagem_yellow, 360, 240, 0);
				al_flip_display();
				al_rest(1.0);
				//redesenha tudo como tava
				al_draw_bitmap(imagem_red, 0, 0, 0);
				al_draw_bitmap(imagem_blue, 0, 240, 0);
				al_draw_bitmap(imagem_green, 360, 0, 0);
				al_draw_bitmap(imagem_yellow, 360, 240, 0);
				al_flip_display();
				al_rest(1.0);
			}else if(colors[i] == 2){ //amarelo
				al_draw_bitmap(imagem_red, 0, 0, 0);
				al_draw_bitmap(imagem_blue, 0, 240, 0);
				al_draw_bitmap(imagem_green, 360, 0, 0);
				al_draw_bitmap(imagem_dyellow, 360, 240, 0);
				al_flip_display();
				al_rest(1.0);
				//redesenha tudo como tava
				al_draw_bitmap(imagem_red, 0, 0, 0);
				al_draw_bitmap(imagem_blue, 0, 240, 0);
				al_draw_bitmap(imagem_green, 360, 0, 0);
				al_draw_bitmap(imagem_yellow, 360, 240, 0);
				al_flip_display();
				al_rest(1.0);
			}else if(colors[i] == 3){ //verde
				al_draw_bitmap(imagem_red, 0, 0, 0);
				al_draw_bitmap(imagem_blue, 0, 240, 0);
				al_draw_bitmap(imagem_dgreen, 360, 0, 0);
				al_draw_bitmap(imagem_yellow, 360, 240, 0);
				al_flip_display();
				al_rest(1.0);
				//redesenha tudo como tava
				al_draw_bitmap(imagem_red, 0, 0, 0);
				al_draw_bitmap(imagem_blue, 0, 240, 0);
				al_draw_bitmap(imagem_green, 360, 0, 0);
				al_draw_bitmap(imagem_yellow, 360, 240, 0);
				al_flip_display();
				al_rest(1.0);
			}
		}
		int flag_acabei_fase = 0;
		int contador = 0;
		uint32_t tecla;
		uint32_t pressionado = 15; //nao pressionei nada ainda
		int k, flag=1;
		//Segunda parte: Aguardar o input do usuario para ver se ele acertou a sequencia.
		while(ok){
			while(contador != (int) colors.size()){
				k = read(dev, &tecla, BUTTONS);
				if(tecla == 15){
					flag = 1;
				}
				if(flag == 0){
					tecla = 20;
				}
				if(tecla != 20){ //se uma tecla foi pressionada..
					//verifica qual foi a tecla
					switch(tecla){
						case 7: //vermelho
							tecla = 0;
							//vou desenhar a alteracao que o usuario fez...
							al_draw_bitmap(imagem_dred, 0, 0, 0);
							al_draw_bitmap(imagem_blue, 0, 240, 0);
							al_draw_bitmap(imagem_green, 360, 0, 0);
							al_draw_bitmap(imagem_yellow, 360, 240, 0);
							al_flip_display();
							al_rest(1.0);
							//redesenha tudo como tava
							al_draw_bitmap(imagem_red, 0, 0, 0);
							al_draw_bitmap(imagem_blue, 0, 240, 0);
							al_draw_bitmap(imagem_green, 360, 0, 0);
							al_draw_bitmap(imagem_yellow, 360, 240, 0);
							al_flip_display();
							al_rest(1.0);
							if(tecla != colors[contador]){
								printf("entrei!! 1\n");
								ok = 0;
								goto perdeu;
							}
							flag = 0;
							contador++;
							break;
						case 11: //azul
							tecla = 1;
							//vou desenhar a alteracao que o usuario fez...
							al_draw_bitmap(imagem_red, 0, 0, 0);
							al_draw_bitmap(imagem_dblue, 0, 240, 0);
							al_draw_bitmap(imagem_green, 360, 0, 0);
							al_draw_bitmap(imagem_yellow, 360, 240, 0);
							al_flip_display();
							al_rest(1.0);
							//redesenha tudo como tava
							al_draw_bitmap(imagem_red, 0, 0, 0);
							al_draw_bitmap(imagem_blue, 0, 240, 0);
							al_draw_bitmap(imagem_green, 360, 0, 0);
							al_draw_bitmap(imagem_yellow, 360, 240, 0);
							al_flip_display();
							al_rest(1.0);
							if(tecla != colors[contador]){
								ok = 0;
								printf("entrei!! 2\n");
								goto perdeu;
							}
							flag = 0;
							contador++;
							break;
						case 13: //amarelo
							tecla = 2;
							//vou desenhar a alteracao que o usuario fez...
							al_draw_bitmap(imagem_red, 0, 0, 0);
							al_draw_bitmap(imagem_blue, 0, 240, 0);
							al_draw_bitmap(imagem_green, 360, 0, 0);
							al_draw_bitmap(imagem_dyellow, 360, 240, 0);
							al_flip_display();
							al_rest(1.0);
							//redesenha tudo como tava
							al_draw_bitmap(imagem_red, 0, 0, 0);
							al_draw_bitmap(imagem_blue, 0, 240, 0);
							al_draw_bitmap(imagem_green, 360, 0, 0);
							al_draw_bitmap(imagem_yellow, 360, 240, 0);
							al_flip_display();
							al_rest(1.0);
							if(tecla != colors[contador]){
								ok = 0;
								printf("entrei!! 3\n");
								goto perdeu;
							}
							flag = 0;
							contador++;
							break;
						case 14: //verde
							tecla = 3;
							//vou desenhar a alteracao que o usuario fez...
							al_draw_bitmap(imagem_red, 0, 0, 0);
							al_draw_bitmap(imagem_blue, 0, 240, 0);
							al_draw_bitmap(imagem_dgreen, 360, 0, 0);
							al_draw_bitmap(imagem_yellow, 360, 240, 0);
							al_flip_display();
							al_rest(1.0);
							//redesenha tudo como tava
							al_draw_bitmap(imagem_red, 0, 0, 0);
							al_draw_bitmap(imagem_blue, 0, 240, 0);
							al_draw_bitmap(imagem_green, 360, 0, 0);
							al_draw_bitmap(imagem_yellow, 360, 240, 0);
							al_flip_display();
							al_rest(1.0);
							if(tecla != colors[contador]){
								ok = 0;
								printf("entrei!! 4\n");
								goto perdeu;
							}
							flag = 0;
							contador++;
							break;
					}
				}
				if(contador == tamanho){
					flag_acabei_fase = 1;
					break;
				}
			}
			if(flag_acabei_fase == 1){
				//Se eu acabei de fase, devo ir adicionar mais uma cor ao meu vetor colors..
				asm volatile(
					"rdtsc;"
					"movl %%eax, %0;"
					:"=r" (new_color)
				);
				colors.pb(new_color);
				tamanho = (int)colors.size();
				break;
			}	
		}
		perdeu:
		if(ok == 0){ //Se eu errei a sequencia.. tenho que resetar o jogo.
			char tcaixa[50] = "Genius IHS";
			char titulo[100] = "GAME OVER!";
			char texto[200] = "Do you want to start a new game?";
			int r = al_show_native_message_box(NULL,tcaixa,titulo,texto,NULL,ALLEGRO_MESSAGEBOX_YES_NO);
			if(r == 1){ //Se o jogador escolheu jogar novamente...
				colors.clear();
				tamanho = 4;
				for(int i = 0; i < tamanho; i++){
					asm volatile(
						"rdtsc;"
						"movl %%eax, %0;"
						:"=r" (new_color)
					);
					colors.pb(new_color);
				}
			}else{//Se o jogador escolheu sair do jogo...
				goto finish;
			}
		}
		ok = 1;
	}
	finish:
	al_destroy_display(window);
	al_destroy_event_queue(fila_eventos);
	return 0;
}
