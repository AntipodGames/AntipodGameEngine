#include "AGE/displaymanager.h"
#include "AGE/types.hpp"


using namespace age;

void DisplayManager::storeProp(Entity::_property_t prop){
    _prop_list.push(prop);
}


void DisplayManager::display(age::QSystem &window,AnimatedManager& am, TextureManager& tm){


    Entity::_property_t current_prop;

    if(_scene.layers.empty()){
        while(!_prop_list.empty()){
            current_prop = _prop_list.front();
            _prop_list.pop();
            _apply(am,current_prop);
            am.get(TO_STRING(current_prop["sprite"])->value)->show(window);
        }
    }

    std::queue<Entity::_property_t> tmp_list;

    for(size_t i = 0; i < _scene.layers.size(); i++){

        sf::Sprite tmp(*tm.GetTexture(_scene.layers[i]));
        tmp.setScale(window.get_size()[0]/(float)tm.GetTexture(_scene.layers[i])->getSize().x,window.get_size()[1]/(float)tm.GetTexture(_scene.layers[i])->getSize().y);
        window.draw(tmp);

        while(!_prop_list.empty()){
            current_prop = _prop_list.front();
            if(TO_INTEGER(current_prop["layer"])->value == i){
                _prop_list.pop();
                _apply(am,current_prop);
                am.get(TO_STRING(current_prop["sprite"])->value)->show(window);
            }else{
                tmp_list.push(current_prop);
                _prop_list.pop();
            }
        }
        _prop_list = tmp_list;
        tmp_list = std::queue<Entity::_property_t>();
    }
}

void DisplayManager::_apply(AnimatedManager &am, Entity::_property_t prop){
    std::string name = TO_STRING(prop["sprite"])->value;
    am.get(name)->setFrame(TO_INTEGER(prop["frame"])->value);
    am.get(name)->setPosition(TO_INTEGER(prop["x"])->value,
                             TO_INTEGER(prop["y"])->value);
    am.get(name)->setAngle(TO_DOUBLE(prop["angle"])->value);
    am.get(name)->resize(TO_INTEGER(prop["width"])->value,
                        TO_INTEGER(prop["height"])->value);
    am.get(name)->set_state(TO_STRING(prop["state"])->value);

}
