#include <AGE/system.hpp>

using namespace age;

void System::_link_sprites_to_entities(){
    for(auto& entity : _engine->get_entities()){
        entity.second->link_to_sprite(_am.get(TO_STRING(entity.second->access_property()["sprite"])->value));
    }
}


void System::_load_all_graphics(const std::string& file){
    YAML::Node yaml_file = YAML::LoadFile(_work_dir + file);

    for(const auto& spr : yaml_file){
        _am.add(spr.first.as<std::string>(),
                AnimatedSprite(_tm,
                               _work_dir + spr.second["path"].as<std::string>(),
                _work_dir + spr.second["animation_graph"].as<std::string>(),
                spr.second["width"].as<int>(),
                spr.second["height"].as<int>(),
                spr.second["center_pos"]["x"].as<int>(),
                spr.second["center_pos"]["y"].as<int>()));
    }
}


void System::_load_graphics(const std::string& file, const std::string& name){
    YAML::Node yaml_file = YAML::LoadFile(_work_dir + file);

    YAML::Node spriteNode = yaml_file[name];

    _am.add(name,
            AnimatedSprite(_tm,
                           _work_dir + spriteNode["path"].as<std::string>(),
            _work_dir + spriteNode["animation_graph"].as<std::string>(),
            spriteNode["width"].as<int>(),
            spriteNode["height"].as<int>(),
            spriteNode["center_pos"]["x"].as<int>(),
            spriteNode["center_pos"]["y"].as<int>()));
}


void System::_load_level(const std::string &file){
    YAML::Node yml_file = YAML::LoadFile(_work_dir + file);


    //LOAD SCENE
    Scene scne;
    for(size_t i = 0; i < yml_file["scene"]["layers"].size(); ++i){
        std::cout << yml_file["scene"]["layers"][i] << std::endl;
        scne.layers.push_back(_work_dir + yml_file["scene"]["layers"][i].as<std::string>());
    }
    for(size_t i = 0; i < yml_file["scene"]["collision_maps"].size(); ++i){
        scne.collision_maps.push_back(_work_dir + yml_file["scene"]["collision_maps"][i].as<std::string>());

    }

    _dm->set_scene(scne);

// TODO : handle the case without player
//    if(!yml_file["player"].IsNull()){
//        _load_graphics(_graphics,"player_sprite"); //NOTE: To be changed. Let the possibility to change the name of the graphics for the player
//    }

    //LOAD NPC
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
        entity->add_property("sprite",STRING_PTR(e.second["graphic"].as<std::string>()));
        _engine->get_entities().emplace(e.first.as<std::string>(),entity);
        _load_graphics(_graphics,e.second["graphic"].as<std::string>());
    }
    _link_sprites_to_entities();
}
