#include <AGE/engine.hpp>
#include <AGE/types.hpp>
#include <yaml-cpp/yaml.h>

#include <iostream>
#include <fstream>
#include <string>

using namespace age;

Engine::Engine(const DisplayManager::Ptr& dm,const std::string &configFile){
    connect(this,SIGNAL(sendProperty(Entity::_property_t)),dm.get(),SLOT(storeProp(Entity::_property_t)));
    load_config_file(configFile);
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

    for(auto it =_entities_map.begin(); it != _entities_map.end(); ++it){
        it->second->_run();
        emit sendProperty(it->second->access_property());
    }
}

