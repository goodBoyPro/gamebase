#include "GActor.h"
#include <GCamera.h>
#include <game.h>
void GActor::drawLoop() {
    IVector psInWin = wsToWin(posInWs);
    sprPt->setPosition(psInWin.x, psInWin.y-z/pixSize);
    window->draw(*sprPt);
}

void GActor::dataLoop() {}

void GActor::destroyActor() {
    isValid = 0;
    if (isValid)
        destroyActor();
}

GActor::GActor() {
    addActors(this);
    sprPt = &spr;
}

GActor::~GActor() {}
