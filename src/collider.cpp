#include <AGE/collider.hpp>

using namespace age;

bool BoxCollider::is_in(float x, float y){
    return fabs(x - _center[0]) < _width/2. && fabs(y - _center[1]) < _height/2.;
}

bool BoxCollider::is_colliding(const BoxCollider::Ptr bc){
    return fabs(bc->_center[0] - _center[0]) < _width/2. + bc->_width/2. && fabs(bc->_center[1] - _center[1]) < _height/2. + bc->_height;
}


void CollisionManager::check_collision(){

}

void CollisionManager::_collisions_in(const QuadTree<EmptyCollider::Ptr>::ConstPtr qt){
    if(qt->is_leaf()){
        for(size_t i = 0; i < qt->get_elts().size(); i++){
            for(size_t j = 0; j < qt->get_elts().size(); j++){
                if(i!=j){
                    if(qt->get_elts()[i]->is_colliding(qt->get_elts()[j]))
                        emit qt->get_elts()[i]->collision();

                }
            }
        }
        return;
    }

    _collisions_in(qt->get_upleft());
    _collisions_in(qt->get_upright());
    _collisions_in(qt->get_downleft());
    _collisions_in(qt->get_downright());

}
