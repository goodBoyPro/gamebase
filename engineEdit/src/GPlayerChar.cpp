#include "GPlayerChar.h"
#include <GCamera.h>
#include <GController.h>
GPlayerChar::GPlayerChar()
{
    controller = new GController;
    setPlayerController(controller);
    
    controller->bindKey[GController::w] = std::bind(&GPlayerChar::moveUp, this, -1);
    controller->bindKey[GController::s] = [&]()
    { moveUp(1); };
    controller->bindKey[GController::d] = [this]()
    { moveRight(1); };
    controller->bindKey[GController::a]= [this]()
    { moveRight(-1); };
    controller->bindKey[GController::mouseLeft] = [this]()
    { setPosMouse(1); };
    controller->AXISX =
        std::bind(&GPlayerChar::moveRight, this, std::placeholders::_1);
    controller->AXISY =
        std::bind(&GPlayerChar::moveUp, this, std::placeholders::_1);
    // 当键盘无操作，鼠标也无操作时才能执行
    controller->KEYIDLE = [this]()
    {
        if (!isMouseMove)
            move({0, 0}, 0);
    };
    camera = new GCamera;
    // 写在子类中
    //  tex.loadFromFile("bin/res/a.png");
    //  spr.setTexture(tex);
    //  spr.setOrigin(tex.getSize().x/2,tex.getSize().y);
    //  spr.setScale(4,4);
    setGameCamera(camera);
   
    camera->posInWs.x = posInWs.x;
        camera->posInWs.y = posInWs.y;
}

GPlayerChar::~GPlayerChar()
{
    delete controller;
    delete camera;
}

FVector GPlayerChar::getVelocity() { return posInWs - previousPosition; }

void GPlayerChar::move(FVector _fvector, float _value)
{
    static canRun t1;
    if (t1.delay(MOVETICK))
    {
        previousPosition = posInWs;
        posInWs += (_fvector * (_value * MOVETICK / 1000));
        moveUpVec = {0, 0};
        moveRightVec = {0, 0};
        
    }
}

void GPlayerChar::moveUp(float value)
{    
    moveUpVec = {0, value};isMouseMove=0;
}

void GPlayerChar::moveRight(float value)
{
    moveRightVec = {value, 0};
    isMouseMove=0;
}

void GPlayerChar::setPosMouse(float value)
{
    isMouseMove = 1;
    posMouseClicked = winToWs(sf::Mouse::getPosition(*getWindow()));
}

void GPlayerChar::moveToMouse()
{

    if (isMouseMove)
    {

        static float precision = 0.1;

        FVector a = normalize(posMouseClicked - posInWs);
        if (fabs(posInWs.x - posMouseClicked.x) > precision ||
            fabs(posInWs.y - posMouseClicked.y) > precision)
        {
            move(a, speed);
        }
        else
            isMouseMove = 0;
    }
}

void GPlayerChar::moveByKey()
{
    move(normalize(moveUpVec + moveRightVec), speed);

    isMouseMove = 0;
}

void GPlayerChar::dataLoop()
{
    if (isMouseMove)
        moveToMouse();
    else
        moveByKey();
       
}

void GPlayerChar::drawLoop()
{
  
    IVector psInWin = wsToWin(posInWs);
    sprPt->setPosition(psInWin.x, psInWin.y);
    getWindow()->draw(*sprPt);
}

//场景剔除可能会造成loop不调用，因此在主循环中调用
void GPlayerChar::cameraFollowPlayer()
{
    camera->posInWs.x = posInWs.x;
    camera->posInWs.y = posInWs.y;
}
