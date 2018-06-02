#ifndef PLAYER_H
#define PLAYER_H

#include "AGE/controller.hpp"
#include "AGE/system.hpp"

class Player : public age::ControllerEntity
{
    Q_OBJECT

public:

    typedef std::shared_ptr<Player> Ptr;
    typedef std::shared_ptr<const Player> ConstPtr;

    Player(age::System* sys) : age::ControllerEntity(sys){}

    double calc_angle(double,double,double,double);

public slots:
    void left();
    void right();
    void down();
    void up();
    void mouse_position(float,float);
    void key_press(int);
    void key_release(int);

};

#endif // PLAYER_H
