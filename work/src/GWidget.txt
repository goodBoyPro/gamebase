#include "GWidget.h"
#include <GController.h>
#include <GDebug.h>
#include <game.h>

// GController GWidget::widgetController;
GWidget::GWidget() {}

GWidget::GWidget(IVector pos, int width__, int height__) {
   
    position = pos;
    width = width__;
    height = height__;
    tex.loadFromFile("res/widget.png");
    spr.setTexture(tex);
    spr.setOrigin(spr.getTexture()->getSize().x / 2,
                  spr.getTexture()->getSize().y / 2);
    spr.setPosition(pos.x, pos.y);
    spr.setScale(width / spr.getTexture()->getSize().x,
                 height / spr.getTexture()->getSize().y);

    btns.emplace_back(
        IVector(getLeftTopPoint().x + 100, getLeftTopPoint().y + 100), 160, 80,
        L"开始游戏");
    btns.emplace_back(
        IVector(getLeftTopPoint().x + 100, getLeftTopPoint().y + 200), 160, 80,
        L"关闭游戏");

    btns[0].onClicked = [&]() { if(pop()){GControllerInterface::getPlayerController()->setFoucusOnWidget(false);}; };
    btns[1].onClicked = []() {
        ((Game *)(GGameInterface::getGameIns()))
            ->gameWindow->setMouseCursorGrabbed(false);
        GGameInterface::getGameIns()->bGameContinue = 0;
    };
}

GWidget::~GWidget() {}

void GWidget::draw(sf::RenderWindow &window_) {
    window_.draw(spr);
    
    for (GButton &a : btns) {
        a.drawLoop(window_);
    }
}

void GWidget::onEventAny(sf::RenderWindow &window_,sf::Event&event_) {
    for (GButton &a : btns) {
        a.pollKey(window_);
    }
    if(event_.type==sf::Event::KeyPressed){
        switch (event_.key.code)
        {
        case sf::Keyboard::Backspace:
            
            if(pop()){GControllerInterface::getPlayerController()->setFoucusOnWidget(false);};
            break;
        
        default:
            break;
        }
    }
}

void GWidget::resizeView() {}

IVector GWidget::getLeftTopPoint() {
    ;
    return {position.x - width / 2, position.y - height / 2};
}
