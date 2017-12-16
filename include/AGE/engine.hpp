#ifndef ENGINE_HPP
#define ENGINE_HPP

#include <iostream>
#include <QObject>
#include <unordered_map>
#include <memory>
#include <AGE/entity.hpp>
#include <AGE/displaymanager.h>

namespace age {

class Engine : public QObject
{
    Q_OBJECT

public:
    typedef std::shared_ptr<Engine> Ptr;
    typedef std::shared_ptr<const Engine> ConstPtr;

    typedef std::unordered_map<std::string, std::shared_ptr<Entity>> entites_map_t;

    Engine(const DisplayManager::Ptr& dm){
        connect(this,SIGNAL(sendProperty(Entity::_property_t)),dm.get(),SLOT(storeProp(Entity::_property_t)));
    }
    Engine(const DisplayManager::Ptr& dm,const std::string& configFile);

    virtual ~Engine(){
    }

    bool load_config_file(const std::string& configFile);

    virtual void _run();
    entites_map_t& get_entities(){return _entities_map;}
    virtual void _update() = 0;
    virtual void _init() = 0;
signals:
    void sendProperty(Entity::_property_t prop);

protected:


    entites_map_t _entities_map;

    //parameters
    int _width;
    int _height;
};

}

#endif // ENGINE_HPP
