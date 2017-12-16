#ifndef QSYSTEM_HPP
#define QSYSTEM_HPP

#include <iostream>
#include <SFML/Graphics.hpp>
#include <QWidget>
#include <QTimer>
#include <memory>
#include <AGE/parameters.hpp>
#include <yaml-cpp/yaml.h>

#ifdef Q_WS_X11
    #include <Qt/qx11info_x11.h>
    #include <X11/Xlib.h>
#endif //Q_WS_X11


namespace age {


class QSystem : public QWidget, public sf::RenderWindow
{
    Q_OBJECT
public:

    QSystem(QWidget *parent) :
        QWidget(parent), _is_init(false)
    {
        setAttribute(Qt::WA_PaintOnScreen);
        setAttribute(Qt::WA_OpaquePaintEvent);
        setAttribute(Qt::WA_NoSystemBackground);

        setFocusPolicy(Qt::StrongFocus);

    }

    QSystem(QWidget *parent,const QPoint& Position, const QSize& Size) :
        QWidget(parent), _is_init(false)
    {
        setAttribute(Qt::WA_PaintOnScreen);
        setAttribute(Qt::WA_OpaquePaintEvent);
        setAttribute(Qt::WA_NoSystemBackground);

        setFocusPolicy(Qt::StrongFocus);

        _size[0] = Size.width();
        _size[1] = Size.height();
        _position[0] = static_cast<float>(Position.x());
        _position[1] = static_cast<float>(Position.y());

        _work_dir = "";

        _initialize();

    }

    virtual ~QSystem(){
    }

//public slots:
//    void paint(){
//        repaint(_position.x(),_position.y(),_size.width(),_size.height());
//    }

    std::array<float,2> get_size(){return _size;}
    std::array<float,2> get_position(){return _position;}
protected:
    virtual void _init() = 0;
    virtual void _update() = 0;

    void _init_view(int x, int y, int width, int height){
        sf::Vector2f viewCenter(x,y);
        sf::Vector2f viewSize(width,height);
        _view = sf::View(viewCenter,viewSize);
    }

    void _initialize(){

        move(QPoint(_position[0],_position[1]));
        setMaximumSize(QSize(_size[0],_size[1]));
        setMinimumSize(QSize(_size[0],_size[1]));

        _timer.reset(new QTimer);
        _timer->setInterval(parameters::frame_rate);

        _init_view(_size[0]/2.,_size[1]/2.,_size[0],_size[1]);

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


            // On paramètre le timer de sorte qu'il génère un rafraîchissement à la fréquence souhaitée
            connect(_timer.get(), SIGNAL(timeout()), this, SLOT(repaint()));
            _timer->start();

            _is_init = true;
        }
    }


    sf::View _view;
    std::unique_ptr<QTimer> _timer;
    bool _is_init;

    std::array<float,2> _position;
    std::array<float,2> _size;
    std::string _graphics;
    std::string _work_dir;
};

}
#endif // QSYSTEM_HPP
