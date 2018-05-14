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

public slots:
    void left();
    void right();
    void down();
    void up();
    void mouse_position(float,float);

};

#endif // PLAYER_H
