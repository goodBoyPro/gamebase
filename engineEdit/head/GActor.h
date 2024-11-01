#ifndef GACTOR_H
#define GACTOR_H
#include "GObject.h"
#include <GCollision.h>
#include<game.h>
class GActorComponent;
class GActor : public GObject {  
  protected:
  private:
    FVector posInWs;
    sf::Sprite *sprPt = nullptr;
    static sf::Texture tex;
    static sf::Sprite spr;
    static void initGactor() {}
   
    // 接口
  public:
    static long drawCallNum;
    virtual void gameBegin() = 0;
    virtual void eventTick()=0;
    //
    void drawActor();
  public:
    GActor(/* args */);
    virtual ~GActor();
    
    virtual void dataLoop();

    // sf::Texture tex;
    // sf::Sprite spr;

    // 引用计数
    std::atomic<int> count = 1;
    virtual void destroyActor();
    float z = 0;

  public:
    FVector &getPosInWs();
    void setPosInWs(const FVector &pos_);
    void setRenderSprite(sf::Sprite *sprPt_);
    sf::Sprite *getRenderSprite();
    virtual bool addWsPosOffset(const FVector &vec);
    

    // 碰撞
    class CollisionInterface *collisionForMove = nullptr;
    void setMoveCollision(class CollisionInterface *ptr) {
        collisionForMove = ptr;
    }

    // 使用方法：createCollision(new T);创建后无需手动释放
    template <class T> T *createComponent(GComponent *ptr) {
        allComponents.push_back(ptr);
        return (T *)ptr;
    }
    template <class T> T *createActorComponent(GActorComponent *ptr) {
        allActorComponents.push_back(ptr);
       bindActorComponent(ptr);
        return (T *)ptr;
    }
    private:
    void bindActorComponent(GActorComponent*ptr);

  private:
    char xxx[16];
    std::vector<GComponent *> allComponents;
    std::vector<GActorComponent *> allActorComponents;
};
template <class T> T *createActor() { return new T; }

/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
class GActorComponent : public GActor {
  private:
    sf::Sprite *sprPt1;
    FVector relativePosition;
    FVector relativeRotation;
    void drawActor1() {
        IVector psInWin = wsToWin(getPosInWs());
        sprPt1->setPosition(psInWin.x, psInWin.y - z / pixSize);
        getWindow()->draw(*sprPt1);
    }
    public:
    FVector&getRelativePosition(){return relativePosition;}
    void setRelativePosition(const FVector&pos){relativePosition=pos;}
    
};

#endif