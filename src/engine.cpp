#include "../include/AGE/engine.hpp"
#include "../include/AGE/types.hpp"
#include <yaml-cpp/yaml.h>

#include <iostream>
#include <fstream>
#include <string>

using namespace age;

boost::random::mt19937 gen;

Engine::Engine(const DisplayManager::Ptr& dm,const std::string &configFile){
    connect(this,SIGNAL(sendProperty(Entity::_property_t)),dm.get(),SLOT(storeProp(Entity::_property_t)));
    connect(this,SIGNAL(sendQuadTree(const QuadTree<EmptyCollider>::ConstPtr)),
            dm.get(),SLOT(displayColliderOverlay(const QuadTree<EmptyCollider>::ConstPtr)));
    load_config_file(configFile);
    _cm.reset(new CollisionManager());
    _vf.clear();

}

bool Engine::load_config_file(const std::string &configFile){
    //TODO add an error management
    YAML::Node file = YAML::LoadFile(configFile);
    _width = file["window"]["width"].as<int>();
    _height = file["window"]["height"].as<int>();

    return true;
}

void Engine::_run(){
    _update();

    //check collisions
    _cm->check_collision();
    emit sendQuadTree(_cm->get_quadtree());

    //apply vector fields
    for(auto& field: _vf){
        field.second.update();
        for(auto& entity: _entities_map)
            field.second.apply(entity.second->access_property());
        for(auto& cont: _controllers_map)
            field.second.apply(cont.second->access_property());
    }

    //apply entities scripts and send properties to graphical system
    for(auto it =_entities_map.begin(); it != _entities_map.end(); ++it){
        it->second->_run();
        emit sendProperty(it->second->access_property());
    }

    //apply controllers scripts and send properties to graphical system
    for(auto it =_controllers_map.begin(); it != _controllers_map.end(); ++it){
        it->second->_run();
        emit sendProperty(it->second->access_property());
    }
}

void Engine::add_entity(std::string name, const Entity::Ptr& ent){
    _entities_map.emplace(name,ent);
    _cm->insert(ent->get_collider());
    connect(ent.get(),SIGNAL(update_qt(EmptyCollider::Ptr,float,float)),_cm.get(),SLOT(move_collider(EmptyCollider::Ptr,float,float)));
    connect(ent.get(),SIGNAL(send_vector_field(const std::string&, const vector_field&)),this,SLOT(add_vector_field(const std::string&, const vector_field&)));
}

void Engine::add_controller(std::string name, const ControllerEntity::Ptr &cont){
    _controllers_map.emplace(name,cont);
    _cm->insert(cont->get_collider());
    connect(cont.get(),SIGNAL(update_qt(EmptyCollider::Ptr,float,float)),_cm.get(),SLOT(move_collider(EmptyCollider::Ptr,float,float)));
    connect(cont.get(),SIGNAL(send_vector_field(const std::string&, const vector_field&)),this,SLOT(add_vector_field(const std::string&, const vector_field&)));
}

void Engine::add_vector_field(const std::string& name, const vector_field &vf){
    _vf[name] = vf;
}
