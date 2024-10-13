#ifndef GWORLD_H
#define GWORLD_H

#include "GObject.h"

class GWorld : public GObject {
  private:
    /* data */
  public:
    GWorld(/* args */);
    virtual~GWorld();
    FVector posWs;
    sf::Sprite *sprPt;
    sf::Sprite spr;
    sf::Texture tex;
    virtual void dataLoop();
    virtual void drawLoop();
    
};
extern GWorld *worldCurrent;

#endif