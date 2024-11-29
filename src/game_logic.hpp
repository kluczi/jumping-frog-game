// #pragma ONCE
// #include <ncurses.h>
// #include <fstream>
// #include "config.hpp"

// // co tick aplikacji sprawdzac czy x,y auta nie naklada sie z x i y zabki

// struct Player {
//     int pos_x=HEIGHT, pos_y=WIDTH/2;
//     void reset_pos() {
//         pos_x==HEIGHT;
//         pos_y=WIDTH/2;
//     }
// };

// struct Car {
//     int pos_x, pos_y;
//     int direction=0; // 0 znaczy ze jedzie z lewej strony, 1 oznacza ze jedzie z prawej strony
//     double speed=0.5;       
//     int tick;
    
// };

// // void handleColisions() {

// // }


// void endGame(Player &player) {
//     player.reset_pos();
    
// }



// void gameLopp() {
//     bool game_ended=false;
//     Player curr_player;
//     curr_player.pos_x=WIDTH/2;
//     curr_player.pos_y=HEIGHT;
//     while (!game_ended) {
//         clear();
//         handleInput(curr_player);
//     }
// }

// void handleInput(Player &player) {
//     chtype input = getch();
//     //zapisujemy poprzednie wartosci x, y, gdyby wyszly poza zakres planszy
//     int temp_pos_x=player.pos_x;
//     int temp_pos_y=player.pos_y;
//     switch(input) {
//         case 'w':
//             player.pos_y-=1;
//         case 's':
//             player.pos_y+=1;
//         case 'a':
//             player.pos_x-=1;
//         case 'b':
//             player.pos_x+=1;
//     }
//     if(player.pos_y==0) {
//         endGame(player);
//     }
//     if (player.pos_x<0 || player.pos_x >= WIDTH || player.pos_y<0 || player.pos_y >= HEIGHT) {
//         player.pos_x=temp_pos_x;
//         player.pos_y=temp_pos_y;
//     }
// }

// // void createCar() {

// // }

// // void handleCars() {

// // }