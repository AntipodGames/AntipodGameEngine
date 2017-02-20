#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <iostream>
#include <memory>
#include <functional>
#include <QObject>
#include <AGE/types.hpp>
#include <unordered_map>

namespace age {

class Entity
{

public:

    typedef std::shared_ptr<Entity> Ptr;
    typedef std::shared_ptr<const Entity> ConstPtr;

    typedef std::unordered_map<std::string,Types::Ptr> _property_t;

    Entity(){
        _behavior = [](_property_t&){};
        _property.emplace("name",STRING_PTR(""));
        _property.emplace("x",INTEGER_PTR(-1));
        _property.emplace("y",INTEGER_PTR(-1));
        _property.emplace("velocity",INTEGER_PTR(0));
        _property.emplace("width",INTEGER_PTR(0));
        _property.emplace("height",INTEGER_PTR(0));
        _property.emplace("angle",DOUBLE_PTR(0));
        _property.emplace("state",STRING_PTR("idle"));
        _property.emplace("frame",INTEGER_PTR(0));
        _property.emplace("clock",INTEGER_PTR(0));
    }

    Entity(const std::string& name){
        _behavior = [](_property_t&){};

        _property.emplace("name",STRING_PTR(name));
        _property.emplace("x",INTEGER_PTR(-1));
        _property.emplace("y",INTEGER_PTR(-1));
        _property.emplace("velocity",INTEGER_PTR(0));
        _property.emplace("width",INTEGER_PTR(0));
        _property.emplace("height",INTEGER_PTR(0));
        _property.emplace("angle",DOUBLE_PTR(0));
        _property.emplace("state",STRING_PTR("idle"));
        _property.emplace("frame",INTEGER_PTR(0));
        _property.emplace("clock",INTEGER_PTR(0));

    }

    Entity(const std::string& n, int w, int h, int v){
        _behavior = [](_property_t&){};

        _property.emplace("name",STRING_PTR(n));
        _property.emplace("x",INTEGER_PTR(-1));
        _property.emplace("y",INTEGER_PTR(-1));
        _property.emplace("velocity",INTEGER_PTR(v));
        _property.emplace("width",INTEGER_PTR(w));
        _property.emplace("height",INTEGER_PTR(h));
        _property.emplace("angle",DOUBLE_PTR(0));
        _property.emplace("state",STRING_PTR("idle"));
        _property.emplace("frame",INTEGER_PTR(0));
        _property.emplace("clock",INTEGER_PTR(0));
    }


    Entity(const Entity& e)
    {
        _behavior = e._behavior;
        _property = e._property;
    }



    virtual ~Entity(){

    }

    void _run(){
       _behavior(_property);
       TO_INTEGER(_property["clock"])->value=TO_INTEGER(_property["clock"])->value%120+1;
    }


    _property_t access_property(){return _property;}
//    std::function<void(Property*)> access_behavior(){return _behavior;}

    void set_behavior(std::function<void(_property_t&)> beh){
        _behavior = beh;
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




protected :
    std::function<void(_property_t&)> _behavior;
    _property_t _property;

};
}

#endif // ENTITY_HPP
