#include "GButton.h"
#include <GDebug.h>
#include <GWidget.h>
#include <game.h>
texButton tbtn;
GButton::GButton(IVector pos__, int width_, int height_,const wchar_t *wchar_) {
    // 防止未定义按键导致程序崩溃
    onClicked = []() {};
    colorPtr = &textColorIdle;
    position = pos__;
    width = width_;
    height = height_;
    swprintf(wchar,wchar_);
    spr.setTexture(tbtn.tex);
    sprOriginSize = {(int)spr.getTexture()->getSize().x,(int)spr.getTexture()->getSize().y};
    spr.setPosition(position.x, position.y);
    spr.setScale(width / (float)spr.getTexture()->getSize().y,
                 height / (float)spr.getTexture()->getSize().y);
    
}
bool bOnce = 1;

void GButton::drawLoop(sf::RenderWindow&window_) {
    
    spr.setPosition(position.x, position.y);
    spr.setTextureRect({state * sprOriginSize.y, 0,
                        sprOriginSize.y, sprOriginSize.y});
    window_.draw(spr);
    printText(window_,wchar,position.x+height*0.5,position.y+height*0.3,height*0.3,*colorPtr,font[2]);
}
bool GButton::isMouseOn(sf::RenderWindow&window_) {
    int x = sf::Mouse::getPosition(window_).x - position.x;
    int y = sf::Mouse::getPosition(window_).y - position.y;
    if (x < 0 || x > width || y < 0 || y > height)
        return false;
    return true;
}
void GButton::pollKey(sf::RenderWindow&window_) {
     
    if (isMouseOn(window_)) {
        state = GButton::hovered;
        colorPtr = &textColorHovered;
        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
            if (bOnce) {
                state = clicked;
                onClicked();
            }
            bOnce = 0;
        } else
            bOnce = 1;

    }

    else
       { state = GButton::idle;
        colorPtr = &textColorIdle;
       }
};
