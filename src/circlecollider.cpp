#include "AGE/circlecollider.hpp"

using namespace age;

bool CircleCollider::isIN(float x, float y){
    Eigen::Vector2f X(x,y);
    return (X-_center).transpose()*_axis.inverse()*(X-_center) <= 1.;
}

bool CircleCollider::intersection(const CircleCollider &cc){

}
