#ifndef DISPLAYMANAGER_H
#define DISPLAYMANAGER_H

#include <QObject>
#include <AGE/entity.hpp>
#include <AGE/animatedmanager.h>
#include <AGE/qsystem.hpp>
#include <SFML/Graphics.hpp>
#include <queue>

namespace age{


typedef struct scene{
    std::vector<std::string> layers;
    std::vector<std::string> collision_maps;
} Scene;

class DisplayManager : public QObject
{
    Q_OBJECT
public:
    typedef std::shared_ptr<DisplayManager> Ptr;
    typedef std::shared_ptr<const DisplayManager> ConstPtr;

    DisplayManager() : QObject(){}

    void display(age::QSystem& window,AnimatedManager& am, TextureManager &tm);

    void set_scene(const Scene& scene){
        for(const auto & l : scene.layers)
            _scene.layers.push_back(l);
        for(const auto & c : scene.collision_maps)
            _scene.collision_maps.push_back(c);
    }

public slots:
    void storeProp(Entity::_property_t prop);

private:
    std::queue<Entity::_property_t> _prop_list;

    void _apply(AnimatedManager& am, Entity::_property_t prop);

    Scene _scene;
};
}//age

#endif // DISPLAYMANAGER_H
