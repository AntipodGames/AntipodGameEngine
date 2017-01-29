#include "AGE/displaymanager.h"
#include "AGE/types.hpp"


using namespace age;

void DisplayManager::storeProp(Entity::_property_t prop){
    _prop_list.push(prop);
}


void DisplayManager::display(sf::RenderWindow &window,AnimatedManager& am){
    Entity::_property_t current_prop;
    while(!_prop_list.empty()){
        current_prop = _prop_list.front();
        _prop_list.pop();
        _apply(am,current_prop);
        am.get(TO_STRING(current_prop["name"])->value).show(window);
    }
}

void DisplayManager::_apply(AnimatedManager &am, Entity::_property_t prop){
    std::string name = TO_STRING(prop["name"])->value;
    am.get(name).setFrame(TO_INTEGER(prop["frame"])->value);
    am.get(name).setPosition(TO_INTEGER(prop["x"])->value,
                             TO_INTEGER(prop["y"])->value);
    am.get(name).setAngle(TO_DOUBLE(prop["angle"])->value);
    am.get(name).resize(TO_INTEGER(prop["width"])->value,
                        TO_INTEGER(prop["height"])->value);
}
