#pragma once
#include "config.hpp"
#include <ncurses.h>

void placeRandomCar(Board &board, Car &car, Car cars[]) {
    bool well_placed = false;
    while (!well_placed) {
        int temp_x = RA(0, HEIGHT - 1);
        int temp_y = RA(0, WIDTH - 1);
        if (board.board[temp_x][0] == 'R') { // checking if random x,y are street
            bool is_free = true;
            for (int i = 0; i < CARS; i++) { // checking if other car is on the x,y street
                if (cars[i].pos_x == temp_x) {
                    is_free = false;
                    break;
                }
            }
            if (is_free) {
                car.pos_x = temp_x;
                car.pos_y = temp_y;
                car.direction = RA(0, 1) ? -1 : 1;

                // board.neutral_cars_count++;
                // car.type = 'N';
                // car.speed_timer = BASIC_NEUTRAL_CAR_SPEED;
                if (board.neutral_cars_count < MAX_NEUTRAL_CARS) { // && RA(0, 1)
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

bool checkCollisions(Board &board, Player &player, Car cars[]) {
    for (int i = 0; i < CARS; i++) {
        if (cars[i].type == 'E' && player.pos_x == cars[i].pos_x && player.pos_y == cars[i].pos_y) {
            return true;
        }
    }
    for (int x = 0; x < HEIGHT; x++) {
        for (int y = 0; y < WIDTH; y++) {
            if (!player.attached_to && board.board[x][y] == 'H' && player.pos_x == x && player.pos_y == y) { // checking if frog did not jump into the hole
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

bool checkFriendlyCar(Player &player, Car &car) {
    if (!player.attached_to && car.type == 'F' && (player.pos_x - 1 == car.pos_x) && (abs(player.pos_y - car.pos_y) <= 1)) {
        return true;
    }
    return false;
}

void moveCars(Board &board, Player &player, Car cars[]) {
    for (int i = 0; i < CARS; i++) {
        if (cars[i].speed_timer == 0) {
            if (!checkNeutralCar(player, cars[i])) {
                if (RA(0, 1) == 0) {
                    if (cars[i].pos_y == WIDTH - 1 && cars[i].direction == 1) {
                        cars[i].pos_y = -1;
                        cars[i].speed_timer = RA(10, 15);
                        continue;
                    } else if (cars[i].pos_y == 0 && cars[i].direction == -1) {
                        cars[i].pos_y = WIDTH;
                        cars[i].speed_timer = RA(10, 15);
                        continue;
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
            // zmiana predkosci
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
}
