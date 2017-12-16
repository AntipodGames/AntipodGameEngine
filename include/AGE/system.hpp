#ifndef SYSTEM_HPP
#define SYSTEM_HPP

#include <SFML/Graphics.hpp>
#include <QWidget>
#include <QTimer>
#include <memory>
#include <AGE/parameters.hpp>
#include <AGE/qsystem.hpp>
#include <AGE/displaymanager.h>
#include <AGE/engine.hpp>
#include <yaml-cpp/yaml.h>

#ifdef Q_WS_X11
    #include <Qt/qx11info_x11.h>
    #include <X11/Xlib.h>
#endif //Q_WS_X11


namespace age {


class System : public age::QSystem
{
    Q_OBJECT
public:

    typedef std::shared_ptr<System> Ptr;
    typedef std::shared_ptr<const System> ConstPtr;


    System(QWidget *parent,const QPoint& Position, const QSize& Size,const std::string& graphics_file = "graphics/graphics.yml")
        : QSystem(parent,Position,Size){
        _dm.reset(new DisplayManager);
        _graphics = graphics_file;

    }

    System(QWidget *parent, const std::string& config_file) : QSystem(parent){
        _dm.reset(new DisplayManager);
        YAML::Node file = YAML::LoadFile(config_file);
        if(file.IsNull()){
            std::cerr << "Unable to open configuration file : " << config_file << std::endl;
            return;
        }

        _work_dir = file["work_dir"].as<std::string>();

        _size[0] = file["window"]["width"].as<int>();
        _size[1] = file["window"]["height"].as<int>();

        _position[0] = file["window"]["pos_x"].as<int>();
        _position[1] = file["window"]["pos_y"].as<int>();

        _graphics = file["graphics"].as<std::string>();
        _initialize();
    }

    virtual ~System(){
        _dm.reset();
        _engine.reset();
    }

protected:
//    virtual void _init(){}
//    virtual void _update(){}
    virtual void paintEvent(QPaintEvent*){
        _engine->_run(); //we update the engine : state of all entity and physical state of the environment
        _update(); //we update the graphic part
        clear();
        setView(_view);
        _dm->display(*this,_am,_tm);
        display();
    }

    void _load_graphics(const std::string& file, const std::string& name);
    void _load_all_graphics(const std::string& file);
    void _link_sprites_to_entities();
    void _load_level(const std::string& file);
    void _load_env_config(const std::string& file);

    DisplayManager::Ptr _dm;
    Engine::Ptr _engine;
    AnimatedManager _am;
    TextureManager _tm;
    Scene _scene;

};

}
#endif // SYSTEM_HPP
