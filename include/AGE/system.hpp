#ifndef SYSTEM_HPP
#define SYSTEM_HPP

#include <SFML/Graphics.hpp>
#include <QWidget>
#include <QTimer>
#include <memory>
#include <AGE/parameters.hpp>
#include <AGE/controller.hpp>
#include <AGE/displaymanager.h>
#include <yaml-cpp/yaml.h>
#include <AGE/engine.hpp>

#ifdef Q_WS_X11
    #include <Qt/qx11info_x11.h>
    #include <X11/Xlib.h>
#endif //Q_WS_X11


namespace age {

class QSystem : public QWidget, public sf::RenderWindow
{
    Q_OBJECT
public:
    QSystem(QWidget *parent,const QPoint& Position, const QSize& Size) :
        QWidget(parent), _is_init(false)
    {
        setAttribute(Qt::WA_PaintOnScreen);
        setAttribute(Qt::WA_OpaquePaintEvent);
        setAttribute(Qt::WA_NoSystemBackground);

        setFocusPolicy(Qt::StrongFocus);

        move(Position);
        setMaximumSize(Size);
        setMinimumSize(Size);

        std::cout << parent->windowTitle().toStdString() << std::endl;
        _timer.reset(new QTimer);
        _timer->setInterval(parameters::frame_rate);

        _dm.reset(new DisplayManager);


    }

    virtual ~QSystem(){
        _dm.reset();
        _engine.reset();
    }

//public slots:
//    void paint(){
//        repaint(_position.x(),_position.y(),_size.width(),_size.height());
//    }



protected:
    virtual void _init() = 0;
    virtual void _update() = 0;

    void _init_view(int x, int y, int width, int height){
        sf::Vector2f viewCenter(x,y);
        sf::Vector2f viewSize(width,height);
        _view = sf::View(viewCenter,viewSize);
    }

    QPaintEngine* paintEngine() const {return 0;}
    virtual void showEvent(QShowEvent*){
        if (!_is_init)
        {
            // Sous X11, il faut valider les commandes qui ont été envoyées au serveur
            // afin de s'assurer que SFML aura une vision à jour de la fenêtre
            #ifdef Q_WS_X11
               XFlush(QX11Info::display());
            #endif

            // On crée la fenêtre SFML avec l'identificateur du widget
            RenderWindow::create(winId());

            // On laisse la classe dérivée s'initialiser si besoin
            _init();

            // On paramètre le timer de sorte qu'il génère un rafraîchissement à la fréquence souhaitée
            connect(_timer.get(), SIGNAL(timeout()), this, SLOT(repaint()));
            _timer->start();

            connect(_engine.get(),SIGNAL(sendProperty(Entity::_property_t)),_dm.get(),SLOT(storeProp(Entity::_property_t)));


            _is_init = true;
        }
    }


    sf::View _view;
    std::unique_ptr<QTimer> _timer;
    bool _is_init;

    Engine::Ptr _engine;
    DisplayManager::Ptr _dm;
    AnimatedManager _am;
    TextureManager _tm;
//    QPoint _position;
//    QSize _size;
};

class System : public QSystem
{
    Q_OBJECT
public:

    typedef std::shared_ptr<System> Ptr;
    typedef std::shared_ptr<const System> ConstPtr;


    System(QWidget *parent,const QPoint& Position, const QSize& Size)
        : QSystem(parent,Position,Size){
    }

    virtual ~System(){
    }

protected:
    virtual void _init(){}
    virtual void _update(){}
    virtual void paintEvent(QPaintEvent*){
        _engine->_run(); //we update the engine : state of all entity and physical state of the environment
        _update(); //we update the graphic part
        clear();
        setView(_view);
        _dm->display(*this,_am);
        display();
    }

    void _load_graphics(std::string file){
        std::cout << "load " << file << std::endl;
        YAML::Node yaml_file = YAML::LoadFile(file);

        for(int i = 0; i < yaml_file.size(); i++ ){
            std::stringstream stream;
            stream << "sprite_" << i;
            YAML::Node spriteNode = yaml_file[stream.str()];

            _am.add(spriteNode["name"].as<std::string>(),
                    AnimatedSprite(_tm,
                    spriteNode["path"].as<std::string>(),
                    spriteNode["frames_nbr"].as<int>(),
                    spriteNode["width"].as<int>(),
                    spriteNode["height"].as<int>(),
                    spriteNode["center_pos"]["x"].as<int>(),
                    spriteNode["center_pos"]["y"].as<int>(),
                    0));
        }
    }


};

}
#endif // SYSTEM_HPP
