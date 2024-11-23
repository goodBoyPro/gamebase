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
    xlib::TimerManager::DelayTask *ptr = nullptr;
    void eventBegin() {
        
        DELAY(
            5,
            [&]() {
                setPosInWs(getPosInWs() + FVector3(0.01, 0.01, 0));
            },
            true);
        DELAY(5000, [&]() { destroyActor(); });
        //createDelayTask(6,[&]() { setPosInWs(getPosInWs() + FVector3(0.01, 0.01, 0)); });
    }
    sf::Sprite spr;
    void delayTest() {}

    void eventTick() {
        
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
    };
    virtual ~actorTest() { std::unique_lock lk(mtx); }

    FVector3 velocity;
    
    xlib::TimerManager::DelayTask *delayTaskPtr = nullptr;
    static xlib::TimerManager::DelayTask dtp;
    std::mutex mutAsuncTask;

    bool flag = 0;
};
class actorComponentTest : public GActorComponent {
  public:
    void eventTick() {}
    void eventBegin() {
        // createDelayTask(10, [&]() {
        //     static float a;
        //     FVector3 pos = {sinf(a / 30) / 4, cosf(a / 30) / 4, 0.7};
        //     a++;

        //     setRelativePosition(pos);
        // });
        funcR();
    }
    void funcR() {
        static float a;
        DELAY(1, [&]() {
            FVector3 pos = {sinf(a / 30) / 4, cosf(a / 30) / 4, 0.7};
            a++;
            setRelativePosition(pos);
            
        },true);
    }
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
