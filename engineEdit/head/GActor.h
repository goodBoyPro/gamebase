#ifndef GACTOR_H
#define GACTOR_H
#include "GObject.h"
#include <GCollision.h>
#include <game.h>
class GActorComponent;
class GActor : public GObject {
  protected:
  private:
    char memory[24];
    FVector3 posInWs;
    sf::Sprite *sprPt = nullptr;
    static sf::Texture tex;
    static sf::Sprite spr;
    static void initGactor() {}

    // 接口
  public:
    static long drawCallNum;
    virtual void gameBegin() = 0;
    virtual void eventTick() = 0;
    //
    void drawActor();

  public:
    GActor(/* args */);
    virtual ~GActor();

    virtual void dataLoop();

   

    // 引用计数
    std::atomic<int> count = 1;
    virtual void destroyActor();
    float z = 0;

  public:
    FVector3 &getPosInWs();
    void setPosInWs(const FVector3 &pos_);
    void setRenderSprite(sf::Sprite *sprPt_);
    sf::Sprite *getRenderSprite();
    virtual bool addWsPosOffset(const FVector3 &vec);

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
    void bindActorComponent(GActorComponent *ptr);

  private:
    
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
    FVector3 relativePosition;
    FVector3 relativeRotation;  

  public:
    virtual ~GActorComponent(){};
    FVector3 &getRelativePosition() { return relativePosition; }
    void setRelativePosition(const FVector3 &pos) { relativePosition = pos; }
};

#endif