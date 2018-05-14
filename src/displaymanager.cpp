#include "../include/AGE/displaymanager.h"
#include "../include/AGE/types.hpp"


using namespace age;

void DisplayManager::storeProp(Entity::_property_t prop){
    _prop_list.push(prop);
}

void DisplayManager::displayColliderOverlay(const QuadTree<EmptyCollider>::ConstPtr qt){
    if(qt->get_level() == 0)
        _overlay_list.clear();
    std::shared_ptr<sf::RectangleShape> qt_rect(new sf::RectangleShape(
                                                 sf::Vector2f(qt->get_area()[2],qt->get_area()[3])));
    qt_rect->setPosition(sf::Vector2f(qt->get_area()[0],qt->get_area()[1]));
    qt_rect->setFillColor(sf::Color::Transparent);
    qt_rect->setOutlineColor(sf::Color::Red);
    qt_rect->setOutlineThickness(2.f);
    _overlay_list.push_back(qt_rect);
    if(qt->is_leaf()){
        for(auto& collider: qt->get_elts()){
            if(collider->get_collider_type() == EmptyCollider::BOX){
                std::shared_ptr<sf::RectangleShape> rect(new sf::RectangleShape(
                                                             sf::Vector2f(std::static_pointer_cast<BoxCollider>(collider)->get_width(),
                                                     std::static_pointer_cast<BoxCollider>(collider)->get_height())));
                rect->setPosition(sf::Vector2f(collider->get_center()[0],collider->get_center()[1]));
                rect->setFillColor(sf::Color::Transparent);
                rect->setOutlineColor(sf::Color::Red);
                rect->setOutlineThickness(2.f);
                _overlay_list.push_back(rect);
            }
            else if(collider->get_collider_type() == EmptyCollider::CIRCLE){
                std::shared_ptr<sf::CircleShape> circle(new sf::CircleShape(
                                                            std::static_pointer_cast<CircleCollider>(collider)->get_radius()));
                circle->setPosition(sf::Vector2f(collider->get_center()[0],collider->get_center()[1]));
                circle->setFillColor(sf::Color::Transparent);
                circle->setOutlineColor(sf::Color::Red);
                circle->setOutlineThickness(2.f);
                _overlay_list.push_back(circle);
            }/*
            else if(collider->get_collider_type() == EmptyCollider::ELLIPSE){

            }*/
        }
        return;
    }

    displayColliderOverlay(qt->get_upleft());
    displayColliderOverlay(qt->get_upright());
    displayColliderOverlay(qt->get_downleft());
    displayColliderOverlay(qt->get_downright());
}

void DisplayManager::display(age::QSystem &window,AnimatedManager& am, TextureManager& tm){

    if(_prop_list.empty()){
        std::cerr << "no entity to display !" << std::endl;
    }

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

    for(auto& ol : _overlay_list){
        window.draw(*ol);
    }
}

void DisplayManager::_apply(AnimatedManager &am, Entity::_property_t prop){
    std::string name = TO_STRING(prop["sprite"])->value;
    am.get(name)->setFrame(TO_INTEGER(prop["frame"])->value);
    am.get(name)->setPosition(TO_DOUBLE(prop["x"])->value,
                             TO_DOUBLE(prop["y"])->value);
    am.get(name)->setAngle(TO_DOUBLE(prop["angle"])->value);
    am.get(name)->resize(TO_INTEGER(prop["width"])->value,
                        TO_INTEGER(prop["height"])->value);
    am.get(name)->set_state(TO_STRING(prop["state"])->value);

}
