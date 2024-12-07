#pragma once
#include <stdlib.h>

#define GRASS 'G'
#define ROAD 'R'
#define FROG 'F'
#define HOLE 'H'
#define FINISH_LINE 'L'

#define HEIGHT 14
#define WIDTH 14
#define OFFSET 1
#define CARS 8

#define BASIC_ENEMY_CAR_SPEED 7
#define BASIC_FRIENDLY_CAR_SPEED 10
#define BASIC_NEUTRAL_CAR_SPEED 8
#define BASIC_PLAYER_SPEED 10

#define FROG_COLOR COLOR_MAGENTA
#define GRASS_COLOR COLOR_GREEN
#define ROAD_COLOR COLOR_BLACK
#define FINISH_COLOR COLOR_YELLOW
#define ENEMY_CAR_COLOR COLOR_RED
#define FRIENDLY_CAR_COLOR COLOR_BLUE
#define NEUTRAL_CAR_COLOR COLOR_YELLOW
#define HOLE_COLOR COLOR_WHITE

#define FORWARD 'w'
#define BACKWARD 's'
#define LEFT 'a'
#define RIGHT 'd'
#define CAR_JUMP ' '
#define QUIT 'q'
#define RESET 'r'

#define RA(min, max) ((min) + rand() % ((max) - (min) + 1)) // function taken from catch the ball

struct Board {
    WINDOW *board_win;
    char board[HEIGHT][WIDTH];
};

struct Status {
    WINDOW *status_win;
};

struct Car {
    int pos_x;
    int pos_y;
    int direction; // 1 stands for right, -1 stands for left
    char type;     // E stands for enemy car, F stands for friendly car, N stands for neutral car
    int speed_timer;
};

struct Player {
    int pos_x;
    int pos_y;
    int speed_timer;
    int points;
    int moves;
    int timer;
    Car *attached_to;
    bool is_dead;
};
