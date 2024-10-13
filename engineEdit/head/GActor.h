#ifndef GACTOR_H
#define GACTOR_H
#include "GObject.h"

class GActor : public GObject {
  private:
    /* data */
  public:
    GActor(/* args */);
    virtual ~GActor();
    virtual void drawLoop();
    virtual void dataLoop();

    FVector posInWs;
    sf::Texture tex;
    sf::Sprite spr;
    sf::Sprite *sprPt = nullptr;
    //引用计数
    std::atomic<int> count = 1;
    virtual void destroyActor();
    float z=0;
 
    
};
template <class T> T *createActor() { return new T; }

#endif