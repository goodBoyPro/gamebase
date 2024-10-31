#ifndef GACTOR_H
#define GACTOR_H
#include "GObject.h"
#include <GCollision.h>
class GActorComponent;
class GActor : public GObject
{
private:
  FVector posInWs;
  sf::Sprite *sprPt = nullptr;
  static sf::Texture tex;
  static sf::Sprite spr;
  static void initGactor(){}
  // 接口
public:
  virtual void gameBegin() = 0;
  virtual void eventTick() = 0;

public:
  GActor(/* args */);
  virtual ~GActor();
  virtual void drawLoop();
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
  void drawActor();

  // 碰撞
  class CollisionInterface *collisionForMove = nullptr;
  void setMoveCollision(class CollisionInterface *ptr) { collisionForMove = ptr; }

  // 使用方法：createCollision(new T);创建后无需手动释放
  template <class T>
  T *createComponent(GComponent *ptr)
  {
    allComponents.push_back(ptr);
    return (T *)ptr;
  }
  template <class T>
  T *createActorComponent(GActorComponent *ptr)
  {
    allActorComponents.push_back(ptr);
    return (T *)ptr;
  }

private:
  char xxx[16];
  std::vector<GComponent *> allComponents;
  std::vector<GActorComponent *> allActorComponents;
};
template <class T>
T *createActor() { return new T; }
class GActorComponent : public GActor
{
};

#endif