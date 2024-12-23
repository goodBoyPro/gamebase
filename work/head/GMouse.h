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

    
    void drawMouseCusor(sf::RenderWindow&window_) {
        spr.setPosition((sf::Vector2f)(sf::Mouse::getPosition(window_)));
        window_.draw(spr);
    }
    void setMouseVisible(sf::RenderWindow&window_,bool isVisible){
        window_.setMouseCursorVisible(isVisible);
    }
};

#endif // GMOUSE_H
