#ifndef GPLAYERCHAR_H
#define GPLAYERCHAR_H
#include "GActor.h"
#define MOVETICK 10
class GPlayerChar : public GActor {
  private:
    /* data */
  public:
    GPlayerChar(/* args */);
    ~GPlayerChar();
    class GController *controller = nullptr;
    class GCamera *camera = nullptr;
    FVector getVelocity();
    void move(FVector _fvector, float _value);
    void moveUp(float value);
    void moveRight(float value);
    FVector posMouseClicked;
    void setPosMouse(float value);
    bool isMouseMove = 0;
    void moveToMouse();
    void moveByKey();
    virtual void dataLoop() override;
    virtual void drawLoop() override;
    FVector previousPosition;
    float cameraDistance=pixSize*100;
    float speed = 1;
    virtual void cameraFollowPlayer();
    FVector moveVector={0,0};
    FVector moveUpVec={0,0};
    FVector moveRightVec={0,0};
    FVector posPrevious;

  private:
};

#endif