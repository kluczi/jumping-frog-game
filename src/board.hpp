#pragma ONCE

#include <fstream>
#include <ncurses.h>
#include <unistd.h>

#include "config.hpp"
#include "game_logic.hpp"

using namespace std;

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

void endGame(Board &board, Player &player) {
    werase(board.board_win);
    mvwprintw(board.board_win, HEIGHT / 2 + 1, (WIDTH - 10) / 2, "Kum kum zabka");
    wrefresh(board.board_win);
    getch();
    exit(0);
}

void calculatePoints(Player &player, int moves_counter, int timer) {
    int points = points = ((moves_counter * 100) / (CARS + 1)) - (timer * 0.5); // points are calculated by following pattern: (moves forward*100/number of cars+1)-(time/2)
    player.points = max(points, 0);                                             // preventing points to be negative
}

// functions to load data
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
        if (x < HEIGHT) {
            fseek(file, 1, SEEK_CUR);
        }
    }
    fclose(file);
}

void placeRandomCar(Board &board, Car &car, Car cars[]) {
    bool well_placed = false;
    while (!well_placed) {
        int temp_x = RA(0, HEIGHT - 1);
        int temp_y = RA(0, WIDTH - 1);
        int cars_size = sizeof(cars) / sizeof(cars[0]);
        if (board.board[temp_x][temp_y] == 'R') { // checking if random x,y are street
            bool is_free = true;
            for (int i = 0; i < CARS + 1; i++) { // checking if other car is on the x,y street
                if (cars[i].pos_x == temp_x) {
                    is_free = false;
                    break;
                }
            }
            if (is_free) {
                car.pos_x = temp_x;
                car.pos_y = temp_y;
                car.direction = RA(0, 1) ? -1 : 1;
                if (board.neutral_cars_count < 1 && RA(0, 1)) {
                    board.neutral_cars_count++;
                    car.type = 'N';
                    car.speed_timer = BASIC_NEUTRAL_CAR_SPEED;
                } else if (board.friendly_cars_count < MAX_FRIENDLY_CARS && RA(0, 1)) {
                    board.friendly_cars_count++;
                    car.type = 'F';
                    car.speed_timer = BASIC_FRIENDLY_CAR_SPEED;
                } else {
                    car.type = 'E';
                    car.speed_timer = BASIC_ENEMY_CAR_SPEED;
                }
                well_placed = true;
            }
        }
    }
}

void loadCarsFromFile(const char *file_name, Car cars[CARS]) {
    FILE *file = fopen(file_name, "r");
    if (file == NULL) {
        return;
    }
    for (int i = 0; i < CARS; i++) {
        fscanf(file, "%d %d %d %c %d", &cars[i].pos_x, &cars[i].pos_y, &cars[i].direction, &cars[i].type, &cars[i].speed_timer);
    }
}

void maintainCar(Board &board, Car cars[]) {
    int active_cars = 0;
    for (int i = 0; i < CARS; i++) {
        if (cars[i].pos_y >= 0 && cars[i].pos_y < WIDTH) {
            active_cars++;
        }
    }

    for (int i = 0; i < CARS && active_cars < MIN_CARS; i++) {
        if (cars[i].pos_y < 0 || cars[i].pos_y >= WIDTH) {
            placeRandomCar(board, cars[i], cars);
            active_cars++;
        }
    }
}

void movePlayer(Board &board, Player &player, chtype input) {
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
        endGame(board, player);
        break;
    }
    if (player.pos_x == 0) {
        endGame(board, player);
    }
}

void drawBoard(Board board, Player player, Car cars[]) {
    werase(board.board_win);
    for (int x = 0; x < HEIGHT; x++) {
        for (int y = 0; y < WIDTH; y++) {
            switch (board.board[x][y]) {
            case 'G':
                mvwaddch(board.board_win, x, y, ' ' | COLOR_PAIR(2) | A_REVERSE);
                break;
            case 'R':
                mvwaddch(board.board_win, x, y, ' ' | COLOR_PAIR(3) | A_REVERSE);
                break;
            case 'L':
                mvwaddch(board.board_win, x, y, ' ' | COLOR_PAIR(4) | A_REVERSE);
                break;
            case 'H':
                mvwaddch(board.board_win, x, y, '0' | COLOR_PAIR(8));
                break;
            }
        }
    }
    mvwaddch(board.board_win, player.pos_x, player.pos_y, ' ' | COLOR_PAIR(1) | A_REVERSE);

    for (int i = 0; i < CARS; i++) {
        if (cars[i].pos_y >= 0 && cars[i].pos_y < WIDTH) {
            if (cars[i].type == 'E') {
                mvwaddch(board.board_win, cars[i].pos_x, cars[i].pos_y, ' ' | COLOR_PAIR(5) | A_REVERSE);
            } else if (cars[i].type == 'F') {
                mvwaddch(board.board_win, cars[i].pos_x, cars[i].pos_y, ' ' | COLOR_PAIR(6) | A_REVERSE);
            } else {
                mvwaddch(board.board_win, cars[i].pos_x, cars[i].pos_y, ' ' | COLOR_PAIR(7) | A_REVERSE);
            }
        }
    }

    wrefresh(board.board_win);
}

void resetBoard(Board board, Player player, Car cars[]) {
}

bool checkCollisions(Board &board, Player &player, Car cars[]) {
    for (int i = 0; i < CARS; i++) {
        if (cars[i].type == 'E' && player.pos_x == cars[i].pos_x && player.pos_y == cars[i].pos_y) {
            return true;
        }
    }
    for (int x = 0; x < HEIGHT; x++) {
        for (int y = 0; y < WIDTH; y++) {
            if (board.board[x][y] == 'H' && player.pos_x == x && player.pos_y == y) { // checking if frog did not jump into the hole
                return true;
            }
        }
    }

    return false;
}

bool checkNeutralCar(Player &player, Car &car) {
    if (car.type == 'N' && player.pos_x == car.pos_x && ((player.pos_y - 1 == car.pos_y && car.direction == 1) || (player.pos_y + 1 == car.pos_y && car.direction == -1))) {
        return true;
    }
    return false;
}

bool checkFriendlyCar(Player &player, Car &car, chtype input) {
    bool isFrogOnCar = false;
    if (!isFrogOnCar && car.type == 'F' && player.pos_y == car.pos_y && (player.pos_x + 1 == car.pos_x || player.pos_x - 1 == car.pos_x)) {
        player.pos_x = car.pos_x;
        player.pos_y = car.pos_y;
        isFrogOnCar = true;
        return true;
    }
}

bool checkBorder(Car car) {
    if (RA(0, 1) == 1 && (car.pos_y == 0 || car.pos_y == WIDTH - 1)) {
        return true;
    }
    return false;
}

void moveCars(Board &board, Player &player, Car cars[]) {
    bool stopped = false;
    for (int i = 0; i < CARS; i++) {
        if (cars[i].speed_timer == 0) {
            if (!checkNeutralCar(player, cars[i])) {
                if (RA(0, 1) == 0) {
                    if (cars[i].pos_y > WIDTH - 1 || cars[i].pos_y < 0) {
                        cars[i].pos_y = -1;
                    } else {
                        cars[i].pos_y += cars[i].direction;
                    }
                } else {
                    cars[i].pos_y += cars[i].direction;
                }
                if (cars[i].pos_y > WIDTH - 1 || cars[i].pos_y < 0) {
                    cars[i].direction *= -1;
                    cars[i].pos_y += cars[i].direction;
                }
            }
            if (cars[i].type == 'E') {
                if (player.timer % 5 == 0) {
                    cars[i].speed_timer = RA(1, 10);
                } else {
                    cars[i].speed_timer = RA(3, 10);
                }
            } else {
                if (player.timer % 10 == 0) {
                    cars[i].speed_timer = RA(10, 20);
                } else {
                    cars[i].speed_timer = RA(10, 15);
                }
            }
        }
    }
    maintainCar(board, cars);
}

void showStatus(Status &status, Player &player) {
    mvwprintw(status.status_win, 1, 1, "Bartosz Kluska 203185");
    mvwprintw(status.status_win, 3, 1, "Moves: %d", player.moves);
    mvwprintw(status.status_win, 4, 1, "Time: %d", player.timer);
    mvwprintw(status.status_win, 5, 1, "Points: %d", player.points);
    wrefresh(status.status_win);
}

void gameLoop(Board &board, Player &player, Car cars[], Status &status) {
    bool game_ended = false;
    int tick = 0;
    while (!game_ended) {
        chtype input = getch();
        if (input == QUIT) {
            game_ended = true;
        }
        if (player.speed_timer == 0) {
            movePlayer(board, player, input);
            if (input != ERR) {
                player.speed_timer = 10;
                if (input == 'w') { // points are calculated by forward movement to prevent points being scored by right/left/down movement
                    player.moves++;
                }
            }
        }
        if (checkCollisions(board, player, cars)) {
            game_ended = true;
            break;
        }
        moveCars(board, player, cars);
        maintainCar(board, cars);
        if (player.speed_timer > 0) {
            player.speed_timer--;
        }

        for (int i = 0; i < CARS; i++) {
            cars[i].speed_timer--;
        }

        drawBoard(board, player, cars);

        tick++;
        if (tick == 100) {
            player.timer++;
            tick = 0;
        }

        usleep(10000);

        calculatePoints(player, player.moves, player.timer);

        showStatus(status, player);
    }
}

void boardInit(Board &board, Status &status) {
    noecho();             // nei wyswietla nci terminalowego
    curs_set(0);          // ukrywa kursor
    start_color();        // wlacza kolory
    use_default_colors(); // nie zalacza tla tylko robi to terminalowo
    colorInit();

    int x_max, y_max;
    int start_row, start_col, end_row, end_col;
    getmaxyx(stdscr, y_max, x_max);
    start_row = ((y_max / 2) - (HEIGHT / 2));
    start_col = ((x_max / 2) - (WIDTH / 2));
    board.board_win = newwin(HEIGHT + (2 * OFFSET), WIDTH + (2 * OFFSET), start_row - 1, start_col - 1);
    status.status_win = newwin(HEIGHT + (2 * OFFSET), 2 * WIDTH + (2 * OFFSET), start_row - 2, start_col + WIDTH + (2 * OFFSET) - 1);
    wrefresh(status.status_win);
    wrefresh(board.board_win);
    loadMapFromFile("src/map.txt", board.board);
    board.friendly_cars_count = 0;
    board.neutral_cars_count = 0;
    Car cars[CARS];
    for (int i = 0; i < MIN_CARS; i++) {
        placeRandomCar(board, cars[i], cars);
    }
    // loadCarsFromFile("src/cars.txt",cars);
    Player player = {HEIGHT - 1, (WIDTH / 2), BASIC_PLAYER_SPEED, 0, 0, 0};

    timeout(0);
    drawBoard(board, player, cars);

    keypad(board.board_win, true);
    nodelay(board.board_win, true);
    gameLoop(board, player, cars, status);
}
