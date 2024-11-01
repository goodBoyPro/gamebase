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
class Playertest : public GPlayerChar
{
private:
    /* data */
public:
    void gameBegin() {}
    void eventTick() {
         IVector psInWin = wsToWin(getPosInWs());
        // 阴影
        GSource::getSource().getSprite(10).setScale(0.3, 0.2);
        GSource::getSource().getSprite(10).setPosition(psInWin.x, psInWin.y);
        getWindow()->draw(getSource().getSprite(10));
       

        drawAnimation();
        sf::Sprite &sprtest = getSource().getTexArray(1).getSprite(0);

        const wchar_t *ch = L"hello World\n你好！\n呵呵！";
        gtalk.draw({getRenderSprite()->getPosition().x,
                    getRenderSprite()->getPosition().y -
                        getRenderSprite()->getGlobalBounds().height},
                   ch);
        static GDebug db;
        swprintf(db.wchar_, L"玩家位置：%f,%f", getPosInWs().x, getPosInWs().y);
        static GDebug gddd;
        swprintf(gddd.wchar_, L"场景对象数量：%d", actors.size());
        static GDebug gd2;
        swprintf(gd2.wchar_, L"键鼠位置：%f,%f",
                 winToWs(sf::Mouse::getPosition(*getWindow())).x,
                 winToWs(sf::Mouse::getPosition(*getWindow())).y);
    }
    Playertest()
    {

        gph.gravity = -30;
        getPlayerController()->bindKey[GController::space] = [&]()
        {
            if (!gph.speedZ)
                gph.speedZ = 6;
        };

        getPlayerController()->bindKey[GController::uiOpenOrClose] = [&]()
        {
            widget.addToViewport();
        };

        // GCollision *collisiontemp=static_cast<GCollision *>(createComponent(new GCollision));
        GCollision *collisiontemp = createComponent<GCollision>(new GCollision);
        collisiontemp->setRadius(0.2);
        collisiontemp->setScale(1, 0.5);
        setMoveCollision(collisiontemp);
        setRenderAnimationBp(&aniBp);
        getRenderSprite()->setScale(2, 2);
    };

    GAnimationBp aniBp;

    GPhysix gph;
    GTalk gtalk;
    GWidget widget = {{WINW / 2, WINH / 2}, 800, 600};
    DelayTask task = {100, []()
                      { printf("task"); }};
    virtual void dataLoop()
    {
        cameraFollowPlayer();
        GPlayerChar::dataLoop();

        static canRun vTest;
        if (vTest.delay(25))
        {
        }
        gph.drop(z);

        // 由于每隔4ms执行一次移动，在此期间，角色状态会被意外的切换
        // 解决方法：所有移动逻辑使用指定的移动方法；
    }
    virtual ~Playertest() {};
    sf::Shader sh;
    void updateState()
    {
        FVector v = getVelocity();
        if (v.x > 0)
            aniBp.orientation = RIGHT_ORTN;
        else if (v.x < 0)
            aniBp.orientation = LEFT_ORTN;

        if (z > 0.001)
        {
            aniBp.state = GAnimationBp::jump;
        }
        else
        {
            if (getSpeed() > 0.001)
                aniBp.state = GAnimationBp::walk;
            else
                aniBp.state = GAnimationBp::idle;
        }
    }

   
    virtual void cameraFollowPlayer() override
    {

        // camera->posInWs.x = nsg::smoothInterpolateTo(
        //     camera->posInWs.x, posInWs.x, 0.001f, deltaTime);
        // camera->posInWs.y = nsg::smoothInterpolateTo(
        //     camera->posInWs.y, posInWs.y, 0.001f, deltaTime);
        camera.posInWs = getPosInWs();
    };
};
