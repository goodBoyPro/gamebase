#include <AnicomponentTest.h>
#include <GAnimationBp.h>
#include <GCamera.h>
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
    char x[40];
    virtual void drawActor() override {
        // 阴影
        GSource::getSource().getSprite(10).setScale(0.3, 0.2);
        const FVector3 &posShadow = {getPosInWs().x, getPosInWs().y, 0.f};
        IVector posTemp = wsToWin({posShadow.x, posShadow.y, 0});
        GSource::getSource().getSprite(10).setPosition(posTemp.x, posTemp.y);
        getWindow()->draw(getSource().getSprite(10));
    }

    void eventBegin() {  }
    void SpawnAt() {
        delay(500, [&]() {
            actorTest *a = spawnActorAtLocation<actorTest>(getPosInWs());
            
            a->velocity = getVelocity()*2.f;
            SpawnAt();
        });
    };
    DelayTask taskSpawnAT = {8000, []() { spawnActorAtLocation<actorTest>(); }};

    void eventTick() {
        PRINTDEBUG( L"玩家位置：%f,%f", getPosInWs().x, getPosInWs().y);
        PRINTDEBUG( L"场景对象数量：%d", actors.size());
        PRINTDEBUG( L"键鼠位置：%f,%f",
                  winToWs(sf::Mouse::getPosition(*getWindow())).x,
                  winToWs(sf::Mouse::getPosition(*getWindow())).y);
        PRINTDEBUG(L"玩家位置：%f,%f", getPosInWs().x, getPosInWs().y);   
        PRINTDEBUG(L"玩家节点：%d,%d",mapNodeId,gridMapOfActor.getPositionIndex(getPosInWs()));  
       
      
       
       
    }
    Playertest() {
        //createActorComponent<actorComponentTest>(new actorComponentTest);
        GTalk *gt = createActorComponent<GTalk>(new GTalk);
        GComponentAnimation *aniCom = setAnimationComponent<anicomponenttest>();
        gt->playerSpr = aniCom->getRenderSprite();
        gph.gravity = -30;
        getPlayerController()->bindKey[GController::space] = [&]() {
            if (!gph.speedZ)
                gph.speedZ = 6;
        };

        getPlayerController()->bindKey[GController::uiOpenOrClose] = [&]() {
            widget.addToViewport();
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
        cameraFollowPlayer();
        GPlayerChar::dataLoop();
        updateState();
        static canRun vTest;
        if (vTest.delay(25)) {
        }
        gph.drop(getPosInWs().z);

        // 由于每隔4ms执行一次移动，在此期间，角色状态会被意外的切换
        // 解决方法：所有移动逻辑使用指定的移动方法；
    }
    virtual ~Playertest() {};

    void updateState() {
        GAnimationBp *aniBpPtr =
            (GAnimationBp *)(getAnimationComponent()->getAnimationBp());
        FVector3 v = getVelocity();
        if (v.x > 0)
            aniBpPtr->orientation = RIGHT_ORTN;
        else if (v.x < 0)
            aniBpPtr->orientation = LEFT_ORTN;

        if (getPosInWs().z > 0.001) {
            aniBpPtr->state = GAnimationBp::jump;
        } else {
            if (getSpeed() > 0.001)
                aniBpPtr->state = GAnimationBp::walk;
            else
                aniBpPtr->state = GAnimationBp::idle;
        }
    }

    virtual void cameraFollowPlayer() override {

        // camera->posInWs.x = nsg::smoothInterpolateTo(
        //     camera->posInWs.x, posInWs.x, 0.001f, deltaTime);
        // camera->posInWs.y = nsg::smoothInterpolateTo(
        //     camera->posInWs.y, posInWs.y, 0.001f, deltaTime);
        camera.posInWs = getPosInWs();
    };
};