#ifndef ENGINE_HPP
#define ENGINE_HPP

#include <iostream>
#include <QObject>
#include <map>
#include <memory>
#include "entity.hpp"
#include "controller.hpp"
#include "displaymanager.h"
#include "collider.hpp"
#include "vector_field.hpp"
#include <boost/random.hpp>


namespace age {

static boost::random::mt19937 gen;

class Engine : public QObject
{
    Q_OBJECT

public:
    typedef std::shared_ptr<Engine> Ptr;
    typedef std::shared_ptr<const Engine> ConstPtr;

    typedef std::map<std::string, std::shared_ptr<Entity>> entites_map_t;
    typedef std::map<std::string, std::shared_ptr<ControllerEntity>> controller_map_t;


    Engine(const DisplayManager::Ptr& dm){
        connect(this,SIGNAL(sendProperty(_property_t)),dm.get(),SLOT(storeProp(_property_t)));
        _cm.reset(new CollisionManager());
        connect(this,SIGNAL(sendQuadTree(const QuadTree<EmptyCollider>::ConstPtr)),
                dm.get(),SLOT(displayColliderOverlay(const QuadTree<EmptyCollider>::ConstPtr)));
        _vf.clear();
    }
    Engine(const DisplayManager::Ptr& dm,const std::string& configFile);

    virtual ~Engine(){
    }

    bool load_config_file(const std::string& configFile);

    virtual void _run();
    entites_map_t& get_entities(){return _entities_map;}
    controller_map_t& get_controllers(){return _controllers_map;}

    virtual void _update() = 0;
    virtual void _init() = 0;

    void add_entity(std::string name, const Entity::Ptr& ent);
    void add_controller(std::string name, const ControllerEntity::Ptr& cont);

public slots:
    void add_vector_field(const std::string& name, const vector_field& vf);

signals:
    void sendProperty(_property_t prop);
    void sendQuadTree(const QuadTree<EmptyCollider>::ConstPtr qt);

//public slots:
//    void receive_vector_field(const vector_field& vf);


protected:

    entites_map_t _entities_map;
    controller_map_t _controllers_map;
    CollisionManager::Ptr _cm;
    std::map<std::string,vector_field> _vf;

    //parameters
    int _width;
    int _height;

    void _update_cm();
};

}

#endif // ENGINE_HPP
