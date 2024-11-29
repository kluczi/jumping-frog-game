#pragma ONCE
#include <ncurses.h>
#include <fstream>
#include "config.hpp"
#include "game_logic.hpp"

struct Board {
    WINDOW *board_win;
    chtype board[HEIGHT][WIDTH];

};

void colorInit() {
    init_pair(1,FROG_COLOR,-1);   
    init_pair(2,GRASS_COLOR,-1); 
    init_pair(3,ROAD_COLOR,-1);  
    init_pair(4, FINISH_COLOR, -1);
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
    for(int x=0; x<HEIGHT; x++) {
        for(int y=0; y<WIDTH; y++) {
            
            if(HEIGHT % 2 == 0) {
                if((x-1)%2==0) {
                    mvwaddch(board.board_win,x+OFFSET,y+OFFSET, ' ' | COLOR_PAIR(2) | A_REVERSE);
                } else {
                    mvwaddch(board.board_win,x+OFFSET,y+OFFSET, ' ' | COLOR_PAIR(3) | A_REVERSE);
                }
            } else {
                if((x-1)%2!=0) {
                    mvwaddch(board.board_win,x+OFFSET,y+OFFSET, ' ' | COLOR_PAIR(2) | A_REVERSE);
                } else {
                    mvwaddch(board.board_win,x+OFFSET,y+OFFSET, ' ' | COLOR_PAIR(3) | A_REVERSE);
                }
            }

            if (x==0) {
                for(int z=0; z<WIDTH; z++) {
                        mvwaddch(board.board_win,OFFSET,z+OFFSET, ' ' | COLOR_PAIR(4) | A_REVERSE);
                    
                }
            }
        }
    } //dodaje znak do okna na konkretnej pozycji
    mvwaddch(board.board_win, HEIGHT, (WIDTH/2)+1, ' ' | COLOR_PAIR(1) | A_REVERSE);
    // mvwaddch(board.board_win, HEIGHT, (WIDTH/2)-1, ' ' | COLOR_PAIR(1) | A_REVERSE);
    // mvwaddch(board.board_win, HEIGHT, (WIDTH/2)+1, ' ' | COLOR_PAIR(1) | A_REVERSE);
    wrefresh(board.board_win);
    keypad(board.board_win,true);
    
}

// void fillBoard(Board board) {

// }

// void drawBoard(Board board) {
//     clear();
//     for(int x=0; x<HEIGHT; x++) {
//         for(int y=0; y<WIDTH;y++) {
//             if(x%2==0) {
//                 mvwaddch(board.board_win,x+OFFSET,y+OFFSET, ' ' | COLOR_PAIR(2) | A_REVERSE);
//             }
//         }
//     }

// }