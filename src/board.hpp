#pragma ONCE
#include <ncurses.h>
#include <unistd.h>
#include <fstream>
#include "config.hpp"
#include "game_logic.hpp"
using namespace std;

struct Board {
    WINDOW *board_win;
    char board[HEIGHT][WIDTH];
};

struct Player {
    int pos_x;
    int pos_y;
};

void colorInit() {
    init_pair(1,FROG_COLOR,-1);   
    init_pair(2,GRASS_COLOR,-1); 
    init_pair(3,ROAD_COLOR,-1);  
    init_pair(4, FINISH_COLOR, -1);
}

void endGame(Board &board, Player &player) {
    werase(board.board_win); 
    mvwprintw(board.board_win, HEIGHT / 2 + 1, (WIDTH - 10) / 2, "Kum kum zabka!");
    box(board.board_win, 0, 0); 
    wrefresh(board.board_win); 
    getch();
    exit(0); 
}

void loadMap(const char * file_name, char map[HEIGHT][WIDTH]) {
    FILE * file=fopen(file_name, "r");
    if(file==NULL) {
        cout << "ERROR" << endl;
        return;
    }
    cout << "GOOD" << endl;
    for (int x = 0; x < HEIGHT; x++) {
        for (int y = 0; y < WIDTH; y++) {
            char map_char=fgetc(file);
            map[x][y] = map_char;
        }
        if(x<HEIGHT) {
            fseek(file,1,SEEK_CUR);
        }
    }
    fclose(file);
}

void fillBoard(Board board) {
     //generujemy plansze z pliku
    colorInit();
    for(int x=0; x<HEIGHT; x++) {
        for(int y=0; y<WIDTH; y++) {
            switch(board.board[x][y]){
                case('G'):
                    mvwaddch(board.board_win, x+OFFSET, y+OFFSET, ' ' | COLOR_PAIR(2) | A_REVERSE);
                    wrefresh(board.board_win);
                    break;
                case('R'):
                    mvwaddch(board.board_win, x+OFFSET, y+OFFSET, ' ' | COLOR_PAIR(3) | A_REVERSE);
                    wrefresh(board.board_win);
                    break;
                case('L'):
                    mvwaddch(board.board_win, x+OFFSET, y+OFFSET, ' ' | COLOR_PAIR(4) | A_REVERSE);
                    wrefresh(board.board_win);
                    break;
            }
        }
    }
}

void movePlayer(Board &board, Player &player) {
    chtype input=getch();
    switch (input) {
        case FORWARD: // 
            if (player.pos_x > 0 ) {
                player.pos_x--;
            }
            break;
        case BACKWARD: 
            if (player.pos_x < HEIGHT-1) {
                player.pos_x++;
            } 
            break;
        case RIGHT: 
            if (player.pos_y < WIDTH-1) {
                player.pos_y++;
            }
            break;
        case LEFT: 
            if (player.pos_y > 0) {
                player.pos_y--;
            }
            break;
        case QUIT: 
            endGame(board, player);  
            break;
    }
    if (player.pos_x==0) {
        endGame(board, player);
    }
    // DEBUG USUNAC POZNIEJ
    move(HEIGHT + 2, 0); 
    clrtoeol();          
    printw("Player position: x=%d, y=%d", player.pos_x, player.pos_y);
    refresh();           
}

void updateBoard(Board board, Player player) {
    for (int x = 0; x < HEIGHT; x++) {
        for (int y = 0; y < WIDTH; y++) {
            switch (board.board[x][y]) {
                case 'G':
                    mvwaddch(board.board_win, x + OFFSET, y + OFFSET, ' ' | COLOR_PAIR(2) | A_REVERSE);
                    break;
                case 'R':
                    mvwaddch(board.board_win, x + OFFSET, y + OFFSET, ' ' | COLOR_PAIR(3) | A_REVERSE);
                    break;
                case 'L':
                    mvwaddch(board.board_win, x + OFFSET, y + OFFSET, ' ' | COLOR_PAIR(4) | A_REVERSE);
                    break;
            }
        }
    }
    mvwaddch(board.board_win, player.pos_x + OFFSET, player.pos_y + OFFSET, ' ' | COLOR_PAIR(1) | A_REVERSE);
    wrefresh(board.board_win);
}




void gameLoop(Board &board, Player &player) {
    bool game_ended=false;
    while (!game_ended) {
        chtype input = wgetch(board.board_win);
        if (input == QUIT ) { 
            game_ended = true;
        } else {
            movePlayer(board, player);    
            updateBoard(board, player); 
        }
        usleep(30000);
    }
}

void boardInit(Board board) {
    noecho(); //nei wyswietla nci terminalowego
    curs_set(0); //ukrywa kursor
    start_color(); //wlacza kolory
    use_default_colors(); //nie zalacza tla tylko robi to terminalowo    
    colorInit();                    
    board.board_win = newwin(HEIGHT+2*OFFSET, WIDTH+2*OFFSET, 0, 0); // tworzenie nowego okna
    box(board.board_win, 0, 0);
    chtype zmienna[HEIGHT][WIDTH]; //lepszy typ dla literek
    loadMap("src/map.txt", board.board);
    fillBoard(board);
    Player player = {HEIGHT-1, (WIDTH / 2)};
    updateBoard(board,player);
    wrefresh(board.board_win);
    keypad(board.board_win,true);
    nodelay(board.board_win, true);
    gameLoop(board, player);
    
}

