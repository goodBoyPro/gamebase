#include "GMouse.h"
#include<game.h>
GMouse::GMouse() { tex.loadFromFile("res/mouse.png");
    spr.setTexture(tex);
    spr.setScale(0.3,0.3);
    setMouseVisible(*(Game::gameIns->gameWindow),0);
}

GMouse::~GMouse() {}
