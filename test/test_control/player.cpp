#include "player.h"

void Player::left(){
    TO_DOUBLE(_property["x"])->value-=TO_DOUBLE(_property["velocity"])->value;
}
void Player::right(){
    TO_DOUBLE(_property["x"])->value+=TO_DOUBLE(_property["velocity"])->value;
}
void Player::down(){
    TO_DOUBLE(_property["y"])->value+=TO_DOUBLE(_property["velocity"])->value;
}
void Player::up(){
    TO_DOUBLE(_property["y"])->value-=TO_DOUBLE(_property["velocity"])->value;
}

void Player::mouse_position(float x, float y){
    std::cout << x << " " << y << std::endl;
}
