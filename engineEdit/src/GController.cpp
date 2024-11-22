#include "GController.h"

#include <game.h>



void GController::pollKey() {

    // 无延迟按键
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        getPlayerController()->bindKey[GController::w]();
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        getPlayerController()->bindKey[GController::a]();
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        getPlayerController()->bindKey[GController::s]();
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        getPlayerController()->bindKey[GController::d]();
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
        getPlayerController()->bindKey[GController::space]();
    // if (!sf::Keyboard::isKeyPressed(sf::Keyboard::W) &&
    //     !sf::Keyboard::isKeyPressed(sf::Keyboard::A) &&
    //     !sf::Keyboard::isKeyPressed(sf::Keyboard::S) &&
    //     !sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    //     playerController->KEYIDLE();
     if (sf::Keyboard::isKeyPressed(sf::Keyboard::B))
        getPlayerController()->bindKey[GController::b]();

    if (getWindow()->pollEvent(getGameIns()->event)) {
        if (getGameIns()->event.type == sf::Event::Closed) {
            getWindow()->close();
            getGameIns()->bGameContinue = 0;
        }
        if (getGameIns()->event.type == sf::Event::Resized)
            resizeWindow(getWindow());

        // 键盘按键
        if (getGameIns()->event.type == sf::Event::KeyPressed) {
            switch (getGameIns()->event.key.code) {
            case sf::Keyboard::Grave:
                openConsoleWindow();
                break;
            case sf::Keyboard::Space:

                break;
            case sf::Keyboard::BackSpace:
                bindKey[GController::uiOpenOrClose]();
                break;
            default:
                break;
            }
        }
        if (getGameIns()->event.type == sf::Event::KeyReleased) {
            switch (getGameIns()->event.key.code) {

            case sf::Keyboard::A:
                bindKey[GController::ar]();
                break;
            case sf::Keyboard::D:
                bindKey[GController::dr]();
                break;
            case sf::Keyboard::W:
                bindKey[GController::wr]();
                break;
            case sf::Keyboard::S:
                bindKey[GController::sr]();
                break;
            default:
                break;
            }
        }
        // 鼠标
        if (getGameIns()->event.type == sf::Event::MouseButtonPressed) {
            switch (getGameIns()->event.mouseButton.button) {
            case sf::Mouse::Left:
                getPlayerController()->bindKey[GController::mouseLeft]();

                break;
            case sf::Mouse::Right:
                getPlayerController()->KEYRIGHT();
                break;

            default:
                break;
            }
        }
        // 手柄
        // if(event.type==sf::Event::JoystickMoved){
        //     switch (event.joystickMove.axis)
        //     {
        //     case sf::Joystick::X:
        //         playerController->AXISX(sf::Joystick::getAxisPosition(0,sf::Joystick::Axis::X)/10);
        //     case sf::Joystick::Y:
        //         playerController->AXISY(sf::Joystick::getAxisPosition(0,sf::Joystick::Axis::Y)/10);
        //         break;

        //     default:
        //         break;
        //     }
        // }
        if (sf::Joystick::hasAxis(0, sf::Joystick::Axis::X)) {
            getPlayerController()->AXISX(
                sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::X) / 50);
        }
        if (sf::Joystick::hasAxis(0, sf::Joystick::Axis::Y)) {
            getPlayerController()->AXISY(
                sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::Y) / 50);
        }
    }
}
