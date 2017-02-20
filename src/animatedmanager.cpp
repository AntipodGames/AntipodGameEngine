#include "AGE/animatedmanager.h"

using namespace age;


void AnimatedManager::add(const std::string& label, AnimatedSprite image){
    BDD.insert(std::make_pair(label,image));


    if(frameBDD.find(label) != frameBDD.end()){
        std::vector<int> tmp(frameBDD[label]);
        tmp.push_back(1);
        frameBDD.emplace(label,tmp);
    }
    else{
        std::vector<int> tmp;
        tmp.push_back(1);
        frameBDD.emplace(label,tmp);
    }
}

AnimatedSprite& AnimatedManager::get(const std::string& label){
   return BDD[label];

}

void AnimatedManager::set(const std::string& label, const std::string& lbl){
    BDD[label] = BDD[lbl];
}

void AnimatedManager::changeFrame(const std::string& label, int n){
    std::vector<int> tmp(frameBDD[label]);
    AnimatedSprite tmpS(BDD[label]);
    if(tmp[n] >= tmpS.get_current_state().nbr_frames)
        tmp[n] == 0;

    tmp[n]+=1;

    frameBDD.emplace(label,tmp);
}

void AnimatedManager::deleteFrame(const std::string& label, int n){
    std::vector<int> tmp(frameBDD[label]);

    tmp.erase(tmp.begin() + n);
    tmp.shrink_to_fit();

    frameBDD.emplace(label,tmp);

}
