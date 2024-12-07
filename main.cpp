#include "src/board.hpp"
// #include "src/game_logic.hpp"
#include <ctime>
#include <iostream>
using namespace std;

int main() {
    srand(time(0));
    Board board;
    Status status;
    initscr(); // init window
    refresh();
    game(board, status);
    endwin(); // close window
}
