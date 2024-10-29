#include "GSource.h"
#include <GAnimation.h>
GSource &getSource() { return GSource::getSource(); }

sf::Sprite &getSprite(int id) { return getSource().getSprite(id); }

void GSource::loadAnimation(std::string pathAnimation) {
    xmlRead xr(pathAnimation.c_str());
    for (auto &pair : xr.mapData) {
        
       

        //  animations[pair.first] = {pair.second};不是原地构造，发生了拷贝
       
        

        //   animations.emplace(std::piecewise_construct,
        //                      std::forward_as_tuple(pair.first),
        //                      std::forward_as_tuple(pair.second));//原地构造
          animations.emplace(pair.first,pair.second);//原地构造
    }
}
