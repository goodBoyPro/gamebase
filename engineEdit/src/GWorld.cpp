#include "GWorld.h"
#include <GPlayerChar.h>
#include <actorTest.h>

GWorld::GWorld() {
    // 写在子类中
    //  tex.loadFromFile("bin/res/map.png");
    //  spr.setTexture(tex);
    //  spr.setPosition(0,0);

    // playerCharacter = new GPlayerChar;
    sprPt = &spr;   
    setWorld(this);
    
}

GWorld::~GWorld() {}

void GWorld::dataLoop() {}

void GWorld::drawLoop() {
    sprPt->setPosition(wsToWin(posWs).x, wsToWin(posWs).y);
    getWindow()->draw(*sprPt);
}

