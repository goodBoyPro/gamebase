#include "GMouse.h"

GMouse::GMouse() { tex.loadFromFile("res/mouse.png");
    spr.setTexture(tex);
    spr.setScale(0.3,0.3);
    setMouseVisible(0);
}

GMouse::~GMouse() {}
