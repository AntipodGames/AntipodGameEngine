#include <AGE/engine.hpp>
#include <AGE/types.hpp>
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

bool Engine::load_level(int i){
    YAML::Node yml_file = YAML::LoadFile("levels/level_"+std::to_string(i)+".yml");
    //LOAD BACKGROUND

    //Load entities
    std::cout << yml_file["npc"] << std::endl;
    for(const auto& e : yml_file["npc"]){
        Entity::Ptr entity(new Entity);
        for(const auto& prop: e.second["property"]){
            if(prop.second["type"].as<std::string>() == "int")
                entity->add_property(prop.first.as<std::string>(),
                                     INTEGER_PTR(prop.second["value"].as<int>()));
            else if(prop.second["type"].as<std::string>() == "float")
                entity->add_property(prop.first.as<std::string>(),
                                     FLOAT_PTR(prop.second["value"].as<float>()));
            else if(prop.second["type"].as<std::string>() == "double")
                entity->add_property(prop.first.as<std::string>(),
                                     DOUBLE_PTR(prop.second["value"].as<double>()));
            else if(prop.second["type"].as<std::string>() == "bool")
                entity->add_property(prop.first.as<std::string>(),
                                     BOOL_PTR(prop.second["value"].as<bool>()));
            else if(prop.second["type"].as<std::string>() == "string")
                entity->add_property(prop.first.as<std::string>(),
                                     STRING_PTR(prop.second["value"].as<std::string>()));
        }
        _entities_map.emplace(e.first.as<std::string>(),entity);
    }


}

void Engine::_run(){
    _update();

    for(auto it =_entities_map.begin(); it != _entities_map.end(); ++it){
        it->second->_run();

        emit sendProperty(it->second->access_property());

    }
}

