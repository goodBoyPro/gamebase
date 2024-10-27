#ifndef GACTOR_H
#define GACTOR_H
#include "GObject.h"
#include<GCollision.h>
class GActor : public GObject {
  private:
    FVector posInWs;
    sf::Sprite *sprPt = nullptr;
   

  public:
    GActor(/* args */);
    virtual ~GActor();
    virtual void drawLoop();
    virtual void dataLoop();

    sf::Texture tex;
    sf::Sprite spr;

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
    class CollisionInterface *collision = nullptr;
};
template <class T> T *createActor() { return new T; }

#endif