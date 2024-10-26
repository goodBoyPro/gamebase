#include "GActor.h"
#include <GCamera.h>
#include <game.h>

void GActor::drawLoop() {
    IVector psInWin = wsToWin(posInWs);
    sprPt->setPosition(psInWin.x, psInWin.y-z/pixSize);
    getWindow()->draw(*sprPt);
}

void GActor::dataLoop() {}

void GActor::destroyActor() {
    isValid = 0;
    if (isValid)
        destroyActor();
}

FVector& GActor::getPosInWs() { return posInWs; }

void GActor::setPosInWs(const FVector &pos_) { posInWs = pos_; }

void GActor::setRenderSprite(sf::Sprite *sprPt_) { sprPt = sprPt_; }

sf::Sprite *GActor::getRenderSprite() { return sprPt; }

void GActor::addWsPosOffset(const FVector &vec) {
    setPosInWs(getPosInWs() + vec);
}

GActor::GActor() {
    addActors(this);
    setRenderSprite(&spr);
}

GActor::~GActor() {}
