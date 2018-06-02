#include "player.h"

double Player::calc_angle(double x1, double y1, double x2, double y2){
    float angle;

    if(x1 - x2 > 0)
        angle = atan((y1 - y2)/(x1 - x2)) + PI;
    else if(x1 - x2 < 0)
        angle = atan((y1 - y2)/(x1 - x2));

    return angle;
}


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
    double px = TO_DOUBLE(_property["x"])->value;
    double py = TO_DOUBLE(_property["y"])->value;

    TO_DOUBLE(_property["angle"])->value = calc_angle(px,py,x,y) - PI/2;
}

void Player::key_press(int key){
    if(key == Qt::Key_A)
        std::cout << "a" << std::endl;
}

void Player::key_release(int key){
    if(key == Qt::Key_A)
        std::cout << "a release" << std::endl;
}
