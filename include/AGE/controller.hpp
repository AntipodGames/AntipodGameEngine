#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP


#include <iostream>
#include <array>
#include "qsystem.hpp"
#include "entity.hpp"

namespace age {


class ControllerEntity : public Entity {

    Q_OBJECT

public:

    typedef std::shared_ptr<ControllerEntity> Ptr;
    typedef std::shared_ptr<const ControllerEntity> ConstPtr;

    ControllerEntity(age::QSystem* system){
        connect(system,SIGNAL(sig_left()),this,SLOT(left()));
        connect(system,SIGNAL(sig_right()),this,SLOT(right()));
        connect(system,SIGNAL(sig_up()),this,SLOT(up()));
        connect(system,SIGNAL(sig_down()),this,SLOT(down()));
        connect(system,SIGNAL(sig_key_pressed(int)),this,SLOT(key_press(int)));
        connect(system,SIGNAL(sig_key_released(int)),this,SLOT(key_release(int)));
        connect(system,SIGNAL(send_mouse_pos(float,float)),
                this,SLOT(mouse_position(float,float)));
        connect(system,SIGNAL(sig_mouse_left()),
                this,SLOT(mouse_left_click()));
        connect(system,SIGNAL(sig_mouse_right()),
                this,SLOT(mouse_right_click()));
//        connect(system,SIGNAL(sig_mouse_double_left()),
//                this,SLOT(mouse_left_double()));
//        connect(system,SIGNAL(sig_mouse_double_right()),
//                this,SLOT(mouse_right_double()));

    }
    ControllerEntity(const ControllerEntity& ce) :
        Entity(ce), mouse_pos(ce.mouse_pos){}

public slots:
    virtual void left(){}
    virtual void right(){}
    virtual void up(){}
    virtual void down(){}
    virtual void key_press(int){}
    virtual void key_release(int){}
    virtual void mouse_right_click(){}
    virtual void mouse_left_click(){}
    virtual void mouse_position(float,float){}

protected:
    std::array<float,2> mouse_pos;
};

}//age namespace
#endif // CONTROLLER_HPP
