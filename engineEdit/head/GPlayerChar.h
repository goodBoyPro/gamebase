#ifndef GPLAYERCHAR_H
#define GPLAYERCHAR_H
#include <GActorWithAnim.h>
#include <GCamera.h>
#define MOVETICK 10
class GPlayerChar : public GActorWithAnim
{
private:
  bool isMovingW = 0;
  bool isMovingA = 0;
  bool isMovingS = 0;
  bool isMovingD = 0;
  FVector posPrevious; // 用於判定角色朝向
  bool isMouseMove = 0;
  float cameraDistance = pixSize * 100;
  FVector moveVector = {0, 0};
  FVector moveUpVec = {0, 0};
  FVector moveRightVec = {0, 0};
  float speed = 0;


public:
  GPlayerChar(/* args */);
  virtual ~GPlayerChar();
  class GController *controller = nullptr;

  GCamera camera;
  FVector getVelocity();

  virtual void updateState() = 0;
  void move(FVector _fvector, float _value);
  void moveUp(float value);
  void moveRight(float value);
  FVector posMouseClicked;
  void setPosMouse(float value);

  void moveToMouse();
  void moveByKey();
  virtual void dataLoop() override;
  // 子类重载中应适时调用
  // virtual void drawLoop() override;
  float getSpeed() { return speed; };
  void setSpeed(float value) { speed = value; };
  
 

  virtual void cameraFollowPlayer();

private:
};

#endif