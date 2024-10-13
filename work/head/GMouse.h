#if !defined(GMOUSE_H)
#define GMOUSE_H
#include <GObject.h>
class GMouse {
  private:
    /* data */
  public:
    GMouse(/* args */);
    ~GMouse();
    sf::Texture tex;
    sf::Sprite spr;

    void drawMouseText() {
        printText(L"我是鼠标", sf::Mouse::getPosition(*window).x,
                  sf::Mouse::getPosition(*window).y, 60, sf::Color(255, 0, 0));
    };
    void drawMouseCusor() {
        spr.setPosition((FVector)(sf::Mouse::getPosition(*window)));
        window->draw(spr);
    }
    void setMouseVisible(bool isVisible){
        window->setMouseCursorVisible(isVisible);
    }
};

#endif // GMOUSE_H
