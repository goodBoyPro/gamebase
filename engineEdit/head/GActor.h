#ifndef GACTOR_H
#define GACTOR_H
#include "GObject.h"
#include <GCollision.h>
#include <game.h>
#include <gridmap.h>
#include <mutex>
class GActorComponent;
class GActor : public GObject {
  protected:
  private:
    sf::Sprite *sprPt = nullptr;
    FVector3 posInWs;
    static sf::Texture texDefault;
    static sf::Sprite spriteDefault;
    // 接口
  public:
    int mapNodeId;
    static long drawCallNum;
    static GridMap<GActor *> gridMapOfActor;
    virtual void eventBegin() = 0;
    virtual void eventTick() = 0;
    //
    virtual void drawActor();
    // 计时精度只有30ms
    template <class T> void createDelayTask(int delay, const T &callback) {
        return allDelaytasks.push_back(new DelayTask(delay, callback));
    };

  public:
    GActor(/* args */);
    virtual ~GActor();
    // 系统函数,所有子类重写时都应先运行
    virtual void dataLoop();

    // 引用计数
    std::atomic<int> count = 1;
    // 资源在此处统一释放，用户不要delete
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

    // 使用方法：createCollision(new T);创建后由系统管理，无需手动释放
    template <class T> T *createComponent(GComponent *ptr) {
        allComponents.push_back(ptr);
        return (T *)ptr;
    }
    template <class T> T *createActorComponent(GActorComponent *ptr) {
        allActorComponents.push_back(ptr);
        bindActorComponent(ptr);
        ptr->eventBegin();
        return (T *)ptr;
    }

  private:
    void bindActorComponent(GActorComponent *ptr);

  private:
    std::vector<DelayTask *> allDelaytasks;
    std::vector<GComponent *> allComponents;
    std::vector<GActorComponent *> allActorComponents;
};
template <class T> T *spawnActorAtLocation(FVector3 pos = {0, 0, 0}) {
    GActor *a = (GActor *)new T;
    a->setPosInWs(pos);
    a->eventBegin();
    return (T *)a;
}

/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
class GActorComponent : public GActor {
  private:
    sf::Sprite *sprPt1;
    FVector3 relativePosition = {0, 0, 0};
    FVector3 relativeRotation;

  public:
    virtual ~GActorComponent() {};
    FVector3 &getRelativePosition() { return relativePosition; }
    void setRelativePosition(const FVector3 &pos) { relativePosition = pos; }
};

#endif