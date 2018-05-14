#ifndef EMPTYCOLLIDER_H
#define EMPTYCOLLIDER_H

#include <iostream>
#include <memory>
#include <tuple>
#include <cmath>
#include <map>
#include <QObject>
#include <eigen3/Eigen/Core>
#include "types.hpp"
#include "quadtree.hpp"
#define PI  3.14159

namespace age {

inline float distance(float x1, float y1, float x2, float y2){
    return std::sqrt((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2));
}

class EmptyCollider : public QObject
{
    Q_OBJECT
public:
    typedef enum collider_type_t{
        EMPTY,BOX,CIRCLE,ELLIPSE
    } collider_type_t;
    typedef std::shared_ptr<EmptyCollider> Ptr;
    typedef std::shared_ptr<const EmptyCollider> ConstPtr;

    typedef std::map<std::string,Types::Ptr> _property_t;

    EmptyCollider() : QObject(){
        _collider_type = EMPTY;
        _center[0] = 0;
        _center[1] = 0;
    }
    EmptyCollider(const std::array<float,2>& center) : QObject(),
        _center(center){
        _collider_type = EMPTY;
    }
    EmptyCollider(const EmptyCollider& ec) : QObject(),
        _center(ec._center){
        _collider_type = EMPTY;
    }


    virtual bool is_colliding(const EmptyCollider::Ptr){return false;}

    const std::array<float,2>& get_center(){return _center;}


    std::vector<EmptyCollider::Ptr> collisions;

    void move(float x, float y){_center[0] = x; _center[1] = y;}
    void set_prop(const _property_t& prop){_prop = prop;}
    const _property_t& get_prop(){return _prop;}
    collider_type_t get_collider_type(){return _collider_type;}

signals:
    void collision(const _property_t&);
    void no_collision();

protected:
    std::array<float,2> _center;
    _property_t _prop;
    collider_type_t _collider_type;
};

class BoxCollider : public EmptyCollider
{
public:
    typedef std::shared_ptr<BoxCollider> Ptr;
    typedef std::shared_ptr<const BoxCollider> ConstPtr;

    BoxCollider(){_collider_type = BOX;}
    BoxCollider(float width, float height, float x, float y){
        _collider_type = BOX;
        _width = width;
        _height = height;
        _center[0] = x;
        _center[1] = y;
    }
    BoxCollider(const BoxCollider &bc) : EmptyCollider(bc),
        _width(bc._width), _height(bc._height){
        _collider_type = BOX;
    }

    bool is_in(float x, float y);
    bool is_colliding(const BoxCollider::Ptr bc);

    float get_width(){return _width;}
    float get_height(){return _height;}

private:
    float _width;
    float _height;
};

class CircleCollider : public EmptyCollider
{
public:
    typedef std::shared_ptr<CircleCollider> Ptr;
    typedef std::shared_ptr<const CircleCollider> ConstPtr;

    CircleCollider(float radius) : EmptyCollider(),
        _radius(radius){
        _collider_type = CIRCLE;
    }
    CircleCollider(float radius, std::array<float,2> center) :
        _radius(radius){
        _center = center;
        _collider_type = CIRCLE;
    }
    CircleCollider(const CircleCollider& cc) :
       EmptyCollider(cc), _radius(cc._radius){
        _collider_type = CIRCLE;
    }

    bool is_in(float x, float y);
    bool is_colliding(const CircleCollider::Ptr cc);

    float get_radius(){return _radius;}

private:
    float _radius;
};

class EllipseCollider : public EmptyCollider
{
public :
    typedef std::shared_ptr<EllipseCollider> Ptr;
    typedef std::shared_ptr<const EllipseCollider> ConstPtr;

    EllipseCollider(){
        _collider_type = ELLIPSE;
    }
    EllipseCollider(float x,float y,float rad,float rad2)
    {
        _collider_type = ELLIPSE;
        _center[0] = x;
        _center[1] = y;
        _angle = 0;
        float r1x = rad*std::cos(_angle);
        float r1y = rad*std::sin(_angle);
        float r2x = rad2*std::cos(_angle+PI/2.);
        float r2y = rad2*std::sin(_angle+PI/2.);
        _axis(0,0) = r1x*r1x;
        _axis(0,1) = r1y*r1y;
        _axis(1,1) = r2x*r2x;
        _axis(1,0) = r2y*r2y;
    }
    EllipseCollider(const EllipseCollider &ec) :
        EmptyCollider(ec),_angle(ec._angle), _axis(ec._axis){
        _collider_type = ELLIPSE;
    }

//    bool is_in(float x, float y);
//    bool is_colliding(const EllipseCollider::Ptr ec);

private:
    float _angle;
    Eigen::Vector2f _center;
    Eigen::Matrix2f _axis;
};

class CollisionManager : public QObject
{
    Q_OBJECT
public:

    typedef std::shared_ptr<CollisionManager> Ptr;
    typedef std::shared_ptr<const CollisionManager> ConstPtr;

    CollisionManager(){
        _qt.reset(new QuadTree<EmptyCollider>(0,{0,0,800,600}));
    }

    CollisionManager(const CollisionManager& cm) : _qt(cm._qt){}

    void check_collision();
    void insert(EmptyCollider::Ptr collider);

    QuadTree<EmptyCollider>::Ptr get_quadtree(){return _qt;}

public slots:
    void move_collider(EmptyCollider::Ptr clld, float x, float y);

private:
    void _collisions_in(const QuadTree<EmptyCollider>::ConstPtr qt);

    QuadTree<EmptyCollider>::Ptr _qt;

};

}

#endif // EMPTYCOLLIDER_H
