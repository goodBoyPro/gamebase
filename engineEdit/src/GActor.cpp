#include "GActor.h"

#include <GCollision.h>
#include <game.h>
 sf::Texture GActor::texDefault;
 sf::Sprite GActor::spriteDefault;
long GActor::drawCallNum=0;
GridMap<GActor *> GActor::gridMapOfActor={FVector2(-10.f,-10.f),500,500,5.f,5.f};
void GActor::eventTick()
{
    
    
}

void GActor::dataLoop() {
    for(auto task:allDelaytasks){
        (*task)();
    }
}

void GActor::destroyActor()
{
    cancelAllAsyncTask();
    isValid = 0; 
    
}

const FVector3 &GActor::getPosInWs()const{
    
    return posInWs;
}

void GActor::setPosInWs(const FVector3 pos_)
{
    //std::shared_lock lk(gridMapOfActor.sortMut);
    if(isAllAsyncTaskCanceled)
        return;
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
        mapNodeId = id;
       
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
        spriteDefault.setTexture(texDefault);
        return true;
    }();
    sprPt =&spriteDefault;
    //addActors(this);
    setRenderSprite(&spriteDefault);
    //////////////////////
    mapNodeId=gridMapOfActor.addActor(this);
    /////////////////////
}

GActor::~GActor()
{
//    for (auto component : allComponents)
//     {
//         delete component;
//         component = nullptr;
//     }  
//     for (auto component : allActorComponents)
//     {
//         component->destroyActor();
//         component = nullptr;
//     }
//     for (auto task : allDelaytasks){
//         delete task;
//     }
        
}
