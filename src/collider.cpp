#include "../include/AGE/collider.hpp"

using namespace age;

bool BoxCollider::is_in(float x, float y){
    return fabs(x - _center[0]) < _width/2. && fabs(y - _center[1]) < _height/2.;
}

bool BoxCollider::is_colliding(const BoxCollider::Ptr bc){
    return fabs(bc->_center[0] - _center[0]) < _width/2. + bc->_width/2. && fabs(bc->_center[1] - _center[1]) < _height/2. + bc->_height;
}

bool CircleCollider::is_in(float x, float y){
    return fabs(x - _center[0]) + fabs(y - _center[1]) < _radius;
}

bool CircleCollider::is_colliding(const CircleCollider::Ptr cc){
    return sqrt((cc->_center[0] - _center[0])*(cc->_center[0] - _center[0]) +
            (cc->_center[1] - _center[1])*(cc->_center[1] - _center[1])) < _radius + cc->_radius;
}


void CollisionManager::check_collision(){
    _collisions_in(_qt);
}

void CollisionManager::insert(EmptyCollider::Ptr collider){
    _qt->insert(collider);
}

void CollisionManager::_collisions_in(const QuadTree<EmptyCollider>::ConstPtr qt){
    if(qt->is_leaf()){
        std::vector<bool> collision_free(_qt->get_elts().size(),true);
        bool is_colliding = false;
        for(size_t i = 0; i < qt->get_elts().size(); i++){
            for(size_t j = 0; j < qt->get_elts().size(); j++){
                if(i!=j){
                    if(qt->get_elts()[i]->get_collider_type() == EmptyCollider::BOX)
                        is_colliding = std::static_pointer_cast<BoxCollider>(qt->get_elts()[i])->
                            is_colliding(std::static_pointer_cast<BoxCollider>(qt->get_elts()[j]));
                    else if(qt->get_elts()[i]->get_collider_type() == EmptyCollider::CIRCLE)
                        is_colliding = std::static_pointer_cast<CircleCollider>(qt->get_elts()[i])->
                            is_colliding(std::static_pointer_cast<CircleCollider>(qt->get_elts()[j]));
                    else if(qt->get_elts()[i]->get_collider_type() == EmptyCollider::ELLIPSE)
                        is_colliding = std::static_pointer_cast<EllipseCollider>(qt->get_elts()[i])->
                            is_colliding(std::static_pointer_cast<EllipseCollider>(qt->get_elts()[j]));

                    collision_free[i] = collision_free[i] && !is_colliding;
                    collision_free[j] = collision_free[j] && !is_colliding;
                    if(is_colliding)
                        emit qt->get_elts()[i]->collision(qt->get_elts()[j]->get_prop());
                }
            }
        }
        for(int i = 0; i < collision_free.size(); i++)
            if(collision_free[i])
                emit qt->get_elts()[i]->no_collision();
        return;
    }

    _collisions_in(qt->get_upleft());
    _collisions_in(qt->get_upright());
    _collisions_in(qt->get_downleft());
    _collisions_in(qt->get_downright());

}

void CollisionManager::move_collider(EmptyCollider::Ptr clld, float x, float y){
    _qt->move(clld,x,y);
}
