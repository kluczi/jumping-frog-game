#pragma once
#include "car.hpp"
#include "config.hpp"
#include <ncurses.h>
#include <stdlib.h>

// points calculated by moving forward and based on number of cars, subtracting half of the timer value to encourage faster gameplay
void calculatePoints(Player &player, int moves_counter, int timer) {
    int points = ((moves_counter * 100) / (CARS + 1)) - (timer * 0.5); // points are calculated by following pattern: (moves forward * 100 / (number of cars + 1)) - (time / 2)
    player.points = fmax(points, 0);                                   // preventing points from being negative
}

void endGame(Board &board, Player &player, Status &status) {
    werase(board.board_win);
    werase(status.status_win);
    wrefresh(status.status_win);

    if (!player.is_dead) {
        mvwprintw(status.status_win, 1, 1, "Congrats, you won in: %d seconds\n and reached: %d points\n", player.timer, player.points);
    } else {
        mvwprintw(status.status_win, 1, 1, "You died and reached: %d points\n", player.points);
    }

    wrefresh(board.board_win);
    wrefresh(status.status_win);
    getch();
    delwin(status.status_win);
    delwin(board.board_win);
    exit(0);
}

void movePlayer(Board &board, Player &player, Status &status, chtype input) {
    // handles player movement based on the input key
    switch (input) {
    case FORWARD:
        if (player.pos_x > 0) {
            player.pos_x--;
        }
        break;

    case BACKWARD:
        if (player.pos_x < HEIGHT - 1) {
            player.pos_x++;
        }
        break;

    case RIGHT:
        if (player.pos_y < WIDTH - 1) {
            player.pos_y++;
        }
        break;

    case LEFT:
        if (player.pos_y > 0) {
            player.pos_y--;
        }
        break;

    case QUIT:
        // ends game if player chooses to quit
        player.is_dead = true;
        endGame(board, player, status);
        break;
    }

    // checks if player is on top of the board (win condition)
    if (player.pos_x == 0) {
        endGame(board, player, status);
    }
}
