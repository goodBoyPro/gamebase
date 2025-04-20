#include "GActor.h"
#include <GCollision.h>
#include <GSource.h>
#include <game.h>
sf::Texture GActor::texDefault;
sf::Sprite GActor::spriteDefault;
long GActor::drawCallNum = 0;
void GActor::eventTick() {}

void GActor::dataLoop() {
    for (auto task : allDelaytasks) {
        (*task)();
    }
}

void GActor::destroyActor() {
    cancelAllAsyncTask();
    isValid = 0;
}

void GActor::setSprite(int fileId_, int picIndex_) {
    getSource().setSprite(*(getRenderSprite()), fileId_, picIndex_);
}

const FVector3 &GActor::getPosInWs() const { return posInWs; }

void GActor::setPosInWs(const FVector3 pos_) {
    // std::shared_lock lk(gridMapOfActor.sortMut);
    if (isAllAsyncTaskCanceled)
        return;
    int id = GGameInterface::getGameIns()
                 ->getWorldActive()
                 ->spaceManager->getPositionIndex(pos_);
    if (!id)
        return;

    posInWs = pos_;
    // 绑定actorComponent
    for (auto component : allActorComponents) {
        bindActorComponent(component);
    }
    // 绑定碰撞
    if (collisionForMove)
        collisionForMove->setPosition(pos_);

        if (id != mapNodeId) {
            GGameInterface::getGameIns()
                ->getWorldActive()
                ->spaceManager->changeActorNode(this, id, mapNodeId);
            mapNodeId = id;
        }
}

void GActor::setRenderSprite(sf::Sprite *sprPt_) { sprPt = sprPt_; }

sf::Sprite *GActor::getRenderSprite() { return sprPt; }

bool GActor::addWsPosOffset(const FVector3 &vec) {
    if (collisionForMove && collisionForMove->isCollisionOn()) {
        FVector3 testPos = getPosInWs() + vec;
        collisionForMove->setPosition(testPos);
        if (collisionForMove->isStock()) {
            collisionForMove->setPosition(getPosInWs());
            return 0;
        } else {
            setPosInWs(testPos);
            return 1;
        }
    } else
        setPosInWs(getPosInWs() + vec);
    return 1;
}

void GActor::drawActor(sf::RenderWindow &window_, const FVector3 &cameraPos_) {

    FVector2 psInWin = wsToWin(posInWs, cameraPos_);
    (sprPt)->setPosition(psInWin.x, psInWin.y);
    float xScale = sizeInWs.x / pixSize / sprPt->getLocalBounds().getSize().x;
    float yScale = sizeInWs.y / pixSize / sprPt->getLocalBounds().getSize().y;
    sprPt->setScale(xScale, yScale);
    window_.draw(*sprPt);
    drawCallNum++;
}
void GActor::drawActor(RenderTex &layer, const FVector3 &cameraPos_) {

    FVector2 psInWin = wsToWin(posInWs, cameraPos_);
    (sprPt)->setPosition(psInWin.x, psInWin.y);
    float xScale = sizeInWs.x / pixSize / sprPt->getLocalBounds().getSize().x;
    float yScale = sizeInWs.y / pixSize / sprPt->getLocalBounds().getSize().y;
    sprPt->setScale(xScale, yScale);
    layer.draw(*sprPt);
    drawCallNum++;
}

void GActor::bindActorComponent(GActorComponent *ptr) {
    ptr->setPosInWs(posInWs + ptr->getRelativePosition());
}

GActor::GActor() {
    static bool init = []() {
        spriteDefault.setTexture(texDefault);
        return true;
    }();
    setRenderSprite(&__spr____);
    //////////////////////
    mapNodeId =
        GGameInterface::getGameIns()->getWorldActive()->spaceManager->addActor(
            this);
    /////////////////////
}

GActor::~GActor() {
    for (auto component : allComponents) {
        delete component;
        component = nullptr;
    }
    if (GGameInterface::getGameIns()
            ->getWorldActive()
            ->spaceManager->noteToActor)
        for (auto component : allActorComponents) {
            component->destroyActor();
            component = nullptr;
        }
    for (auto task : allDelaytasks) {
        delete task;
    }
}
