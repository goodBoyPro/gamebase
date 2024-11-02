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
        printText(L"我是鼠标", sf::Mouse::getPosition(*getWindow()).x,
                  sf::Mouse::getPosition(*getWindow()).y, 60, sf::Color(255, 0, 0));
    };
    void drawMouseCusor() {
        spr.setPosition((sf::Vector2f)(sf::Mouse::getPosition(*getWindow())));
        getWindow()->draw(spr);
    }
    void setMouseVisible(bool isVisible){
        getWindow()->setMouseCursorVisible(isVisible);
    }
};

#endif // GMOUSE_H
