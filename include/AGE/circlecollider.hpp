#ifndef CIRCLECOLLIDER_H
#define CIRCLECOLLIDER_H
#include <cmath>
#include <SFML/Graphics.hpp>
#include <eigen3/Eigen/Core>
#define PI  3.14159


namespace age{

class CircleCollider
{
public:
    CircleCollider(){}

    CircleCollider(const CircleCollider& cc) :
        _center(cc._center),
        _angle(cc._angle),
        _front_angle(cc._front_angle),
        _axis(cc._axis){}

    CircleCollider(float x,float y,float rad,float rad2) :
        _radius1(rad), _radius2(rad2){
        _center[0] = x;
        _center[1] = y;
        _angle = 0;
        _front_angle = 0;
        float r1x = rad*std::cos(_angle);
        float r1y = rad*std::sin(_angle);
        float r2x = rad2*std::cos(_angle+PI/2.);
        float r2y = rad2*std::sin(_angle+PI/2.);
        _axis(0,0) = r1x*r1x;
        _axis(0,1) = r1y*r1y;
        _axis(1,1) = r2x*r2x;
        _axis(1,0) = r2y*r2y;
    }

    bool isIN(float x,float y);
    void set_position(float x, float y){_center[0] = x, _center[1] = y;}
    void rotate(float angle){_angle=angle;}
    void rotate_front(float angle){_front_angle=angle;}

    /**
     * @brief isColor
     * @param map of the environment
     * @param test color
     * @return true if the pixel in front of the collider is [color] false otherwise
     */
    bool isColor(sf::Sprite map, sf::Color color);

    /**
     * @brief intersection
     * @param a circlecollider
     * @return true if cc and this intersect each other.
     */
    bool intersection(const CircleCollider& cc);

    /**
     * @brief front_intersection
     * @param cc
     * @return if this intersect cc on his front.
     */
    bool front_intersection(const CircleCollider& cc);

    float intersection_angle();

private:
    float _angle;
    float _front_angle;

    Eigen::Vector2f _center;
    Eigen::Matrix2f _axis;

    //*Fonctions internes
    float _distance(float,float,float,float);
    int _distance(int,int,int,int);
    //*/
};

}

#endif // CIRCLECOLLIDER_H
