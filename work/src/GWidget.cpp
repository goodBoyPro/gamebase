#include "GWidget.h"
#include <GController.h>
#include <GDebug.h>
#include<game.h>
sf::View GWidget::viewPort;
//GController GWidget::widgetController;
GWidget::GWidget() {}

GWidget::GWidget(IVector pos, int width__, int height__) {
    position=pos;
    width = width__;
    height = height__;
    tex.loadFromFile("res/widget.png");
    spr.setTexture(tex);
    spr.setOrigin(spr.getTexture()->getSize().x/2,spr.getTexture()->getSize().y/2);
    spr.setPosition(pos.x,pos.y);
    spr.setScale(width/spr.getTexture()->getSize().x,height/spr.getTexture()->getSize().y);
   
    //widgetController.bindKey[GController::closeUi] = [&]() { remove(); };
    widgetController.bindKey[GController::uiOpenOrClose] = [this]() {
        UiSwitch();
       
    };
    btns.emplace_back(IVector(getLeftTopPoint().x+100,getLeftTopPoint().y+100),160,80,L"开始游戏");
    btns.emplace_back(IVector(getLeftTopPoint().x+100,getLeftTopPoint().y+200),160,80,L"关闭游戏");

    btns[0].onClicked = [&]() { remove(); };
    btns[1].onClicked = []() {  getWindow()->setMouseCursorGrabbed(false);getGameIns()->bGameContinue = 0; };
}

GWidget::~GWidget() {}

void GWidget::draw() {
    getWindow()->draw(spr);

    for (GButton &a : btns) {
        a.drawLoop();
    }
    
   
}

void GWidget::addToViewport() {

    setWidgetPtr(this);
    saveController =(GController*)getPlayerController();
    setPlayerController(&widgetController);
    
    isOpened = true;
}

void GWidget::remove() {
    ;
   setWidgetPtr(nullptr);
    setPlayerController(saveController);
    widgetController.disableActive();
    isOpened = false;
}

void GWidget::resizeView() {}

IVector GWidget::getLeftTopPoint() {
    ;    
    return {position.x-width/2,position.y-height/2};
}

void GWidget::UiSwitch() {
    if(isOpened)
        remove();else
        addToViewport();
}
