#include <iostream>
#include "src/board.hpp"
#include "src/game_logic.hpp"
using namespace std;


int main()
{

    // Player player;
    Board board;
    initscr(); // inicjalizacja okna
    refresh();
    boardInit(board);

    // noecho(); //nei wyswietla nci terminalowego
    // curs_set(0); //ukrywa kursor
    // start_color(); //wlacza kolory
    // use_default_colors(); //nie zalacza tla tylko robi to terminalowo
    // init_pair(1,COLOR_GREEN,-1);
    // board_win.board_win = newwin(HEIGHT+2*OFFSET, WIDTH+2*OFFSET, 0, 0); // tworzenie nowego okna
    // box(board_win.board_win, 0, 0);
    // chtype zmienna[HEIGHT][WIDTH]; //lepszy typ dla literek
    // mvwaddch(board_win.board_win,0+OFFSET,0+OFFSET, 'A' | COLOR_PAIR(1)); //dodaje znak do okna na konkretnej pozycji
    // wrefresh(board_win.board_win);
    // keypad(board_win.board_win,true);

    getch();  // oczekuje na przycisk
    endwin(); // zamyka okno
}