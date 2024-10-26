#ifndef GPLAYERCHAR_H
#define GPLAYERCHAR_H
#include "GActor.h"
#include<GCamera.h>
#define MOVETICK 10
class GPlayerChar : public GActor {
  private:
    bool isMovingW = 0;
    bool isMovingA = 0;
    bool isMovingS = 0;
    bool isMovingD = 0;
    FVector posPrevious;//用於判定角色朝向

  public:
    GPlayerChar(/* args */);
    virtual~GPlayerChar();
    class GController *controller = nullptr;
    GCamera *camera;
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
    float cameraDistance=pixSize*100;

    
    float speed = 0;
    virtual void cameraFollowPlayer();
    FVector moveVector={0,0};
    FVector moveUpVec={0,0};
    FVector moveRightVec={0,0};
    

  private:
};

#endif