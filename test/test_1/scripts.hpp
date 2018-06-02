#ifndef SCRIPTS_HPP
#define SCRIPTS_HPP

#include <AGE/engine.hpp>
#include <sstream>

namespace scripts {

//ENTITY BEHAVIORS
//static std::function<void(age::Entity::_property_t&)> rand_behav =
//        [](age::Entity::_property_t& p)
//{
//    int x = (rand()%(TO_DOUBLE(p["velocity"])->value + 1) - TO_DOUBLE(p["velocity"])->value);
//    int y = (rand()%(TO_DOUBLE(p["velocity"])->value + 1) - TO_DOUBLE(p["velocity"])->value);
//    std::cout << x << " " << y << std::endl;
//    TO_DOUBLE(p["x"])->value+= x;
//    TO_INTEGER(p["y"])->value+= y;
//};

static std::function<void(age::_property_t&)> mob1 =
        [](age::_property_t& p)
{

    TO_STRING(p["state"])->value = "red";

    int x_max = rand()%200 + 700, x_min = rand()%200; /*y_min = 200 , y_max = 400;*/

    if(TO_DOUBLE(p["angle"])->value == PI/2.)
        TO_DOUBLE(p["x"])->value+=TO_DOUBLE(p["velocity"])->value;


    if(TO_DOUBLE(p["angle"])->value == -PI/2.)
        TO_DOUBLE(p["x"])->value-=TO_DOUBLE(p["velocity"])->value;


    if(TO_DOUBLE(p["x"])->value > x_max){
        TO_DOUBLE(p["angle"])->value = -PI/2.;
    }

    if(TO_DOUBLE(p["x"])->value < x_min){
        TO_DOUBLE(p["angle"])->value =  PI/2.;
    }
    if(TO_DOUBLE(p["x"])->value > 800 || TO_DOUBLE(p["y"])->value > 600
            || TO_DOUBLE(p["x"])->value < 0 || TO_DOUBLE(p["y"])->value < 0){
        TO_DOUBLE(p["x"])->value = rand()%x_max + x_min;
        TO_DOUBLE(p["y"])->value = rand()%600;
    }

};

static std::function<void(age::_property_t&)> mob2 =
        [](age::_property_t& p)
{
    TO_STRING(p["state"])->value = "red";

    int max = rand()%200 + 400, min = rand()%200; /*y_min = 200 , y_max = 400;*/

    if(TO_DOUBLE(p["angle"])->value == PI/2.)
        TO_DOUBLE(p["y"])->value+=TO_DOUBLE(p["velocity"])->value;


    if(TO_DOUBLE(p["angle"])->value == -PI/2.)
        TO_DOUBLE(p["y"])->value-=TO_DOUBLE(p["velocity"])->value;


    if(TO_DOUBLE(p["y"])->value > max){
        TO_DOUBLE(p["angle"])->value = -PI/2.;
    }

    if(TO_DOUBLE(p["y"])->value < min){
        TO_DOUBLE(p["angle"])->value =  PI/2.;
    }

    if(TO_DOUBLE(p["x"])->value > 800 || TO_DOUBLE(p["y"])->value > 600
            || TO_DOUBLE(p["x"])->value < 0 || TO_DOUBLE(p["y"])->value < 0){
        TO_DOUBLE(p["x"])->value = rand()%800;
        TO_DOUBLE(p["y"])->value = rand()%max + min;
    }
};

static std::function<void(age::_property_t&)> follow_line =
        [](age::_property_t& p)
{
    TO_STRING(p["state"])->value = "red";


    TO_DOUBLE(p["x"])->value += TO_DOUBLE(p["velocity"])->value*cos(TO_DOUBLE(p["angle"])->value);
    TO_DOUBLE(p["y"])->value += TO_DOUBLE(p["velocity"])->value*sin(TO_DOUBLE(p["angle"])->value);
    if(TO_DOUBLE(p["x"])->value > 800 || TO_DOUBLE(p["y"])->value > 600
          || TO_DOUBLE(p["x"])->value < 0 || TO_DOUBLE(p["y"])->value < 0){

        TO_DOUBLE(p["x"])->value -= 2*TO_DOUBLE(p["velocity"])->value*cos(TO_DOUBLE(p["angle"])->value);
        TO_DOUBLE(p["y"])->value -= 2*TO_DOUBLE(p["velocity"])->value*sin(TO_DOUBLE(p["angle"])->value);
        boost::random::uniform_int_distribution<> dist(-0.1+TO_DOUBLE(p["angle"])->value + PI,0.1 + TO_DOUBLE(p["angle"])->value + PI);
        TO_DOUBLE(p["angle"])->value = dist(age::gen);
    }
};

static std::function<void(const age::_property_t&, age::_property_t&)> avoid =
        [](const age::_property_t& cp, age::_property_t& p)
{
//    TO_STRING(p["state"])->value = "blue";
    float x_dir = TO_DOUBLE(p["x"])->value - TO_DOUBLE(cp.at("x"))->value;
    float y_dir = TO_DOUBLE(p["y"])->value - TO_DOUBLE(cp.at("y"))->value;
    if(x_dir != 0 )
        TO_DOUBLE(p["x"])->value += TO_DOUBLE(p["velocity"])->value*(x_dir/fabs(x_dir));
    else
        TO_DOUBLE(p["x"])->value += TO_DOUBLE(p["velocity"])->value*cos(TO_DOUBLE(p["angle"])->value);
    if(y_dir != 0)
        TO_DOUBLE(p["y"])->value += TO_DOUBLE(p["velocity"])->value*(y_dir/fabs(y_dir));
    else TO_DOUBLE(p["y"])->value += TO_DOUBLE(p["velocity"])->value*sin(TO_DOUBLE(p["angle"])->value);
};


static std::function<void(const age::_property_t&, age::_property_t&)> avoid2 =
        [](const age::_property_t& cp, age::_property_t& p){
//    float x_dir = TO_INTEGER(p["x"])->value - TO_INTEGER(cp.at("x"))->value;
//    float y_dir = TO_INTEGER(p["y"])->value - TO_INTEGER(cp.at("y"))->value;

    TO_DOUBLE(p["angle"])->value = TO_DOUBLE(p["angle"])->value + PI;

//    if(x_dir > 0)
//        TO_DOUBLE(p["angle"])->value = atan(y_dir/x_dir);
//    else if(x_dir < 0)
//        TO_DOUBLE(p["angle"])->value = -atan(y_dir/x_dir);
//    follow_line(p);
};

//GLOBAL SCRIPTS
inline void create_blurg(const std::string& name, age::Engine::Ptr engine){
    age::Entity::Ptr new_ent(new age::Entity("blurg",10,10,3));
    new_ent->add_property("state",STRING_PTR("red"));
    new_ent->add_property("life",DOUBLE_PTR(50));
    new_ent->add_property("sprite",STRING_PTR("sprite_1"));
//    new_ent->set_behavior(rand_behav);
    new_ent->set_on_collision(avoid2);
    new_ent->set_in_collision(avoid);
    new_ent->set_property("x",DOUBLE_PTR(rand()%700 + 50));
    new_ent->set_property("y",DOUBLE_PTR(rand()%500 + 50));
    boost::random::uniform_int_distribution<> dist(-PI,PI);
    new_ent->set_property("angle",DOUBLE_PTR(dist(age::gen)));
    new_ent->set_collider(new age::CircleCollider(10));
    engine->add_entity(name,new_ent);
}

inline void init_test(age::Engine::Ptr engine,age::AnimatedManager am){
    for(int i = 1; i < 100; i++){
        std::stringstream key_name;
        key_name << "blurg_" << i;
        create_blurg(key_name.str(),engine);
//        if(i%2)
        engine->get_entities()[key_name.str()]->set_behavior(follow_line);
//        else engine->get_entities()[key_name.str()]->set_behavior(mob2);
        engine->get_entities()[key_name.str()]->link_to_sprite(am.get("sprite_1"));

//        std::cout << TO_DOUBLE(engine->get_entities()[key_name.str()]->access_property()["x"])->value << std::endl;

    }
    engine->_run();
}



}
#endif //SCRIPTS_HPP
