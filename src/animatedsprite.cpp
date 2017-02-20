#include "AGE/animatedsprite.h"

using namespace age;

//TODO Clean this class. In particular the constructor. One must remain ! (with default and copy of course)

AnimatedSprite::AnimatedSprite(TextureManager &TM, std::string sprite,
                               std::string anim_yml, int width, int height,
                               int centreX, int centreY){
    _image.setTexture(*(TM.GetTexture(sprite)));
    std::cout << anim_yml << std::endl;
    //TODO create animation graph
    YAML::Node yaml_file = YAML::LoadFile(anim_yml);
    YAML::Node anim_graph = yaml_file["animation_graph"];
    _anim_graph = _animation_graph(anim_graph);
    for(const auto& node : _anim_graph._graph)
        if(node.second.position == 0)
            _current_state = node.first;


    float scale1 = ((float)width*get_current_state().nbr_frames)/
            (float)_image.getTexture()->getSize().x;
    float scale2 = (float)height/(float)_image.getTexture()->getSize().y;

    _image.setScale(scale1,scale2);

    _image.setOrigin(_image.getTexture()->getSize().x/(centreX*get_current_state().nbr_frames),
                    _image.getTexture()->getSize().y/(centreY*_anim_graph._graph.size()));
    _image.setTextureRect(sf::IntRect(0,0,_image.getTexture()->getSize().x/get_current_state().nbr_frames,
                                     _image.getTexture()->getSize().y/_anim_graph._graph.size()));
}




void AnimatedSprite::show(sf::RenderWindow &App){
    App.draw(_image);
}

sf::Vector2f AnimatedSprite::TransformToLocal(const sf::Vector2f &Pt){
    return TransformToLocal(Pt);
}

void AnimatedSprite::setFrame(int i){

    if(i%_anim_graph[_current_state].time_per_frame[_current_frame] == 0)
        _current_frame = (_current_frame+1)%get_current_state().nbr_frames;

    _image.setTextureRect(sf::IntRect((_image.getTexture()->getSize().x/get_current_state().nbr_frames)
                                     *(_current_frame)
                                     ,0
                                     ,(_image.getTexture()->getSize().x/get_current_state().nbr_frames)
                                     ,_image.getTexture()->getSize().y/_anim_graph._graph.size()));

}

int AnimatedSprite::getFrame(){
    return _current_frame;
}

void AnimatedSprite::Rotate(float angle){
    _image.rotate(angle/PI*180);

}

void AnimatedSprite::setAngle(float angle){
    _image.setRotation(angle/PI*180);
}

float AnimatedSprite::getAngle(){
    return _image.getRotation()/180*PI;
}

void AnimatedSprite::setPosition(int x, int y){
    _image.setPosition(x,y);
}

sf::Sprite &AnimatedSprite::get_image(){
    return _image;
}

int AnimatedSprite::getScale(){
    return _scale;
}

void AnimatedSprite::resize(int x, int y){
    _image.setScale((float)(x*get_current_state().nbr_frames)/((float)_image.getTexture()->getSize().x)
                   ,((float)y*_anim_graph._graph.size())/((float)_image.getTexture()->getSize().y));
}

void AnimatedSprite::set_state(const std::string &state){
    if(_anim_graph._graph.find(state) == _anim_graph._graph.end())
        return;
    _current_state = state;
    _image.setTextureRect(sf::IntRect((_image.getTexture()->getSize().x/get_current_state().nbr_frames)
                                     *(_current_frame-1)
                                     ,(_image.getTexture()->getSize().y/_anim_graph._graph.size())
                                     *(_anim_graph[state].position)
                                     ,(_image.getTexture()->getSize().x/get_current_state().nbr_frames)
                                     ,_image.getTexture()->getSize().y/_anim_graph._graph.size()));

}

AnimatedSprite::_animation_graph::_animation_graph(const YAML::Node &yaml_node){

    for(auto it = yaml_node.begin(); it != yaml_node.end(); ++it){
        _states_desc_t description;
        description.nbr_frames = it->second["nbr_frames"].as<int>();
        description.neighbors = std::vector<std::string>(it->second["neighbors"].size());
        for(size_t i  = 0; i < it->second["neighbors"].size(); i++)
            description.neighbors[i] = it->second["neighbors"][i].as<std::string>();
        description.position = it->second["position"].as<int>();
        description.time_per_frame = std::vector<int>(it->second["time_per_frame"].size());
        for(size_t i = 0; i < it->second["time_per_frame"].size(); i++)
            description.time_per_frame[i] = it->second["time_per_frame"][i].as<float>();
        _graph.emplace(it->first.as<std::string>(),description);
    }
}
