#ifndef CIRCLECOLLIDER_H
#define CIRCLECOLLIDER_H
#include <SFML/Graphics.hpp>

namespace age{

class CircleCollider
{
public:
    CircleCollider();

    CircleCollider(const CircleCollider& cc) :
        _x(cc._x), _y(cc._y), _radius1(cc._radius1), _radius2(cc._radius2)
    ,_angle(cc._angle), _front_angle(cc._front_angle){}

    CircleCollider(float x,float y,float rad,float rad2) :
        _x(x), _y(y),_radius1(rad), _radius2(rad2){
        _angle = 0;
        _front_angle = 0;
    }

    bool isIN(float x,float y);
    void set_position(float x, float y){_x = x, _y = y;}
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
    float _x;
    float _y;
    float _radius1, _radius2;
    float _angle;
    float _front_angle;

    //*Fonctions internes
    float _distance(float,float,float,float);
    int _distance(int,int,int,int);
    //*/
};

}

#endif // CIRCLECOLLIDER_H
