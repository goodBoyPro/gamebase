#if !defined(ACTORTEST)
#define ACTORTEST

#include "GActor.h"
#include <GCollision.h>
#include <GPlayerChar.h>
#include <random>
class actorTest : public GActor {
  private:
  public:
    struct GSource {
        sf::Texture tex1;
        sf::Texture tex2;
        sf::Texture tex3;
        sf::Texture tex4;
        sf::Texture tex5;
        GSource() { tex1.loadFromFile("res/a.png"); }
        static GSource gs;
    };

    /* data */
  public:
    std::mutex mtx;
    xlib::Timer20240522::DelayTask *ptr = nullptr;
    void eventBegin() {
        setPosInWs(getPlayerCharactor()->getPosInWs());
        delayTest();
         delay(3000,[&](){if(ptr)ptr->isTaskValid=0;destroyActor();});
    }
    sf::Sprite spr;
    void delayTest() {
        if(!isValid)
            return;
        ptr = delay(15, [&]() {
             std::unique_lock lk(mtx);
            setPosInWs(getPosInWs() + FVector3(0.01, 0.01, 0));
            if(!isValid)
                return;
            delayTest();
        });
    }

    void eventTick() {
        // setPosInWs(getPosInWs()+velocity);
    }
    actorTest(/* args */) {
        delayTaskPtr = &dtp;
        setRenderSprite(&spr);
        spr.setTexture(GSource::gs.tex1);
        spr.setScale(0.7, 0.7);
        spr.setOrigin(24, 48);
        setPosInWs({0, 0, 0});
        static float x = 0;
        x += 1.f;
        velocity = {cosf(x) * 4.f, sinf(x) * 4.f, 0};

        //   GCollision *collision = createComponent<GCollision>(new
        //   GCollision); collision->setRadius(0.5);
        //   setMoveCollision(collision);
    };
    virtual ~actorTest() {
        std::unique_lock lk(mtx);
    }
    int isAsyncCanceled = 0;
    int isDead = 0;
    FVector3 velocity;
    canRun t;
    xlib::Timer20240522::DelayTask *delayTaskPtr = nullptr;
    static xlib::Timer20240522::DelayTask dtp;
    std::mutex mutAsuncTask;

    bool flag = 0;
    // 所有事件写在此处
    void createLoop() {

        delayTaskPtr = delay(1, [this]() {
            {

                setPosInWs(getPosInWs() + velocity);
                setPosInWs(getPosInWs() + velocity);
                createLoop();
            }
        });
    };

    void stopAllAsyncTask() {
        std::unique_lock lk(mutAsuncTask);
        isAsyncCanceled = 1;
        delayTaskPtr->isTaskValid = 0;
    }
    void destroy() {

        stopAllAsyncTask();

        delay(0, [this]() {
            std::unique_lock lk(mutAsuncTask);
            // destroyActor();
        });
    }

    virtual void dataLoop() {}
};
class actorComponentTest : public GActorComponent {
  public:
    void eventTick() {
        static float a;
        FVector3 pos = {sinf(a / 30) / 4, cosf(a / 30) / 4, 0.7};
        a++;
        setRelativePosition(pos);
    }
    void eventBegin() {}
    actorComponentTest() {
        tex.loadFromFile("res/a.png");
        spr.setTexture(tex);
        setRenderSprite(&spr);
        spr.setOrigin(tex.getSize().x / 2, tex.getSize().x / 2);
        spr.setScale(0.7, 0.7);
    }
    sf::Sprite spr;
    sf::Texture tex;
};

#endif // ACTORTEST
