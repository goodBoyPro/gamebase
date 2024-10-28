#include "GSource.h"
#include <GAnimation.h>
GSource &getSource() { return GSource::getSource(); }

sf::Sprite &getSprite(int id) { return getSource().texSpr[id].spr; }

void GSource::loadAnimation() {
    xmlRead xr("res/animData.xml");
    for (auto &pair : xr.mapData) {
        // animations.emplace(std::piecewise_construct,
        //                    std::forward_as_tuple(pair.first),
        //                    std::forward_as_tuple(pair.second["path"].c_str(),
        //                                          stoi(pair.second["sizex"]),
        //                                          stoi(pair.second["sizey"]),
        //                                          stoi(pair.second["row"]),
        //                                          stoi(pair.second["column"])));//map原地构造
        // printf(pair.second["path"].c_str());

        //  animations[pair.first] = {pair.second};不是原地构造，发生了拷贝
       
        

          animations.emplace(std::piecewise_construct,
                             std::forward_as_tuple(pair.first),
                             std::forward_as_tuple(pair.second));
    }
}
