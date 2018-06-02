#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <iostream>
#include <memory>
#include <functional>
#include <QObject>
#include "types.hpp"
#include "animatedsprite.h"
#include <map>
#include "collider.hpp"
#include "vector_field.hpp"

namespace age {

class Entity : public QObject
{

    Q_OBJECT

public:

    typedef std::shared_ptr<Entity> Ptr;
    typedef std::shared_ptr<const Entity> ConstPtr;


    Entity(): QObject(){
        _collider.reset(new EmptyCollider);
        _behavior = [](_property_t&){};
        _custom_run = [](Entity*){};
        _on_collision_behav = [](const _property_t&,_property_t&){};
        _in_collision_behav = [](const _property_t&,_property_t&){};

        _property.emplace("name",STRING_PTR(""));
        _property.emplace("x",DOUBLE_PTR(-1));
        _property.emplace("y",DOUBLE_PTR(-1));
        _property.emplace("velocity",DOUBLE_PTR(0));
        _property.emplace("width",INTEGER_PTR(0));
        _property.emplace("height",INTEGER_PTR(0));
        _property.emplace("angle",DOUBLE_PTR(0));
        _property.emplace("state",STRING_PTR("idle"));
        _property.emplace("sprite",STRING_PTR(""));
        _property.emplace("frame",INTEGER_PTR(0));
        _property.emplace("clock",INTEGER_PTR(0));
        _property.emplace("layer",INTEGER_PTR(0));
    }

    Entity(const std::string& name) : QObject(){
        _behavior = [](_property_t&){};
        _on_collision_behav = [](const _property_t&,_property_t&){};
        _in_collision_behav = [](const _property_t&,_property_t&){};
        _custom_run = [](Entity*){};
        _collider.reset(new EmptyCollider);

        _property.emplace("name",STRING_PTR(name));
        _property.emplace("x",DOUBLE_PTR(-1));
        _property.emplace("y",DOUBLE_PTR(-1));
        _property.emplace("velocity",DOUBLE_PTR(0));
        _property.emplace("width",INTEGER_PTR(0));
        _property.emplace("height",INTEGER_PTR(0));
        _property.emplace("angle",DOUBLE_PTR(0));
        _property.emplace("state",STRING_PTR("idle"));
        _property.emplace("sprite",STRING_PTR(""));
        _property.emplace("frame",INTEGER_PTR(0));
        _property.emplace("clock",INTEGER_PTR(0));
        _property.emplace("layer",INTEGER_PTR(0));
    }

    Entity(const std::string& n, int w, int h, int v) : QObject(){
        _behavior = [](_property_t&){};
        _on_collision_behav = [](const _property_t&,_property_t&){};
        _in_collision_behav = [](const _property_t&,_property_t&){};
        _custom_run = [](Entity*){};

        _collider.reset(new EmptyCollider);
        _property.emplace("name",STRING_PTR(n));
        _property.emplace("x",DOUBLE_PTR(-1));
        _property.emplace("y",DOUBLE_PTR(-1));
        _property.emplace("velocity",DOUBLE_PTR(v));
        _property.emplace("width",INTEGER_PTR(w));
        _property.emplace("height",INTEGER_PTR(h));
        _property.emplace("angle",DOUBLE_PTR(0));
        _property.emplace("state",STRING_PTR("idle"));
        _property.emplace("sprite",STRING_PTR(""));
        _property.emplace("frame",INTEGER_PTR(0));
        _property.emplace("clock",INTEGER_PTR(0));
        _property.emplace("layer",INTEGER_PTR(0));
    }


    Entity(const Entity& e) : QObject()
    {
        _behavior = e._behavior;
        _property = e._property;
        _sprite_link = e._sprite_link;
        _collider = e._collider;
        _on_collision_behav = e._on_collision_behav;
        _in_collision_behav = e._in_collision_behav;
        _custom_run = e._custom_run;
    }



    virtual ~Entity(){

    }

    void _run(){
        if(!_in_collision || !_no_behavior_in_col)
            _behavior(_property);
        _custom_run(this);
       TO_INTEGER(_property["clock"])->value=(TO_INTEGER(_property["clock"])->value+1)%60; //rethink this
       if(TO_INTEGER(_property["clock"])->value%
               (_sprite_link->get_anim_graph()[TO_STRING(_property["state"])->value]
                .time_per_frame[TO_INTEGER(_property["frame"])->value]) == 0)
           TO_INTEGER(_property["frame"])->value = (TO_INTEGER(_property["frame"])->value + 1)%
                   _sprite_link->get_anim_graph()[TO_STRING(_property["state"])->value].nbr_frames;
       float x = _collider->get_center()[0];
       float y = _collider->get_center()[1];
       _collider->move(TO_DOUBLE(_property["x"])->value,TO_DOUBLE(_property["y"])->value);
       _collider->set_prop(_property);
       emit update_qt(_collider,x,y);
       _in_collision = false;
    }



    _property_t& access_property(){return _property;}

    void set_behavior(std::function<void(_property_t&)> beh){
        _behavior = beh;
    }

    void set_custom_run(std::function<void(Entity*)> cr){
        _custom_run = cr;
    }

    const std::string & get_name(){return TO_STRING(_property["name"])->value;}

    /**
     * @brief add a new property if it does not exist,
     * change the value of existing named property otherwise.
     * @param name of the property
     * @param value of the property
     * @return true if a new property is added, false otherwise
     */
    bool add_property(const std::string& name,const Types::Ptr& value){
        if(_property.find(name) == _property.end()){
            _property.emplace(name,value);
            return true;
        }
        _property[name] = value;
        return false;
    }

    /**
     * @brief set a property to a certain value.
     * @param name of the property
     * @param value of the property
     * @return false if the property of name does not exist, true otherwise
     */
    bool set_property(const std::string& name, const Types::Ptr& value){
        if(_property.find(name) == _property.end())
            return false;

        _property[name] = value;
        return true;
    }

    void set_on_collision(std::function<void(const _property_t&,_property_t&)> cb){
        _on_collision_behav = cb;
    }
    void set_in_collision(std::function<void(const _property_t&,_property_t&)> cb){
        _in_collision_behav = cb;
    }

    void set_no_behavior_in_collision(bool b){
        _no_behavior_in_col = b;
    }

    void link_to_sprite(const AnimatedSprite::Ptr& as){
        _sprite_link = as;
    }

    const AnimatedSprite::Ptr& get_sprite(){return _sprite_link;}
    void set_collider(EmptyCollider* collider){
        _collider.reset(collider);
        _collider->set_prop(_property);
        _collider->move(TO_DOUBLE(_property["x"])->value - TO_INTEGER(_property["width"])->value,TO_DOUBLE(_property["y"])->value - TO_INTEGER(_property["height"])->value);
        connect(_collider.get(),SIGNAL(collision(const _property_t&)),this,SLOT(in_collision(const _property_t&)));
        connect(_collider.get(),SIGNAL(no_collision()),this,SLOT(collision_free()));
    }
    EmptyCollider::Ptr get_collider(){return _collider;}

signals:
    void update_qt(EmptyCollider::Ptr clld, float x, float y);
    void send_vector_field(const std::string& name,const vector_field& vf);
    void send_entity(const Entity& e);

protected slots:
    void collision_free(){
        _collision_free = true;
    }

    void in_collision(const _property_t& collider_prop){
        _in_collision_behav(collider_prop,_property);
        if(!_in_collision && _collision_free)
            _on_collision_behav(collider_prop,_property);
        _in_collision = true;
        _collision_free = false;
    }

protected :
    std::function<void(Entity*)> _custom_run;
    std::function<void(_property_t&)> _behavior;
    std::function<void(const _property_t&,_property_t&)> _in_collision_behav;
    std::function<void(const _property_t&,_property_t&)> _on_collision_behav;
    _property_t _property;
    AnimatedSprite::Ptr _sprite_link;
    EmptyCollider::Ptr _collider;
    bool _in_collision = false;
    bool _collision_free = true;
    bool _no_behavior_in_col = false;

};
}

#endif // ENTITY_HPP
