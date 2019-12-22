#include "campoClass.h"

CampoMinado::CampoMinado(int n, int width, int height, int fps, char *title){
    tam = n;
    this->width = width;
    this->height = height;
    this->FPS = fps;
    this->title = title;
    this->posX = 0;
    this->posY = 0;
    makeMatrix();
}
CampoMinado::CampoMinado(int width, int height, int fps, char *title){
    tam = 8;
    this->width = width;
    this->height = height;
    this->FPS = fps;
    this->title = title;
    this->posX = 0;
    this->posY = 0;
    makeMatrix();
}

CampoMinado::~CampoMinado(){
    al_destroy_font(score);
    al_destroy_display(mainWindow);
    al_destroy_event_queue(eventQueue);
}

bool CampoMinado::coreInit(){
    al_init_image_addon();

    if(!al_init()){
        fprintf(stderr, "Allegro couldn't be started\n");
        return false;
    }

    if(!al_init_image_addon()){
        fprintf(stderr, "Allegro couldn't init add-on allegro_image\n");
        return false;
    }

    if(!al_init_font_addon()){
        fprintf(stderr, "Allegro couldn't init add-on allegro_font\n");
        return false;
    }

    if(!al_init_ttf_addon()){
        fprintf(stderr, "Allegro couldn't init add-on allegro_ttf\n");
        return false;
    }

    if(!al_init_primitives_addon()){
        fprintf(stderr, "Allegro couldn't init add-on allegro_primitives\n");
        return false;
    }

    eventQueue = al_create_event_queue();
    if(!eventQueue){
        fprintf(stderr, "Allegro couldn't start event queue\n");
        return false;
    }

    if (!al_install_keyboard()){
        fprintf(stderr, "Falha ao inicializar teclado.\n");
        return false;
    }
    return true;
}

bool CampoMinado::windowInit(){
    // window and event queue creation
    mainWindow = al_create_display(width, height);
    if(!mainWindow){
        fprintf(stderr, "Allegro couldn't create window\n");
        return false;
    }
    al_set_window_title(mainWindow, title);

    // register window on the event queue
    al_register_event_source(eventQueue, al_get_display_event_source(mainWindow));
    al_register_event_source(eventQueue, al_get_keyboard_event_source());

    return true;
}

bool CampoMinado::fontInit(int size){
    
    score = al_load_font("resources/score.ttf", size, 0);

    if(!score){
        fprintf(stderr, "Allegro couldn't load the font\n");
        return false;
    } 

    return true;
}

void CampoMinado::startTimer(){
    startingTime = al_get_time();
}

double CampoMinado::getTimer(){
    return (al_get_time() - startingTime);
}

void CampoMinado::FPSLimit(){
    if(getTimer() < 1.0/FPS){
        al_rest((1.0 / FPS) - getTimer());
    }
}

ALLEGRO_DISPLAY* CampoMinado::getWindow(){
    return mainWindow;
}

ALLEGRO_EVENT_QUEUE* CampoMinado::getEventQueue(){
    return eventQueue;
}

ALLEGRO_EVENT CampoMinado::getEvent(){
    return allegroEvent;
}

bool CampoMinado::isEventQueueEmpty(){
    return al_is_event_queue_empty(eventQueue);
}

void CampoMinado::waitForEvent(){
    al_wait_for_event(eventQueue, &allegroEvent);
}

void CampoMinado::flipAndClear(){
    al_flip_display();
    al_clear_to_color(al_map_rgb(255, 255, 255));
    FPSLimit();
}

//metodos pnc


void CampoMinado::makeMatrix(){
    int i, j;
    int px[] = {-1,-1,-1,0,0,0,1,1,1}, py[] = {-1,0,1,-1,0,1,-1,0,1};
    matTemplate = new char*[tam];
    matInformation = new int*[tam];
    numBomb=0;
    int dale = 0;
    for(i=0;i<tam;i++){
        matTemplate[i] = new char[tam];
        matInformation[i] = new int[tam];
        for(j=0;j<tam;j++){
            matInformation[i][j] = 0;
            matTemplate[i][j] = 'x';
        }
    }
    while(numBomb<tam){
        for(i=0;i<tam && numBomb<tam;i++){
            for(j=0;j<tam && numBomb<tam;j++){
                int val = rand()%100;
                if(val==1){
                    if(matInformation[i][j]==0){
                        matInformation[i][j] = -1;
                        numBomb++;
                    }
                }
            }
        }
    }
    for(i=0;i<tam;i++){
        for(j=0;j<tam;j++){
            int k;
            for(k=0;k<9;k++){
                if(matInformation[i][j]==-1){
                    if(i+px[k]>=0 && i+px[k]<tam && j+py[k]>=0 && j+py[k]<tam ){
                        if(matInformation[i+px[k]][j+py[k]]!=-1) matInformation[i+px[k]][j+py[k]]++; 
                    }
                }    
            }        
        }
    }
}

void CampoMinado::printMatrixI(){
    int i, j;
    for(i=0;i<tam;i++){
        for(j=0;j<tam;j++){
            if(j==0){
                if(matInformation[i][j]==-1) printf("%d", matInformation[i][j]);
                else printf(" %d", matInformation[i][j]);
            } 
            else{
                if(matInformation[i][j]==-1) printf(" %d", matInformation[i][j]);
                else printf("  %d", matInformation[i][j]);
            }
        }
        printf("\n");
    }
}  

void CampoMinado::printMatrixT(){
     int i, j;
    for(i=0;i<tam;i++){
        for(j=0;j<tam;j++){
            printf("%c ", matTemplate[i][j]);
        }
        printf("\n");
    }
}

void CampoMinado::printMatrixGame(){
    int i, j;
    printf(" ");
    for(i=0;i<tam;i++) printf(" %d ", i+1);
    printf("\n");
    for(i=0;i<tam;i++){
        for(j=0;j<tam;j++){
            if(j==0)printf("%d", i+1);
            if(matTemplate[i][j]!='p'){
                printf(" %c ", matTemplate[i][j]);
            }
            else{
                if(j==0){
                    if(matInformation[i][j]==-1) printf("%d", matInformation[i][j]);
                    else printf(" %d", matInformation[i][j]);
                } 
                else{
                    if(matInformation[i][j]==-1) printf(" %d", matInformation[i][j]);
                    else printf("  %d", matInformation[i][j]);
                }
            }
        }
        printf("\n");
    }
}

void CampoMinado::printAllegroMatrix(int alive){
    for(int i=0;i<tam;i++){
        for(int j=0;j<tam;j++){
            int lx = i*58, ly = (7-j)*58;
            if(matTemplate[i][j]=='x'){
                printSquare(lx, ly,(char*) "X");
            }
            else if(matTemplate[i][j]=='s'){
                printSquare(lx, ly,(char*) "Suspeito");
            }
            else switch (matInformation[i][j]){
            case -1:
                fprintf(stderr,"Entrei na danada\n");
                printSquare(lx, ly, (char*)"Bomba");
            break;
            case 0:
                printSquare(lx, ly, (char*)"0");
                
            break;
            case 1:
                printSquare(lx, ly, (char*)"1");
                
            break; 
            case 2:
                printSquare(lx, ly, (char*)"2");
                
            break;
            case 3:
                printSquare(lx, ly, (char*)"3");
                
            break;
            case 4:
                //printar quadrado 4
                printSquare(lx, ly, (char*)"4");
                
            break;  
            case 5:
                //printar quadrado 5
                printSquare(lx, ly, (char*)"5");
                
            break;  
            case 6:
                //printar quadrado 6
                printSquare(lx, ly, (char*)"6");
                
            break;
            case 7:
                //printar quadrado 7
                printSquare(lx, ly, (char*)"7");
                
            break;
            case 8:
                //printar quadrado 8
                printSquare(lx, ly, (char*)"8");
                
            break;    
            default:
                break;
            }
            
        }
        
    }

}

void CampoMinado::updatePosition(int x, int y){
    if(posX+x>=tam || posX+x<0 || posY+y>=tam || posY+y<0) return;
    this->posX += x;
    this->posY += y;
}

void CampoMinado::DFS(int i, int j){
    if(!(i>=0 && i<tam && j>=0 && j<tam)) return; // se nao for valida
    if(visited[i][j]!=0) return; // se ja foi visitada
    if(matInformation[i][j]==-1) return; // se eh uma bomba
    visited[i][j] = 1; // digo que foi visitado
    matTemplate[i][j] = 'p';
    if(matInformation[i][j]!=0) return; // se nao eh uma posicao sem bombas ao redor para continuar a dfs
    int k;
    int px[] = {-1,-1,-1,0,0,0,1,1,1}, py[] = {-1,0,1,-1,0,1,-1,0,1};
    for(k=0;k<9;k++){
        DFS(i+px[k], j+py[k]);
    }
}
    
int CampoMinado::selectSquare(){
    int i, j;
    i = this->posX;
    j = this->posY;
    if(matInformation[i][j]==-1){
        //printf("entrei no danado\n");
        for(int ix=0; ix<tam;ix++){
            for(int jx=0;jx<tam;jx++){
                if(matInformation[ix][jx]==-1) matTemplate[ix][jx] = 'p';
            }
        }
        return 0;
    }
    int l, c;
    visited = new int*[tam];
    for(l=0;l<tam;l++){
        visited[l] = new int[tam];
        for(c=0;c<tam;c++){
            visited[l][c] = 0;
        }
    }
    DFS(i, j);
    return 1;
}

int CampoMinado::endGame(){
    int i, j, cont = 0;
    for(i=0;i<tam;i++){
        for(j=0;j<tam;j++){
            if(matTemplate[i][j]=='x'||matTemplate[i][j]=='s') cont++;
            if(cont>tam) return 0;
        }
    }
    if(cont>tam) return 0;
    //printf("\nPARABENS, VOCE VENCEU!!!!\n");
    return 1;
}

void CampoMinado::foundBombMessage(char *m1, char *m2, char*m3){
    al_show_native_message_box(NULL,m1,m2,m3,NULL,ALLEGRO_MESSAGEBOX_ERROR);
}

int CampoMinado::fibonacci(int n){
    if(n==0 || n==1) return  n;
    else return fibonacci(n-1) + fibonacci(n-2);
}

void CampoMinado::printSquare(int x, int y, char* file){
    
    char msg[100] = "resources/sprites/quadrado";
    strcat(msg,file);
    strcat(msg,".png");
    //fprintf(stderr, "%s\n", msg);
    //string msg = "quadrado" + file;
    imagem = al_load_bitmap(msg);
    al_convert_mask_to_alpha(imagem,al_map_rgb(255,0,255));
    al_draw_bitmap(imagem, x, y, 0);
}

void CampoMinado::wait(double tempo){
    al_rest(tempo);
}
void CampoMinado::drawText(char* msg){
    al_show_native_message_box(NULL,"Fim de jogo",msg,"",NULL,ALLEGRO_MESSAGEBOX_ERROR);
}

void CampoMinado::printPosition(){
    printSquare(posX*58, (7-posY)*58, (char*)"Dourado");
}
void CampoMinado::suspect(){
    if(matTemplate[posX][posY]=='p') return;
    if(matTemplate[posX][posY]=='s') 
        matTemplate[posX][posY] = 'x';
    else
        matTemplate[posX][posY] = 's';

}