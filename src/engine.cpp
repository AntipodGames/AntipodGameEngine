#include <AGE/engine.hpp>
#include <yaml-cpp/yaml.h>

#include <iostream>
#include <fstream>
#include <string>

using namespace age;

Engine::Engine(const std::string &configFile){
    load_config_file(configFile);
}

bool Engine::load_config_file(const std::string &configFile){
    //TODO add an error management
    YAML::Node file = YAML::LoadFile(configFile);
    _width = file["Window"]["Width"].as<int>();
    _height = file["Window"]["Height"].as<int>();
    return true;
}

void Engine::_run(){
    _update();

    for(auto it =_entities_map.begin(); it != _entities_map.end(); ++it){
        it->second->_run();

        emit sendProperty(it->second->access_property());

    }
}

