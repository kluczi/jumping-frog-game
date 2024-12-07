#pragma once

#include <fstream>
#include <ncurses.h>
#include <unistd.h>

#include "car.hpp"
#include "config.hpp"
#include "game_logic.hpp"

using namespace std;

// init color pairs for game elements and map colors
void colorInit() {
    init_pair(1, FROG_COLOR, COLOR_BLACK);
    init_pair(2, GRASS_COLOR, COLOR_BLACK);
    init_pair(3, ROAD_COLOR, COLOR_BLACK);
    init_pair(4, FINISH_COLOR, COLOR_BLACK);
    init_pair(5, ENEMY_CAR_COLOR, COLOR_BLACK);
    init_pair(6, FRIENDLY_CAR_COLOR, COLOR_BLACK);
    init_pair(7, NEUTRAL_CAR_COLOR, COLOR_BLACK);
    init_pair(8, HOLE_COLOR, COLOR_BLACK);
}

// function to load map from file
void loadMapFromFile(const char *file_name, char map[HEIGHT][WIDTH]) {
    FILE *file = fopen(file_name, "r");
    if (file == NULL) {
        return;
    }

    for (int x = 0; x < HEIGHT; x++) {
        for (int y = 0; y < WIDTH; y++) {
            char map_char = fgetc(file);
            map[x][y] = map_char;
        }

        // handle new lines
        if (x < HEIGHT) {
            fseek(file, 1, SEEK_CUR);
        }
    }

    fclose(file);
}

void showStatus(Status &status, Player &player) {
    mvwprintw(status.status_win, 1, 1, "Bartosz Kluska 203185");
    mvwprintw(status.status_win, 3, 1, "Moves: %d", player.moves);
    mvwprintw(status.status_win, 4, 1, "Time: %d", player.timer);
    mvwprintw(status.status_win, 5, 1, "Points: %d", player.points);

    wrefresh(status.status_win);
}

void drawBoard(Board board, Player player, Car cars[]) {
    werase(board.board_win); // clears board before redrawing

    for (int x = 0; x < HEIGHT; x++) {
        for (int y = 0; y < WIDTH; y++) {
            switch (board.board[x][y]) {
            case GRASS:
                mvwaddch(board.board_win, x, y, ' ' | COLOR_PAIR(2) | A_REVERSE);
                break;

            case ROAD:
                mvwaddch(board.board_win, x, y, ' ' | COLOR_PAIR(3) | A_REVERSE);
                break;

            case FINISH_LINE:
                mvwaddch(board.board_win, x, y, ' ' | COLOR_PAIR(4) | A_REVERSE);
                break;

            case HOLE:
                mvwaddch(board.board_win, x, y, '0' | COLOR_PAIR(8));
                break;
            }
        }
    }

    mvwaddch(board.board_win, player.pos_x, player.pos_y, ' ' | COLOR_PAIR(1) | A_REVERSE); // draws frog

    // draws cars based on type
    for (int i = 0; i < CARS; i++) {
        if (cars[i].pos_y >= 0 && cars[i].pos_y < WIDTH) {
            if (cars[i].type == 'E') {
                mvwaddch(board.board_win, cars[i].pos_x, cars[i].pos_y, ' ' | COLOR_PAIR(5) | A_REVERSE);
            } else if (cars[i].type == 'F') {
                if (player.attached_to == &cars[i]) {
                    mvwaddch(board.board_win, cars[i].pos_x, cars[i].pos_y, ' ' | COLOR_PAIR(1) | A_REVERSE); // frog is highlated if attached to friendly car
                } else {
                    mvwaddch(board.board_win, cars[i].pos_x, cars[i].pos_y, ' ' | COLOR_PAIR(6) | A_REVERSE);
                }
            } else {
                mvwaddch(board.board_win, cars[i].pos_x, cars[i].pos_y, ' ' | COLOR_PAIR(7) | A_REVERSE);
            }
        }
    }

    wrefresh(board.board_win);
}

void gameLoop(Board &board, Player &player, Car cars[], Status &status) {
    bool game_ended = false;
    int tick = 0;

    while (!game_ended) {
        chtype input = getch();

        if (input == QUIT) {
            game_ended = true;
        }

        // follow friendly car movement if player is attached to it
        if (player.attached_to) {
            player.pos_x = player.attached_to->pos_x;
            player.pos_y = player.attached_to->pos_y;
        }

        if (player.speed_timer == 0) {
            movePlayer(board, player, status, input);

            if (input != ERR) {
                player.speed_timer = 10;

                // points are calculated by forward movement to
                // prevent points being scored by right/left/down movement
                if (input == FORWARD) {
                    player.moves++;
                }

                if (player.attached_to && input == CAR_JUMP) { // jumping off friendly car
                    player.attached_to = NULL;
                    delete player.attached_to;
                    player.pos_x--;
                }

                for (int i = 0; i < CARS; i++) {
                    if (input == CAR_JUMP && checkFriendlyCar(player, cars[i])) { // jumping on friendly car
                        player.attached_to = &cars[i];
                    }
                }
            }
        }

        // checks for collisions
        if (checkCollisions(board, player, cars)) {
            player.is_dead = true;
            game_ended = true;
            endGame(board, player, status);
            break;
        }

        moveCars(board, player, cars);

        if (player.speed_timer > 0) {
            player.speed_timer--;
        }

        for (int i = 0; i < CARS + 1; i++) {
            cars[i].speed_timer--;
        }

        drawBoard(board, player, cars); // redraws board with new positions

        // handle ticks and game timer
        tick++;

        if (tick == 100) {
            player.timer++;
            tick = 0;
        }

        usleep(10000); // little delay to control game speed

        calculatePoints(player, player.moves, player.timer);

        showStatus(status, player);
    }
}

// setup game
void game(Board &board, Status &status) {
    noecho();
    timeout(0);
    curs_set(0);
    start_color();
    use_default_colors();

    colorInit();

    // center game and status window
    int start_row, start_col, end_row, end_col;
    int x_max, y_max;
    getmaxyx(stdscr, y_max, x_max);

    start_row = ((y_max / 2) - (HEIGHT / 2));
    start_col = ((x_max / 2) - (WIDTH / 2));

    board.board_win = newwin(HEIGHT + (2 * OFFSET), WIDTH + (2 * OFFSET), start_row - 1, start_col - 1);
    status.status_win = newwin(HEIGHT + (2 * OFFSET),
                               2.5 * WIDTH + (2 * OFFSET),
                               start_row - 2,
                               start_col + WIDTH + (2 * OFFSET) - 1);

    wrefresh(status.status_win);
    wrefresh(board.board_win);

    loadMapFromFile("src/map.txt", board.board);

    Car cars[CARS + 1];
    Player player = {HEIGHT - 1, (WIDTH / 2), BASIC_PLAYER_SPEED, 0, 0, 0, NULL, false};

    // init random cars
    for (int i = 0; i < CARS; i++) {
        placeRandomCar(board, cars[i], cars, player);
    }

    drawBoard(board, player, cars); // to draw board for the first time

    keypad(board.board_win, true);  // enable input
    nodelay(board.board_win, true); // turn delay off

    gameLoop(board, player, cars, status); // main game loop
}
