#ifndef ANIMATEDSPRITE_H
#define ANIMATEDSPRITE_H
#include <SFML/Graphics.hpp>
#include <AGE/texturemanager.h>
#include <stack>
#include <yaml-cpp/yaml.h>

#define PI 3.14159265359

//TODO CLEANING: REDUCE NUMBER OF FUNCTIONS

namespace age{

class AnimatedSprite
{
private:
    class _animation_graph{
    public:

        _animation_graph(){}
        _animation_graph(const YAML::Node& yaml_node);
        _animation_graph(const _animation_graph& ag) :
            _graph(ag._graph){}

        typedef struct _states_desc_t{

            _states_desc_t(){}
            _states_desc_t(const _states_desc_t& sd) :
                neighbors(sd.neighbors), nbr_frames(sd.nbr_frames),
                time_per_frame(sd.time_per_frame), position(sd.position){}

            std::vector<std::string> neighbors;
            int nbr_frames;
            std::vector<int> time_per_frame;
            int position;
        } _states_desc_t;

        std::map<std::string, _states_desc_t> _graph;


        _states_desc_t& operator[](std::string state){
            return _graph[state];
        }

    };

public:
    AnimatedSprite(){}

    AnimatedSprite(TextureManager &TM,
                   std::string sprite,
                   std::string anim_yml,
                   int width, int height,
                   int centreX, int centreY);
    AnimatedSprite(const AnimatedSprite& as) :
        _image(as._image),
        _current_frame(as._current_frame),
        _current_state(as._current_state),
        _cpt(as._cpt),
        _scale(as._scale),
        _anim_graph(as._anim_graph){}

    void show(sf::RenderWindow &);
    void setFrame(int);
    int getFrame();
    void setPosition(int,int);
    sf::Sprite &get_image();
    void setAngle(float);
    float getAngle();
    void Rotate(float angle);
    int getScale();
    void resize(int x, int y);

    void set_state(const std::string &state);

    static sf::Vector2f TransformToLocal(const sf::Vector2f &);

    const _animation_graph::_states_desc_t& get_current_state(){
        return _anim_graph[_current_state];
    }

private:
    sf::Sprite _image;

    int _current_frame = 0;
    std::string _current_state;
    int _cpt;
    int _scale;

    _animation_graph _anim_graph;

};
}//age

#endif // ANIMATEDSPRITE_H
