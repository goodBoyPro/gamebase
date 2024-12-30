#include <AnicomponentTest.h>
#include <GAnimationBp.h>
#include <GCollision.h>
#include <GController.h>
#include <GDebug.h>
#include <GPhysix.h>
#include <GPlayerChar.h>
#include <GSource.h>
#include <GTalk.h>
#include <GWidget.h>
#include <actorTest.h>
class Playertest : public GPlayerChar {
  private:
    /* data */
  public:
    virtual void eventBegin() override {}
    void func() {
        DELAY(4, [&]() {
            spawnActorAtLocation<actorTest>(getPosInWs());
            func();
        });
    }
    void SpawnAt() {
        DELAY(
            500,
            [&]() {
                actorTest *a = spawnActorAtLocation<actorTest>(getPosInWs());
                a->velocity = getVelocity() * 2.f;
            },
            true);
    };
    DelayTask taskSpawnAT = {500, [&]() {
                                 actorTest *a = spawnActorAtLocation<actorTest>(
                                     getPosInWs());
                                 a->velocity = getVelocity() * 2.f;
                             }};

    void eventTick() {

        PRINTDEBUG(L"玩家位置：%f,%f", getPosInWs().x, getPosInWs().y);
        PRINTDEBUG(L"场景对象数量：%d", GGameInterface::getGameIns()->getWorldActive()->spaceManager->getActorsNumber());
       
        PRINTDEBUG(L"玩家位置：%f,%f", getPosInWs().x, getPosInWs().y);
        PRINTDEBUG(L"玩家节点：%d,%d", mapNodeId,
                   GGameInterface::getGameIns()->getWorldActive()->spaceManager->getPositionIndex(getPosInWs()));
        PRINTDEBUG(L"像素尺寸：%f", pixSize);
    }
    GComponentAnimation *aniCom;
    Playertest() {
        createActorComponent<actorComponentTest>(new actorComponentTest);
        GTalk *gt = createActorComponent<GTalk>(new GTalk);
        gt->setRelativePosition({0, -0.001, 1});
        gt->setSizeInWs({2, 1, 0});
        aniCom = createAnimationComponent<anicomponenttest>();
        aniCom->setSizeInWs({1, 1, 0});
        gph.gravity = -30;
        controller->bindKey[GController::space] = [&]() {
            if (!gph.speedZ)
                gph.speedZ = 6;
        };

        controller->bindKey[GController::uiOpenOrClose] = [&]() {
            widget.addToViewPort();
             GControllerInterface::getPlayerController()->setFoucusOnWidget(true);
        };
        controller->bindKey[GController::b] = [&]() {
            spawnActorAtLocation<actorTest>(getPosInWs());
        };

        GCollision *collisiontemp = createComponent<GCollision>(new GCollision);
        collisiontemp->setRadius(0.2);
        collisiontemp->setScale(1, 0.5);
        setMoveCollision(collisiontemp);
    };

    GPhysix gph;
    GWidget widget = {{WINW / 2, WINH / 2}, 800, 600};
    DelayTask task = {100, []() { printf("task"); }};
    virtual void dataLoop() {
        GPlayerChar::dataLoop();
        static canRun vTest;
        if (vTest.delay(25)) {
        }
        // gph.drop(getPosInWs().z);
    }
    virtual ~Playertest() {};

    void updateState() {
        GAnimationBp *aniBpPtr =
            (GAnimationBp *)(getAnimationComponent()->getAnimationBp());
        FVector3 v = getVelocity();
        if (v.x > 0) {
            aniBpPtr->orientation = RIGHT_ORTN;
            if (aniCom->getSizeInWs().x > 0)
                aniCom->setSizeInWs({aniCom->getSizeInWs().x * (-1),
                                     aniCom->getSizeInWs().y,
                                     aniCom->getSizeInWs().z});
        } else if (v.x < 0) {
            aniBpPtr->orientation = LEFT_ORTN;
            if (aniCom->getSizeInWs().x < 0)
                aniCom->setSizeInWs({aniCom->getSizeInWs().x * (-1),
                                     aniCom->getSizeInWs().y,
                                     aniCom->getSizeInWs().z});
        }

        if (getPosInWs().z > 0.001) {
            aniBpPtr->state = GAnimationBp::jump;
        } else {
            if (getSpeed() > 0.001)
                aniBpPtr->state = GAnimationBp::walk;
            else
                aniBpPtr->state = GAnimationBp::idle;
        }
    }
};
