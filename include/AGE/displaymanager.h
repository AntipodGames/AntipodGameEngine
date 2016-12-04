#ifndef DISPLAYMANAGER_H
#define DISPLAYMANAGER_H

#include <QObject>
#include <AGE/entity.hpp>
#include <AGE/animatedmanager.h>
#include <SFML/Graphics.hpp>
#include <queue>

namespace age{

class DisplayManager : public QObject
{
    Q_OBJECT
public:
    typedef std::shared_ptr<DisplayManager> Ptr;
    typedef std::shared_ptr<const DisplayManager> ConstPtr;

    DisplayManager() : QObject(){}

    void display(sf::RenderWindow& window,AnimatedManager& am);

public slots:
    void storeProp(Entity::_property_t prop);

private:
    std::queue<Entity::_property_t> _prop_list;

    void _apply(AnimatedManager& am, Entity::_property_t prop);
};
}//age

#endif // DISPLAYMANAGER_H
