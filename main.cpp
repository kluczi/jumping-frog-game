#include <iostream>
#include "src/board.hpp"
#include "src/game_logic.hpp"
#include <ctime>
using namespace std;


int main()
{   
    srand(time(0));
    Board board;
    Status status;
    initscr(); // inicjalizacja okna
    refresh();
    boardInit(board, status);
    getch();  // oczekuje na przycisk
    endwin(); // zamyka okno
}
