#include "GActor.h"
#include <GCamera.h>
#include <GCollision.h>
#include <game.h>
sf::Texture GActor::tex;
sf::Sprite GActor::spr;
long GActor::drawCallNum=0;
GridMap<GActor *> GActor::gridMapOfActor={FVector2(-400,-400),500,500,5,5};
void GActor::eventTick()
{
    
    
}

void GActor::dataLoop() {}

void GActor::destroyActor()
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
    isValid = 0;
    // if (isValid)
    //     destroyActor();
}

FVector3 &GActor::getPosInWs() { return posInWs; }

void GActor::setPosInWs(const FVector3 &pos_)
{
    int id=GActor::gridMapOfActor.getPositionIndex(pos_);
    if(!id)return;
    posInWs = pos_;
    //绑定actorComponent
    for (auto component : allActorComponents)
    {
        bindActorComponent(component);
    }
    //绑定碰撞
    if (collisionForMove)
        collisionForMove->setPosition(pos_);
    
    if(id!=mapNodeId){
        GActor::gridMapOfActor.changeActorNode(this,id,mapNodeId);
        //printf("changed,");
    }
}

void GActor::setRenderSprite( sf::Sprite *sprPt_) { sprPt = sprPt_; }

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
    (sprPt)->setPosition(psInWin.x, psInWin.y);
    getWindow()->draw(*sprPt);
    drawCallNum++;
}

void GActor::bindActorComponent(GActorComponent *ptr)
{
    ptr->setPosInWs(posInWs + ptr->getRelativePosition());
}

GActor::GActor()
{
    static bool init = []() {
        spr.setTexture(tex);
        return true;
    }();
    sprPt =&spr;
    //addActors(this);
    setRenderSprite(&spr);
    //////////////////////
    mapNodeId=gridMapOfActor.addActor(this);
    /////////////////////
}

GActor::~GActor()
{

    
    
}
