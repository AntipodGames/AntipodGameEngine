#ifndef SCRIPTS_HPP
#define SCRIPTS_HPP

#include <AGE/engine.hpp>
#include <functional>

namespace scripts {

static std::function<void(age::_property_t&)> go_forward =
        [](age::_property_t& p){
    double vel = TO_DOUBLE(p["velocity"])->value;
    double angle = TO_DOUBLE(p["angle"])->value;
    TO_DOUBLE(p["x"])->value += vel*cos(angle);
    TO_DOUBLE(p["y"])->value += vel*sin(angle);


};

inline double calc_angle(float x1, float y1, float x2, float y2){
    float angle;

    if(x1 - x2 > 0)
        angle = atan((y1 - y2)/(x1 - x2)) + PI;
    else if(x1 - x2 < 0)
        angle = atan((y1 - y2)/(x1 - x2));
    else {
        if(y1 - y2 > 0)
            angle = -PI/2;
        else angle = PI/2;
    }

    return angle;
}


static std::function<void(age::Entity*)> attractor = [](age::Entity* self){
    age::vector_field vf(10,10,800,600);

    double x = 0, y = 0, xself, yself,vel;
    for(int i = 0 ; i < vf.size()[0]; i++){
        for(int j = 0; j < vf.size()[1]; j++){
            x = i*vf.get_scale()[0];
            y = j*vf.get_scale()[1];
            xself = TO_DOUBLE(self->access_property()["x"])->value;
            yself = TO_DOUBLE(self->access_property()["y"])->value;
            age::_property_t p;
            if(fabs(x - xself) > vf.get_scale()[0] || fabs(y - yself) > vf.get_scale()[1]){

                vel = TO_DOUBLE(self->access_property()["velocity"])->value*cos(PI/4.);

                p.emplace("x",DOUBLE_PTR(vel/**cos(calc_angle(x,y,xself,yself))*/));
                p.emplace("y",DOUBLE_PTR(vel/**sin(calc_angle(x,y,xself,yself))*/));
            }
            else {
                p.emplace("x",DOUBLE_PTR(0));
                p.emplace("y",DOUBLE_PTR(0));
            }

            vf[i][j] = p;
        }
    }
//    emit self->send_vector_field(TO_STRING(self->access_property()["name"])->value,vf);

};


static std::function<void(const age::_property_t&, age::_property_t&)> adjust_angle =
        [](const age::_property_t& cp, age::_property_t& p)
{
    double cx = TO_DOUBLE(cp.at("x"))->value;
    double cy = TO_DOUBLE(cp.at("y"))->value;
    double x = TO_DOUBLE(p.at("x"))->value;
    double y = TO_DOUBLE(p.at("y"))->value;
    if(TO_STRING(cp.at("name"))->value != "blurg"){
        TO_DOUBLE(p["angle"])->value = calc_angle(cx,cy,x,y) + PI/2. - PI/4.;
        TO_STRING(p["state"])->value = "yellow";
    }
};

//GLOBAL SCRIPTS
inline void create_blurg(const std::string& name, age::Engine::Ptr engine){
    age::Entity::Ptr new_ent(new age::Entity("blurg",10,10,9));
    new_ent->add_property("state",STRING_PTR("red"));
    new_ent->add_property("life",DOUBLE_PTR(50));
    new_ent->add_property("sprite",STRING_PTR("sprite_1"));
    new_ent->set_property("x",DOUBLE_PTR(rand()%700 + 50));
    new_ent->set_property("y",DOUBLE_PTR(rand()%500 + 50));
    boost::random::uniform_int_distribution<> dist(-PI,PI);
    new_ent->set_property("angle",DOUBLE_PTR(0));
    new_ent->set_collider(new age::CircleCollider(10));
    new_ent->set_in_collision(adjust_angle);
    new_ent->set_behavior(go_forward);
    engine->add_entity(name,new_ent);
}

inline void create_attractor(const std::string& name, age::Engine::Ptr engine){
    age::Entity::Ptr new_ent(new age::Entity(name,10,10,10));
    new_ent->add_property("state",STRING_PTR("blue"));
    new_ent->add_property("life",DOUBLE_PTR(50));
    new_ent->add_property("sprite",STRING_PTR("sprite_1"));
    new_ent->set_property("x",DOUBLE_PTR(rand()%400 + 200));
    new_ent->set_property("y",DOUBLE_PTR(rand()%300+150));
    boost::random::uniform_int_distribution<> dist(-PI,PI);
    new_ent->set_property("angle",DOUBLE_PTR(0));
    new_ent->set_collider(new age::CircleCollider(100));
    new_ent->set_custom_run(attractor);
//    new_ent->set_behavior(go_forward);
//    new_ent->set_in_collision(adjust_angle);

    engine->add_entity(name,new_ent);

}


inline void init_test(age::Engine::Ptr engine,age::AnimatedManager am){

    for(int i = 1; i < 10; i++){
        std::stringstream key_name;
        key_name << "attractor_" << i;
        create_attractor(key_name.str(),engine);

        engine->get_entities()[key_name.str()]->link_to_sprite(am.get("sprite_1"));
//        engine->get_entities()[key_name.str()]->set_behavior(go_forward);

    }
    for(int i = 1; i < 2; i++){
        std::stringstream key_name;
        key_name << "blurg_" << i;
        create_blurg(key_name.str(),engine);

        engine->get_entities()[key_name.str()]->link_to_sprite(am.get("sprite_1"));
//        engine->get_entities()[key_name.str()]->set_behavior(go_forward);

    }
    engine->_run();
}


}

#endif // SCRIPTS_HPP
