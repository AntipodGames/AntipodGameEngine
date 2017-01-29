#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <iostream>
#include <memory>
#include <functional>
#include <QObject>
#include <AGE/types.hpp>
#include <unordered_map>

namespace age {



//class Property {
//public:
//    typedef std::shared_ptr<Property> Ptr;
//    typedef const std::shared_ptr<Property> ConstPtr;
//    Property(){}
//    Property(std::string n, int x, int y, int w, int h) :
//        _name(n), _x(x), _y(y), _width(w), _height(h){}

//    Property(const Property& prop) :
//        _name(prop._name), _x(prop._x), _y(prop._y), _width(prop._width), _height(prop._height){}

//    virtual ~Property(){

//    }
////    virtual void set_frame(int i) = 0;

//    std::string _name;
//    int _x = 0;
//    int _y = 0;
//    int _width = 0;
//    int _height = 0;
//    int _frame = 0;
//    int _angle = 0;
//};


//class Behavior {
//public :

//    typedef std::shared_ptr<Behavior> Ptr;
//    typedef const std::shared_ptr<Behavior> ConstPtr;

//    Behavior(){}
//    Behavior(const Behavior& b){}

//    virtual ~Behavior(){

//        std::cout << "Destroy Behavior" << std::endl;
//    }

//    virtual void _run(Property* p) = 0;

//};

//using Entity = std::pair<Behavior*,Property*>;

class Entity /*: public QObject*/
{

//    Q_OBJECT
public:

    typedef std::shared_ptr<Entity> Ptr;
    typedef std::shared_ptr<const Entity> ConstPtr;

    typedef std::unordered_map<std::string,Types::Ptr> _property_t;

    Entity(){
        _behavior = [](_property_t){};
    }

    Entity(std::string n, int x, int y, int w, int h, int v){
        _behavior = [](_property_t){};

        _property.emplace("name",STRING_PTR(n));
        _property.emplace("x",INTEGER_PTR(x));
        _property.emplace("y",INTEGER_PTR(y));
        _property.emplace("velocity",INTEGER_PTR(v));
        _property.emplace("width",INTEGER_PTR(w));
        _property.emplace("height",INTEGER_PTR(h));
        _property.emplace("angle",DOUBLE_PTR(0));
        _property.emplace("frame",INTEGER_PTR(0));
    }


    Entity(const Entity& e)
    {
        _behavior = e._behavior;
        _property = e._property;
    }



    virtual ~Entity(){

    }



//signals:

//public slots:
    void _run(){
        _behavior(_property);
    }


    _property_t access_property(){return _property;}
//    std::function<void(Property*)> access_behavior(){return _behavior;}

    void set_behavior(std::function<void(_property_t)> beh){
        _behavior = beh;
    }

    const std::string & get_name(){return TO_STRING(_property["name"])->value;}





protected :
    std::function<void(_property_t)> _behavior;
    _property_t _property;
//    behavior_t _behavior;
//    property_t _property; //define is characteristics
};
}

#endif // ENTITY_HPP
