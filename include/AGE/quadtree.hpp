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
    int max_nbr_elt = 5;
    int max_depth = 0;

    typedef enum branch_t {
        UPLEFT = 0,
        UPRIGHT = 1,
        DOWNLEFT = 2,
        DOWNRIGHT = 3
    }branch_t;

    /**
     * @brief area_t
     * 0: x 1: y 2: width 3: height
     */
    typedef std::array<float,4> area_t;

    typedef std::shared_ptr<QuadTree> Ptr;
    typedef std::shared_ptr<const QuadTree> ConstPtr;

    QuadTree(){}
    QuadTree(int lvl, const area_t& a) :
        _level(lvl), _area(a){}
    QuadTree(const QuadTree& qt):
        _level(qt._level), _area(qt._area), _elts(qt._elts),
        _branch(qt._branch)
    {}

    /**
     * @brief insert one element
     * @param _elt
     */
    bool insert(std::shared_ptr<element> elt){

        if(!_is_in_area(elt->get_center()[0],elt->get_center()[1]))
                return false;

        if((_is_leaf && _elts.size() < max_nbr_elt) || _level >= max_depth){
//            if(!exist(elt))
            _elts.push_back(elt);
            return true;
        }

        if(_is_leaf){
            _split();
            for(const std::shared_ptr<element>& e: _elts)
                _branch[_in_which_branch(e->get_center()[0],e->get_center()[1])]->insert(e);
            _elts.clear();
        }

        _branch[_in_which_branch(elt->get_center()[0],elt->get_center()[1])]->insert(elt);
        return true;
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
            for(auto& branch : _branch)
                branch->clear();
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
        for(auto& branch : _branch)
            s += branch->size();
        return s;
    }

    /**
     * @brief depth of the tree
     */
    int depth(){
        if(_is_leaf)
            return _level;

        int l[4];
        for(int i = 0; i < 4; i++){
            l[i] = _branch[i]->depth();
        }
        int max_l = l[0];
        for(int i = 1; i < 4 ; i++)
            if(l[i] > max_l)
                max_l = l[i];
        return max_l;
    }



    bool is_leaf() const {return _is_leaf;}

    const QuadTree::ConstPtr get_upleft() const {return _branch[UPLEFT];}
    const QuadTree::ConstPtr get_upright() const {return _branch[UPRIGHT];}
    const QuadTree::ConstPtr get_downleft() const {return _branch[DOWNLEFT];}
    const QuadTree::ConstPtr get_downright() const {return _branch[DOWNRIGHT];}

    const std::vector<std::shared_ptr<element>>& get_elts() const {return _elts;}

    int get_level() const {return _level;}
    area_t get_area() const {return _area;}

    /**
     * @brief move an element in this from previous location (x,y) to is new location element->get_center
     * @param element
     * @param previous location x
     * @param previous location y
     */
    bool move(const std::shared_ptr<element>& elt,float x, float y){
        if(_is_leaf){
            if(!_is_in_area(elt->get_center()[0],elt->get_center()[1]) && _level != 0){
                remove(elt);
                return true;
            }
            else return false;
        }

        bool moved = false;

        moved = _branch[_in_which_branch(x,y)]->move(elt,x,y);

        if(moved && !insert(elt)){
            _prune();
            return true;
        }
        return false;
    }

    /**
     * @brief remove an element
     * @param elt
     */
    bool remove(const std::shared_ptr<element>& elt){
        if(_is_leaf){
            for(auto it = _elts.begin(); it != _elts.end(); it++){
                if(*it == elt){
                    _elts.erase(it);
                    return true;
                }
            }
            return false;
        }
        _branch[_in_which_branch(elt->get_center()[0],elt->get_center()[1])]->remove(elt);

        _prune();
    }


    bool exist(const std::shared_ptr<element>& elt){
        for(const std::shared_ptr<element>& e : _elts){
            if(e == elt)
                return true;
        }
        return false;
    }

private:
    void _split(){
        _is_leaf = false;
        _branch[UPLEFT].reset(new QuadTree(_level+1,
                                   {_area[0],_area[1],
                                    _area[2]/2,_area[3]/2}));
        _branch[UPRIGHT].reset(new QuadTree(_level+1,
                                   {_area[0] + _area[2]/2,
                                    _area[1],
                                    _area[2]/2,_area[3]/2}));
        _branch[DOWNLEFT].reset(new QuadTree(_level+1,
                                    {_area[0],
                                     _area[1] + _area[3]/2,
                                     _area[2]/2,_area[3]/2}));
        _branch[DOWNRIGHT].reset(new QuadTree(_level+1,
                                     {_area[0] + _area[2]/2,
                                      _area[1] + _area[3]/2,
                                      _area[2]/2,_area[3]/2}));
    }



    bool _prune(){
        bool to_prune = true;
        for(auto& branch: _branch)
            to_prune = to_prune && branch->is_leaf() && branch->get_elts().empty();
        if(to_prune){
            for(auto& branch: _branch)
                branch.reset();
            _is_leaf = true;
            return true;
        }
        return false;
    }

    bool _is_in_area(float x, float y){
        return x >= _area[0] && x < _area[0] + _area[2]
                && y >= _area[1] && y < _area[1] + _area[3];
    }

    branch_t _in_which_branch(float x, float y){
        float mid_pt_x = _area[0] + _area[2]/2;
        float mid_pt_y = _area[1] + _area[3]/2;


        if(x < mid_pt_x && y < mid_pt_y)
            return UPLEFT;
        else if(x >= mid_pt_x && y < mid_pt_y)
            return UPRIGHT;
        else if(x < mid_pt_x && y >= mid_pt_y)
            return DOWNLEFT;
        else if(x >= mid_pt_x && y >= mid_pt_y)
            return DOWNRIGHT;
    }

    bool _is_leaf = true;
    int _level = 0;
    area_t _area;
    std::vector<std::shared_ptr<element>> _elts;
    std::array<QuadTree::Ptr,4> _branch;
};


}// age

#endif // QUADTREE_H
