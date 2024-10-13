#include "GSource.h"
#include <GAnimation.h>
GSource &getSource() { return GSource::getSource(); }

sf::Sprite &getSprite(int id) { return getSource().texSpr[id].spr; }

void GSource::loadAnimation() {
    printf("ok");
    xmlRead xr("res/animData.xml");
    printf("%s",xr[1]["path"].c_str());
    for (auto &pair : xr.mapData) {
        animations.emplace(
            std::piecewise_construct, std::forward_as_tuple(pair.first),
            std::forward_as_tuple(
                pair.second["path"].c_str(), stoi(pair.second["sizex"]),
                stoi(pair.second["sizey"]), stoi(pair.second["row"]),
                stoi(pair.second["column"])));
                
    }
}
