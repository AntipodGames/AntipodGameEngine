#ifndef QUADTREE_H
#define QUADTREE_H
#include <iostream>
#include <memory>
#include <vector>

namespace age {

template <class element>
class QuadTree
{
public:
    static const int max_nbr_elt;
    static const int max_depth;

    /**
     * @brief area_t
     * 0: x 1: y 2: width 3: height
     */
    typedef std::array<float,4> area_t;

    typedef std::shared_ptr<QuadTree> Ptr;
    typedef std::shared_ptr<const QuadTree> ConstPtr;

    QuadTree(){}
    QuadTree(int lvl, const area_t& a) :
        _area(a), _level(lvl){}
    QuadTree(const QuadTree& qt):
        _level(qt._level), _area(qt._area), _elts(qt._elts),
        _upleft(qt._upleft), _upright(qt._upright),
        _downleft(qt._downleft), _downright(qt._downright){}

    /**
     * @brief insert one element
     * @param _elt
     */
    void insert(element elt){

        if((_is_leaf && _elts.size() < max_nbr_elt) || _level >= max_depth){
            _elts.push_back(elt);
            return;
        }

        if(_is_leaf){
            _split();
            for(const element& e: _elts)
                _add(e);
            _elts.clear();
        }

        _add(elt);
    }

    /**
     * @brief empty the quadtree
     */
    void clear(){
        if(_is_leaf){
            _elts.clear();
            return;
        }

        if(!_is_leaf){
            _upleft->clear();
            _upright->clear();
            _downleft->clear();
            _downright->clear();
        }
    }

    /**
     * @brief number of element in this node
     */
    int node_size(){return _elts.size();}

    /**
     * @brief number of element in the tree
     */
    int size(){
        if(_is_leaf)
            return node_size();

        int s = 0;
        s += _upleft->size();
        s += _upright->size();
        s += _downright->size();
        s += _downleft->size();
        return s;
    }

    /**
     * @brief depth of the tree
     */
    int depth(){
        if(_is_leaf)
            return _level;

        int l[4];
        l[0] = _upleft->depth();
        l[1] = _upright->depth();
        l[2] = _downright->depth();
        l[3] = _downleft->depth();
        int max_l = l[0];
        for(int i = 1; i < 4 ; i++)
            if(l[i] > max_l)
                max_l = l[i];
        return max_l;
    }



    bool is_leaf() const {return _is_leaf;}

    const QuadTree::ConstPtr get_upleft() const {return _upleft;}
    const QuadTree::ConstPtr get_upright() const {return _upright;}
    const QuadTree::ConstPtr get_downleft() const {return _downleft;}
    const QuadTree::ConstPtr get_downright() const {return _downright;}

    const std::vector<element>& get_elts() const {return _elts;}

    /**
     * @brief get an element from its graphical position
     * @param x
     * @param y
     * @return
     */
    const element& get(float x, float y){
        // TO DO
    }

    /**
     * @brief move an element in this from (start_x,start_y) to (goal_x,goal_y)
     * @param start_x
     * @param start_y
     * @param goal_x
     * @param goal_y
     */
    void move(float start_x, float start_y, float goal_x, float goal_y){
        // TO DO
    }

    /**
     * @brief remove an element from its graphical location
     * @param x
     * @param y
     */
    void remove(float x, float y){
        // TO DO
    }

private:
    void _split(){
        _is_leaf = false;
        _upleft.reset(new QuadTree(_level+1,
                                   std::make_tuple(
                                    _area[0],_area[1],
                                    _area[2]/2,_area[3]/2)));
        _upright.reset(new QuadTree(_level+1,
                                    std::make_tuple(
                                    _area[0] + _area[2]/2,
                                    _area[1],
                                    _area[2]/2,_area[3]/2)));
        _downleft.reset(new QuadTree(_level+1,
                                     std::make_tuple(
                                     _area[0],
                                     _area[1] + _area[3]/2,
                                     _area[2],_area[3]/2)));
        _downright.reset(new QuadTree(_level+1,
                                      std::make_tuple(
                                      _area[0] + _area[2]/2,
                                      _area[1] + _area[3]/2,
                                      _area[2]/2,_area[3]/2)));
    }

    void _add(element elt){
        float mid_pt_x = _area[0] + _area[2]/2;
        float mid_pt_y = _area[1] + _area[3]/2;

        if(elt.get_center()[0] <= mid_pt_x && elt.get_center()[1] <= mid_pt_y)
            _upleft->insert(elt);
        else if(elt.get_center()[0] > mid_pt_x && elt.get_center()[1] <= mid_pt_y)
            _upright->insert(elt);
        else if(elt.get_center()[0] <= mid_pt_x && elt.get_center()[1] > mid_pt_y)
            _downleft->insert(elt);
        else if(elt.get_center()[0] > mid_pt_x && elt.get_center()[1] > mid_pt_y)
            _downright->insert(elt);
    }

    bool _is_leaf = true;
    int _level;
    area_t _area;
    std::vector<element> _elts;
    QuadTree::Ptr _upleft;
    QuadTree::Ptr _upright;
    QuadTree::Ptr _downleft;
    QuadTree::Ptr _downright;
};


}// age

#endif // QUADTREE_H
