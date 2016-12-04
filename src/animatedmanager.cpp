#include "AGE/animatedmanager.h"

using namespace age;


void AnimatedManager::add(const std::string& label, AnimatedSprite image){
    BDD.insert(std::make_pair(label,image));


    if(frameBDD.contains(label)){
        QVector<int> tmp(frameBDD.value(label));
        tmp.append(1);
        frameBDD.insert(label,tmp);
    }
    else{
        QVector<int> tmp;
        tmp.append(1);
        frameBDD.insert(label,tmp);
    }
}

AnimatedSprite& AnimatedManager::get(const std::string& label){
   return BDD[label];

}

void AnimatedManager::set(const std::string& label, const std::string& lbl){
    BDD[label] = BDD[lbl];
}

void AnimatedManager::changeFrame(const std::string& label, int n){
    QVector<int> tmp(frameBDD[label]);
    AnimatedSprite tmpS(BDD[label]);
    if(tmp[n] >= tmpS.getNbrFrame())
        tmp[n] == 0;

    tmp[n]+=1;

    frameBDD.insert(label,tmp);
}

void AnimatedManager::deleteFrame(const std::string& label, int n){
    QVector<int> tmp(frameBDD[label]);

    tmp.remove(n);
    tmp.squeeze();

    frameBDD.insert(label,tmp);

}
