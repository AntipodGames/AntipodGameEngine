#ifndef CORE_H
#define CORE_H

#include <QWidget>
#include "AGE/system.hpp"
#include "AGE/engine.hpp"


class Environment : public age::Engine
{

    Q_OBJECT
public :
    Environment(const age::DisplayManager::Ptr& dm) : age::Engine(dm){}
    Environment(const age::DisplayManager::Ptr& dm,const std::string& configFile) : age::Engine(dm,configFile){}

    ~Environment(){}

    void _init(){}
    void _update(){}
};

class Core : public age::System
{

    Q_OBJECT
public:
    Core(const QPoint& position, const QSize& size,QWidget *parent = 0)
        : age::System(parent,position,size){
        _engine.reset(new Environment(_dm));
        _init();
    }

    Core(const std::string& config_file, QWidget* parent = 0) :
        age::System(parent,config_file)
    {
        _engine.reset(new Environment(_dm));
        _init();
    }

    ~Core(){}

    void _init();
    void _update();
};





#endif // CORE_H
