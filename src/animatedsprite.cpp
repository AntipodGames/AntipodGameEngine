#include "AGE/animatedsprite.h"

using namespace age;

//TODO Clean this class. In particular the constructor. One must remain ! (with default and copy of course)

AnimatedSprite::AnimatedSprite(TextureManager &TM, std::string sprite,
                               std::string anim_yml, int width, int height,
                               int centreX, int centreY){
    image.setTexture(*(TM.GetTexture(sprite)));

    //TODO create animation graph
    YAML::Node yaml_file = YAML::LoadFile(anim_yml);
    YAML::Node anim_graph = yaml_file["animation_graph"];
    _anim_graph = _animation_graph(anim_graph);

    float scale1 = ((float)width*nbrFrame)/(float)image.getTexture()->getSize().x;
    float scale2 = (float)height/(float)image.getTexture()->getSize().y;

    image.setScale(scale1,scale2);

    image.setOrigin(image.getTexture()->getSize().x/(centreX*nbrFrame),image.getTexture()->getSize().y/centreY);
    image.setTextureRect(sf::IntRect(0,0,image.getTexture()->getSize().x/nbrFrame,image.getTexture()->getSize().y));
}

/**
 * Constructeur d'image animé
 * @brief AnimatedSprite::AnimatedSprite
 * @param IM
 * @param adr
 * @param nbrF
 * @param width
 * @param height
 * @param centreX
 * @param centreY
 */
AnimatedSprite::AnimatedSprite(TextureManager & TM, std::string adr,int nbrF, int width, int height, int centreX, int centreY,int time)
{
    std::string imAdr(adr);
//    imAdr.append(".png");
//    std::string dataAdr(adr);
//    dataAdr.append(".data");

    image.setTexture(*(TM.GetTexture(imAdr)));

    timer = time;
    frame = 1;
    cpt = 0;
    nbrFrame = nbrF;
    float scale1 = ((float)width*nbrFrame)/(float)image.getTexture()->getSize().x;
    float scale2 = (float)height/(float)image.getTexture()->getSize().y;

    image.setScale(scale1,scale2);

    image.setOrigin(image.getTexture()->getSize().x/(centreX*nbrFrame),image.getTexture()->getSize().y/centreY);
    image.setTextureRect(sf::IntRect(0,0,image.getTexture()->getSize().x/nbrFrame,image.getTexture()->getSize().y));
   // dataAnalyser(width,dataAdr);

}

/**
 * @brief AnimatedSprite::AnimatedSprite
 * @param TM
 * @param adr
 * @param nbrF
 * @param size
 * @param centreX
 * @param centreY
 * @param time
 */
AnimatedSprite::AnimatedSprite(TextureManager & TM, std::string adr,int nbrF,int size, int centreX, int centreY,int time)
{
    std::string imAdr(adr);
//    imAdr.append(".png");
//    std::string dataAdr(adr);
//    dataAdr.append(".data");

    image.setTexture(*(TM.GetTexture(imAdr)));

    timer = time;
    frame = 1;
    cpt = 0;
    nbrFrame = nbrF;
    scale = size;

    image.setScale(((float)size*nbrFrame)/((float)image.getTexture()->getSize().x),
                   ((float)image.getTexture()->getSize().y)/((float)image.getTexture()->getSize().x)
                   *((float)size*nbrFrame)/((float)image.getTexture()->getSize().y));

    image.setOrigin(image.getTexture()->getSize().x/(centreX*nbrFrame),image.getTexture()->getSize().y/centreY);
    image.setTextureRect(sf::IntRect(0,0,image.getTexture()->getSize().x/nbrFrame,image.getTexture()->getSize().y));
   // dataAnalyser(width,dataAdr);

}
AnimatedSprite::AnimatedSprite(TextureManager & TM, std::string adr, int nbrF, int size, bool centrer, int time)
{
    std::string imAdr(adr);
//    imAdr.append(".png");
//    std::string dataAdr(adr);
//    dataAdr.append(".data");

    image.setTexture(*(TM.GetTexture(imAdr)));

    timer = time;
    frame = 1;
    cpt = 0;
    nbrFrame = nbrF;
    scale = size;
    image.setTextureRect(sf::IntRect(0,0,image.getTexture()->getSize().x/nbrFrame,image.getTexture()->getSize().y));

    image.setScale(((float)size*nbrFrame)/((float)image.getTexture()->getSize().x),
                   ((float)image.getTexture()->getSize().y)/((float)image.getTexture()->getSize().x)
                   *((float)size*nbrFrame)/((float)image.getTexture()->getSize().y));
    if(centrer){
     //   if(nbrFrame == 1)
       //     image.SetCenter(0,image.GetImage()->GetHeight()/2);
       // else
        image.setOrigin(image.getTexture()->getSize().x/(2*(nbrFrame)),image.getTexture()->getSize().y/2);
    }
   // dataAnalyser(width,dataAdr);

}



void AnimatedSprite::show(sf::RenderWindow &App){
    App.draw(image);
}

sf::Vector2f AnimatedSprite::TransformToLocal(const sf::Vector2f &Pt){
    return TransformToLocal(Pt);
}

void AnimatedSprite::setFrame(int i){
    frame = i%nbrFrame+1;
    image.setTextureRect(sf::IntRect((image.getTexture()->getSize().x/nbrFrame)*(frame-1)
                                     ,0
                                     ,(image.getTexture()->getSize().x/nbrFrame)
                                     ,image.getTexture()->getSize().y));

}

int AnimatedSprite::getFrame(){
    return frame;
}

int AnimatedSprite::getNbrFrame(){
    return nbrFrame;
}

void AnimatedSprite::Rotate(float angle){
    image.rotate(angle/PI*180);

}

void AnimatedSprite::setAngle(float angle){
    image.setRotation(angle/PI*180);
}

float AnimatedSprite::getAngle(){
    return image.getRotation()/180*PI;
}

void AnimatedSprite::setPosition(int x, int y){
    image.setPosition(x,y);
}

sf::Sprite &AnimatedSprite::getImage(){
    return image;
}

int AnimatedSprite::getScale(){
    return scale;
}

void AnimatedSprite::resize(int x, int y){
    image.setScale((float)(x*nbrFrame)/((float)image.getTexture()->getSize().x)
                   ,((float)y)/((float)image.getTexture()->getSize().y));
}

AnimatedSprite::_animation_graph::_animation_graph(const YAML::Node &yaml_node){

    for(auto it = yaml_node.begin(); it != yaml_node.end(); ++it){
        _states_desc_t description;
        description.nbr_frames = it->second["nbr_frames"].as<int>();
        description.neighbors = std::vector<std::string>(it->second["neighbors"].size());
        for(size_t i  = 0; i < it->second["neighbors"].size(); i++)
            description.neighbors[i] = it->second["neighbors"][i].as<std::string>();
        description.position = it->second["position"].as<int>();
        description.time_per_frame = it->second["time_per_frame"].as<float>();
        _graph.emplace(it->first.as<std::string>(),description);
    }
}
