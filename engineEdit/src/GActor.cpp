#include "GActor.h"
#include <GCamera.h>
#include <GCollision.h>
#include <game.h>
sf::Texture GActor::tex;
sf::Sprite GActor::spr;
long GActor::drawCallNum=0;
void GActor::eventTick()
{
    IVector psInWin = wsToWin(posInWs);
    sprPt->setPosition(psInWin.x, psInWin.y - z / pixSize);
    getWindow()->draw(*sprPt);
}

void GActor::dataLoop() {}

void GActor::destroyActor()
{
    isValid = 0;
    if (isValid)
        destroyActor();
}

FVector3 &GActor::getPosInWs() { return posInWs; }

void GActor::setPosInWs(const FVector3 &pos_)
{
    posInWs = pos_;
    for (auto component : allActorComponents)
    {
        bindActorComponent(component);
    }
    if (collisionForMove)
        collisionForMove->setPosition(pos_);
}

void GActor::setRenderSprite(sf::Sprite *sprPt_) { sprPt = sprPt_; }

sf::Sprite *GActor::getRenderSprite() { return sprPt; }

bool GActor::addWsPosOffset(const FVector3 &vec)
{
    if (collisionForMove && collisionForMove->isCollisionOn())
    {
        FVector3 testPos = getPosInWs() + vec;
        collisionForMove->setPosition(testPos);
        if (collisionForMove->isStock())
        {
            collisionForMove->setPosition(getPosInWs());
            return 0;
        }
        else
        {
            setPosInWs(testPos);
            return 1;
        }
    }
    else
        setPosInWs(getPosInWs() + vec);
    return 1;
}

void GActor::drawActor()
{
    IVector psInWin = wsToWin(posInWs);
    sprPt->setPosition(psInWin.x, psInWin.y);
    getWindow()->draw(*sprPt);
    drawCallNum++;
}

void GActor::bindActorComponent(GActorComponent *ptr)
{
    ptr->setPosInWs(posInWs + ptr->getRelativePosition());
}

GActor::GActor()
{
    addActors(this);
    setRenderSprite(&spr);
}

GActor::~GActor()
{

    for (auto component : allComponents)
    {
        delete component;
        component = nullptr;
    }
    for (auto component : allActorComponents)
    {
        component->destroyActor();
        component = nullptr;
    }
}
