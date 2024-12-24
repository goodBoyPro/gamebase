#include <GAnimationBp.h>

#include <GController.h>
#include <GPlayerChar.h>
GPlayerChar::GPlayerChar() {
    controller = new GController;
    setPlayerController(controller);

    controller->bindKey[GController::w] = [&]() {
        isMovingW = 1;
        moveUp(-1);
    };
    controller->bindKey[GController::s] = [&]() {
        isMovingS = 1;
        moveUp(1);
    };
    controller->bindKey[GController::d] = [this]() {
        isMovingD = 1;
        moveRight(1);
    };
    controller->bindKey[GController::a] = [this]() {
        isMovingA = 1;
        moveRight(-1);
    };
    // 松開按鍵
    controller->bindKey[GController::wr] = [&]() { isMovingW = 0; };
    controller->bindKey[GController::sr] = [&]() { isMovingS = 0; };
    controller->bindKey[GController::dr] = [this]() { isMovingD = 0; };
    controller->bindKey[GController::ar] = [this]() { isMovingA = 0; };
    //
    controller->bindKey[GController::mouseLeft] = [this]() { setPosMouse(*(Game::gameIns->gameWindow),1); };
    controller->AXISX =
        std::bind(&GPlayerChar::moveRight, this, std::placeholders::_1);
    controller->AXISY =
        std::bind(&GPlayerChar::moveUp, this, std::placeholders::_1);
    // 当键盘无操作，鼠标也无操作时才能执行
    controller->KEYIDLE = [this]() {
        if (!isMouseMove)
            move({0, 0,0}, 0);
    };
    GComponentCamera*cameraComPtr=createActorComponent(new GComponentCamera);
    setGameCamera(&(cameraComPtr->camera));
    
}

GPlayerChar::~GPlayerChar() { delete controller; }

FVector3 GPlayerChar::getVelocity() { return getPosInWs() - posPrevious; }


void GPlayerChar::move(FVector3 _fvector, float _value) {
    static canRun t1;
    if (t1.delay(MOVETICK)) {
        posPrevious = getPosInWs();
        bool flag = addWsPosOffset(_fvector * (_value * MOVETICK / 1000));
        
        if (!flag)
            isMouseMove = 0;
        moveUpVec = {0, 0,0};
        moveRightVec = {0, 0,0};
    }
}

void GPlayerChar::moveUp(float value) {
    moveUpVec = {0, value,0};
    isMouseMove = 0;
}

void GPlayerChar::moveRight(float value) {
    moveRightVec = {value, 0,0};
    isMouseMove = 0;
}

void GPlayerChar::setPosMouse(sf::RenderWindow&window_,float value) {
    isMouseMove = 1;
    posMouseClicked = winToWs(sf::Mouse::getPosition(window_));
}

void GPlayerChar::moveToMouse() {

    if (isMouseMove) {

        static float precision = 0.1;

        FVector3 a = normalize(posMouseClicked - getPosInWs());
        if (fabs(getPosInWs().x - posMouseClicked.x) > precision ||
            fabs(getPosInWs().y - posMouseClicked.y) > precision) {
            move(a, speed);
        } else
            isMouseMove = 0;
    }
}

void GPlayerChar::moveByKey() {
    move(normalize(moveUpVec + moveRightVec), speed);

    isMouseMove = 0;
}

void GPlayerChar::dataLoop() {
    if (isMouseMove)
        moveToMouse();
    else
        moveByKey();
    if (isMovingA || isMovingD || isMovingS || isMovingW || isMouseMove)
        speed = 1;
    else
        speed = 0;
    //updateState();
}




