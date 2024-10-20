#include<GDebug.h>
#include <GCamera.h>
#include <GAnimationBp.h>
#include <GPhysix.h>
#include <GPlayerChar.h>
#include <GSource.h>
#include <GTalk.h>
#include <GWidget.h>
#include<GController.h>
class Playertest : public GPlayerChar {
  private:
    /* data */
  public:
    Playertest() {
        PRINTF("test");
        sprPt = aniBp.sprPt;
        sprPt->setScale(2, 2);

        gph.gravity = -30;
        getPlayerController()->KEYJUMP = [&]() {
            if (!gph.speedZ)
                gph.speedZ = 6;
        };

        getPlayerController()->bindKey[GController::uiOpenOrClose] = [&]() {
            widget.addToViewport();
        };
        // getSource().sounds[1].sound.play();
    };

    GAnimationBp aniBp;

    GPhysix gph;
    GTalk gtalk;
    GWidget widget = {{WINW / 2, WINH / 2}, 800, 600};
    DelayTask task = {100, []() { printf("task"); }};
    virtual void dataLoop() {
        cameraFollowPlayer();
        GPlayerChar::dataLoop();

        static canRun vTest;
        if (vTest.delay(25)) {
        }
        gph.drop(z);

        // 由于每隔4ms执行一次移动，在此期间，角色状态会被意外的切换
        // 解决方法：所有移动逻辑使用指定的移动方法；
    }
    ~Playertest() = default;
    sf::Shader sh;
    void updateState() {
        FVector v = getVelocity();
        if (v.x > 0)
            aniBp.orientation = RIGHT_ORTN;
        else if (v.x < 0)
            aniBp.orientation = LEFT_ORTN;

        if (z > 0.001) {
            aniBp.state = GAnimationBp::jump;
        } else {
            if (getLength(v) > 0.0001)
                aniBp.state = GAnimationBp::walk;
            else
                aniBp.state = GAnimationBp::idle;
        }
    }
    IVector psInWin;
    virtual void drawLoop() override {
        psInWin = wsToWin(posInWs);
       
        // 阴影
        GSource::getSource().texSpr[10].spr.setScale(0.3, 0.2);
        GSource::getSource().texSpr[10].spr.setPosition(psInWin.x,
                                                        psInWin.y - 5);
        getWindow()->draw(getSource().texSpr[10].spr);

        // 角色
        updateState();
        sprPt->setPosition(psInWin.x, psInWin.y - z / pixSize);
        aniBp.updateAnim();

        const wchar_t *ch = L"hello World\n你好！\n呵呵！";
        gtalk.draw({sprPt->getPosition().x,
                    sprPt->getPosition().y - sprPt->getGlobalBounds().height},
                   ch);
        static GDebug db;
        swprintf(db.wchar_, L"玩家位置：%f,%f", posInWs.x, posInWs.y);
        static GDebug gddd;
        swprintf(gddd.wchar_, L"场景对象数量：%d", actors.size());
        static GDebug gd2;
        swprintf(gd2.wchar_, L"键鼠位置：%f,%f",
                 winToWs(sf::Mouse::getPosition(*getWindow())).x,
                 winToWs(sf::Mouse::getPosition(*getWindow())).y);
    };
    virtual void cameraFollowPlayer() override {
        
        // camera->posInWs.x = nsg::smoothInterpolateTo(
        //     camera->posInWs.x, posInWs.x, 0.001f, deltaTime);
        // camera->posInWs.y = nsg::smoothInterpolateTo(
        //     camera->posInWs.y, posInWs.y, 0.001f, deltaTime);
        camera->posInWs.x = posInWs.x;
        camera->posInWs.y = posInWs.y;
    };
};
