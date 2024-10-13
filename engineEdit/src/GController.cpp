#include "GController.h"

#include <game.h>

GController::~GController() {}

void GController::pollKey() {

    // 无延迟按键
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        playerController->bindKey[GController::w]();
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        playerController->bindKey[GController::a]();
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        playerController->bindKey[GController::s]();
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        playerController->bindKey[GController::d]();
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
        playerController->bindKey[GController::space]();
    // if (!sf::Keyboard::isKeyPressed(sf::Keyboard::W) &&
    //     !sf::Keyboard::isKeyPressed(sf::Keyboard::A) &&
    //     !sf::Keyboard::isKeyPressed(sf::Keyboard::S) &&
    //     !sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    //     playerController->KEYIDLE();

    if (window->pollEvent(gameIns->event)) {
        if (gameIns->event.type == sf::Event::Closed) {
            window->close();
            gameIns->bGameContinue = 0;
        }
        if (gameIns->event.type == sf::Event::Resized)
            resizeWindow(window);

        // 键盘按键
        if (gameIns->event.type == sf::Event::KeyPressed) {
            switch (gameIns->event.key.code) {
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
        // 鼠标
        if (gameIns->event.type == sf::Event::MouseButtonPressed) {
            switch (gameIns->event.mouseButton.button) {
            case sf::Mouse::Left:
                playerController->bindKey[GController::mouseLeft]();

                break;
            case sf::Mouse::Right:
                playerController->KEYRIGHT();
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
            playerController->AXISX(
                sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::X) / 50);
        }
        if (sf::Joystick::hasAxis(0, sf::Joystick::Axis::Y)) {
            playerController->AXISY(
                sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::Y) / 50);
        }
    }
}
