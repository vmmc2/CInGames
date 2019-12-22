#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <stdbool.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

using namespace std;

#define qtPlayers 2
#define lineParts 30
#define spacingParts 5

#define tamPlayer 50
#define playerMovement 20

#define collision_nheco 5

#define raiobola 5

enum io{HEXPORT, INPORT, GREEN_LEDS, RED_LEDS, BUTTONS, SWITCHES, DISPLAY_7SEG};

/* player struct */
typedef struct{
    int x, y;
    int ogx, ogy;
    int movement;
} Object;

typedef enum{
    NO_MOVEMENT,
    UP,
    DOWN,
    LEFT,
    UPLEFT,
    DOWNLEFT,
    RIGHT,
    UPRIGHT,
    DOWNRIGHT
} Movements;

class Game{
    public:
        // constructor
        Game(int width, int height, int fps, char *title);
        ~Game();

        // allegro inits
        bool coreInit();
        bool windowInit();
        bool fontInit(int size);

        // getters
        ALLEGRO_DISPLAY* getWindow();
        ALLEGRO_EVENT_QUEUE* getEventQueue();
        ALLEGRO_EVENT getEvent();

        // allegro utils
        void waitForEvent();
        void flipAndClear();

        // draw functions
        void drawMiddleLine();

        // checks
        bool isEventQueueEmpty();

        /* fps control */
        void startTimer();
        double getTimer();
        void FPSLimit();

        /* objects control */
        // drawing
        void drawPlayers();
        void drawBall();
        void drawScoreBoard();
        // setters
        void setBallPosition(int x, int y);
        void setPlayerPosition(int id, int x, int y);
        void setBallRadius(double radius);
        void setPlayerMovement(int id, int movement);
        void setBallMovement(int movement);
        void moveBall();
        // getters
        Object getPlayer(int id);
        Object getBall();
        int getPlayerMovement(int id);
        int getBallMovement();
		int getScore(int id);
        // collisions
        bool checkCollision();
        bool checkPlayerCollision(int id, int movement);
        // goal
        void checkGoal();
        void addScore(int id);
        // random
        int randomMovement();

    private:
        // ALLEGRO ITEMS
        ALLEGRO_FONT *scoreBoard;
        ALLEGRO_DISPLAY *mainWindow;
        ALLEGRO_EVENT_QUEUE *eventQueue;
        ALLEGRO_EVENT allegroEvent;

        // Window parameters
        char *title;
        int width;
        int height;

        // FPS Control
        double startingTime;
        double FPS;

        // Objects
        Object players[qtPlayers];
        Object ball;
        double ballRadius;

        // Game parameters
        int placar1;
        int placar2;
        int ballSpeed;
        int collision_count;
        
        int dev;

};
