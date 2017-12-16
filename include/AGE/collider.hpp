#ifndef EMPTYCOLLIDER_H
#define EMPTYCOLLIDER_H

#include <iostream>
#include <memory>
#include <tuple>
#include <cmath>
#include <QObject>
#include <eigen3/Eigen/Core>
#include <AGE/quadtree.hpp>
#define PI  3.14159

namespace age {

inline float distance(float x1, float y1, float x2, float y2){
    return std::sqrt((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2));
}

class EmptyCollider : public QObject
{
    Q_OBJECT
public:
    typedef std::shared_ptr<EmptyCollider> Ptr;
    typedef std::shared_ptr<const EmptyCollider> ConstPtr;

    EmptyCollider() : QObject(){}
    EmptyCollider(const std::array<float,2>& center) : QObject(),
        _center(center){}
    EmptyCollider(const EmptyCollider& ec) : QObject(),
        _center(ec._center){}


    virtual bool is_colliding(const EmptyCollider::Ptr){return false;}

    const std::array<float,2>& get_center(){return _center;}


    std::vector<EmptyCollider::Ptr> collisions;

signals:
    void collision();

protected:
    std::array<float,2> _center;
};

class BoxCollider : public EmptyCollider
{
public:
    typedef std::shared_ptr<BoxCollider> Ptr;
    typedef std::shared_ptr<const BoxCollider> ConstPtr;

    BoxCollider(){}
    BoxCollider(float width, float height, float x, float y){
        _width = width;
        _height = height;
        _center[0] = x;
        _center[1] = y;
    }
    BoxCollider(const BoxCollider &bc) : EmptyCollider(bc),
        _width(bc._width), _height(bc._height){}

    bool is_in(float x, float y);
    bool is_colliding(const BoxCollider::Ptr bc);

private:
    float _width;
    float _height;
};

class CircleCollider : public EmptyCollider
{
public:
    typedef std::shared_ptr<CircleCollider> Ptr;
    typedef std::shared_ptr<const CircleCollider> ConstPtr;

    CircleCollider(){}
    CircleCollider(float radius, std::array<float,2> center) :
    _radius(radius){
        _center = center;
    }
    CircleCollider(const CircleCollider& cc) :
       EmptyCollider(cc), _radius(cc._radius){}

    bool is_in(float x, float y);
    bool is_colliding(const CircleCollider::Ptr cc);

private:
    float _radius;
};

class EllipseCollider : public EmptyCollider
{
public :
    typedef std::shared_ptr<EllipseCollider> Ptr;
    typedef std::shared_ptr<const EllipseCollider> ConstPtr;

    EllipseCollider(){}
    EllipseCollider(float x,float y,float rad,float rad2)
    {
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
        EmptyCollider(ec),_angle(ec._angle), _axis(ec._axis){}

    bool is_in(float x, float y);
    bool is_colliding(const EllipseCollider::Ptr ec);

private:
    float _angle;
    Eigen::Vector2f _center;
    Eigen::Matrix2f _axis;
};

class CollisionManager
{
public:
    void check_collision();

private:
    void _collisions_in(const QuadTree<EmptyCollider::Ptr>::ConstPtr qt);

    QuadTree<EmptyCollider::Ptr> _qt;

};

}

#endif // EMPTYCOLLIDER_H
