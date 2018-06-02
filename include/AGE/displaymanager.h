#ifndef DISPLAYMANAGER_H
#define DISPLAYMANAGER_H

#include <QObject>
#include "entity.hpp"
#include "animatedmanager.h"
#include "qsystem.hpp"
#include <SFML/Graphics.hpp>
#include <vector>
#include <queue>

#include "collider.hpp"

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
    void storeProp(_property_t prop);
    void displayColliderOverlay(const QuadTree<EmptyCollider>::ConstPtr qt);

private:
    std::queue<_property_t> _prop_list;
    std::vector<std::shared_ptr<sf::Drawable>> _overlay_list;

    void _apply(AnimatedManager& am, _property_t prop);

    Scene _scene;
};
}//age

#endif // DISPLAYMANAGER_H
