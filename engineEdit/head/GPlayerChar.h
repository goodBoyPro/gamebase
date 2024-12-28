#ifndef GPLAYERCHAR_H
#define GPLAYERCHAR_H
#include <GActorWithAnim.h>
#include<component/GComponentCamera.h>
#include <component/GComponentAnimation.h>
#define MOVETICK 10
class GPlayerChar : public GActorWithAnim {
  private:
    bool isMovingW = 0;
    bool isMovingA = 0;
    bool isMovingS = 0;
    bool isMovingD = 0;
    FVector3 posPrevious; // 用於判定角色朝向
    bool isMouseMove = 0;
    float cameraDistance = pixSize * 100;
    FVector3 moveVector = {0, 0, 0};
    FVector3 moveUpVec = {0, 0, 0};
    FVector3 moveRightVec = {0, 0, 0};
    float speed = 0;
   

  public:
    GComponentCamera *cameraComPtr = nullptr;
    GPlayerChar(/* args */);
    virtual ~GPlayerChar();
    class GController *controller = nullptr;
    FVector3 getVelocity();

    virtual void updateState() = 0;
    void move(FVector3 _fvector, float _value);
    void moveUp(float value);
    void moveRight(float value);
    FVector3 posMouseClicked;
    void setPosMouse(sf::RenderWindow&window_,float value);

    void moveToMouse();
    void moveByKey();
    virtual void dataLoop() override;
    // 子类重载中应适时调用
    // virtual void drawLoop() override;
    float getSpeed() { return speed; };
    void setSpeed(float value) { speed = value; };



  private:
};

#endif