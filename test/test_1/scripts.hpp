#include <AGE/engine.hpp>

namespace scripts {

//ENTITY BEHAVIORS
static std::function<void(age::Entity::_property_t&)> rand_behav =
        [](age::Entity::_property_t& p)
{
    int x = (rand()%(TO_INTEGER(p["velocity"])->value*2 + 1) - TO_INTEGER(p["velocity"])->value);
    int y = (rand()%(TO_INTEGER(p["velocity"])->value*2 + 1) - TO_INTEGER(p["velocity"])->value);
    std::cout << x << " " << y << std::endl;
    TO_INTEGER(p["x"])->value+= x;
    TO_INTEGER(p["y"])->value+= y;
};



//GLOBAL SCRIPTS
inline void create_blurg(const std::string& name, age::Engine::entites_map_t& ent_map){
    age::Entity::Ptr new_ent(new age::Entity("blurg",10,10,10));
    new_ent->add_property("state",STRING_PTR("red"));
    new_ent->add_property("life",DOUBLE_PTR(10));
    new_ent->add_property("sprite",STRING_PTR("sprite_1"));
    new_ent->set_behavior(rand_behav);

    ent_map.emplace(name,new_ent);
}

inline void init_test(age::Engine::entites_map_t& ent_map,age::AnimatedManager am){
    for(int i = 1; i < 10; i++){
        std::stringstream key_name;
        key_name << "blurg_" << i;
        create_blurg(key_name.str(),ent_map);
        ent_map[key_name.str()]->link_to_sprite(am.get("sprite_1"));
        TO_INTEGER(ent_map[key_name.str()]->access_property()["x"])->value = rand()%400;
        TO_INTEGER(ent_map[key_name.str()]->access_property()["y"])->value = rand()%400;
        TO_DOUBLE(ent_map[key_name.str()]->access_property()["angle"])->value = PI/2.;
    }
}



}
