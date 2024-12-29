#include "GController.h"
#include <game.h>

void GController::pollKey(sf::RenderWindow&window_,sf::Event&event_) {

    // 无延迟按键
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        bindKey[GController::w]();
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        bindKey[GController::a]();
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        bindKey[GController::s]();
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        bindKey[GController::d]();
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
        bindKey[GController::space]();
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::B))
        bindKey[GController::b]();

    if (window_.pollEvent(event_)) 
    {
        if (event_.type == sf::Event::Closed) {
            window_.close();
            GGameInterface::getGameIns()->bGameContinue = 0;
        }
        if (event_.type == sf::Event::Resized)
            resizeWindow(&window_);

        // 键盘按键
        if (event_.type == sf::Event::KeyPressed) {
            if(GWidgetInterface::getTop())
            GWidgetInterface::getTop()->onKeyPressed(event_.key.code);
            switch (GGameInterface::getGameIns()->event.key.code) {
            case sf::Keyboard::Grave:
                openConsoleWindow();
                break;
            case sf::Keyboard::Space:

                break;
            case sf::Keyboard::BackSpace:
                bindKey[GController::uiOpenOrClose]();
                break;
            case sf::Keyboard::RBracket:
                pixSize+=0.001;
                break;
             case sf::Keyboard::LBracket:
                pixSize-=0.001;
                break;
            default:
                break;
            }
        }
        if (event_.type == sf::Event::KeyReleased) {
            switch (event_.key.code) {

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
        if (event_.type == sf::Event::MouseButtonPressed) {
            if(GWidgetInterface::getTop())
            GWidgetInterface::getTop()->onMousePressed(event_.mouseButton.button);
            switch (event_.mouseButton.button) {
            case sf::Mouse::Left:
                bindKey[GController::mouseLeft]();

                break;
            case sf::Mouse::Right:
                KEYRIGHT();
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
            AXISX(sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::X) / 50);
        }
        if (sf::Joystick::hasAxis(0, sf::Joystick::Axis::Y)) {
            AXISY(sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::Y) / 50);
        }
    }
}
