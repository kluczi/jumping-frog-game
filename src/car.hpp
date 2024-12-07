#pragma once
#include "config.hpp"
#include <ncurses.h>

void randomCarType(Car &car) {
    int type = RA(0, 2);
    if (type <= 1) { //~66% chances for enemy car
        car.type = 'E';
        car.speed_timer = RA(5, 15);
    } else if (type == 2) { //~33% chance combined for friendly or neutral car
        if (RA(0, 1)) {     // 50% for friendly car
            car.type = 'F';
            car.speed_timer = RA(10, 15);
        } else { // 50% for neutral car
            car.type = 'N';
            car.speed_timer = RA(15, 20);
        }
    }
}

void placeRandomCar(Board &board, Car &car, Car cars[], Player &player) {
    bool well_placed = false;
    while (!well_placed) {
        // temporary random x and y
        int temp_x = RA(0, HEIGHT - 1);
        int temp_y = RA(0, WIDTH - 1);

        if (board.board[temp_x][0] == ROAD || board.board[temp_x][0] == HOLE) { // checks if the selected position is a valid street
            bool is_free = true;
            for (int i = 0; i < CARS; i++) { // checks if no other car is on the same position
                if (cars[i].pos_x == temp_x) {
                    is_free = false;
                    break;
                }
            }
            if (is_free) {
                car.pos_x = temp_x;
                car.pos_y = temp_y;
                car.direction = RA(0, 1) ? 1 : -1; // random direction
                if (player.attached_to == &car) {  // checks if the player isn't attached to the friendly car
                    break;
                } else {
                    randomCarType(car); // assign random type and speed that depends on type to the car
                }
                well_placed = true;
            }
        }
    }
}

// checks for collisions between the player and either enemy cars or holes
bool checkCollisions(Board &board, Player &player, Car cars[]) {
    for (int i = 0; i < CARS; i++) {
        if (cars[i].type == 'E' && player.pos_x == cars[i].pos_x && player.pos_y == cars[i].pos_y) {
            return true; // collision with enemy car
        }
    }
    for (int x = 0; x < HEIGHT; x++) {
        for (int y = 0; y < WIDTH; y++) {
            if (!player.attached_to && board.board[x][y] == 'H' && player.pos_x == x && player.pos_y == y) { // checking if frog did not jump into the hole
                return true;                                                                                 // collision with hole (frog jumped into hole)
            }
        }
    }

    return false;
}

// checks if the player can interact with a neutral car
bool checkNeutralCar(Player &player, Car &car) {
    if (car.type == 'N' && player.pos_x == car.pos_x && ((player.pos_y - 1 == car.pos_y && car.direction == 1) || (player.pos_y + 1 == car.pos_y && car.direction == -1))) {
        return true;
    }
    return false;
}

// checks if the player can interact with a friendly car
bool checkFriendlyCar(Player &player, Car &car) {
    if (!player.attached_to && car.type == 'F' && (player.pos_x - 1 == car.pos_x) && (abs(player.pos_y - car.pos_y) <= 1)) {
        return true;
    }
    return false;
}

// moves all cars on the board based on their direction and type
void moveCars(Board &board, Player &player, Car cars[]) {
    for (int i = 0; i < CARS; i++) {
        if (cars[i].speed_timer == 0) {              // when car is ready to move
            if (!checkNeutralCar(player, cars[i])) { // checks if car isn't blocked by neutral car
                int type = RA(0, 3);
                if (RA(0, 1) == 0) {
                    if (cars[i].pos_y == WIDTH - 1 && cars[i].direction == 1) {
                        cars[i].pos_y = -1; // reset position to start
                        cars[i].speed_timer = RA(10, 15);
                        if (player.attached_to != &cars[i]) { // checks if player isn't attatched to friendly car
                            randomCarType(cars[i]);           // random type for car which disappeared
                        }
                        continue;
                    } else if (cars[i].pos_y == 0 && cars[i].direction == -1) {
                        cars[i].pos_y = WIDTH - 1;
                        cars[i].speed_timer = RA(10, 15);
                        if (player.attached_to != &cars[i]) {
                            randomCarType(cars[i]);
                        }
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
            // changes speed depended on type
            if (cars[i].type == 'E') {
                if (player.timer % 5 == 0) {
                    cars[i].speed_timer = RA(1, 10);
                } else {
                    cars[i].speed_timer = RA(3, 10);
                }
            } else {
                if (player.timer % 10 == 0 && cars[i].type == 'F') {
                    cars[i].speed_timer = RA(10, 15);
                } else if (player.timer % 10 == 0 && cars[i].type == 'N') {
                    cars[i].speed_timer = RA(10, 20);
                } else {
                    cars[i].speed_timer = RA(13, 17);
                }
            }
        }
    }
}
