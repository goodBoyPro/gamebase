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

    
    void drawMouseCusor() {
        spr.setPosition((sf::Vector2f)(sf::Mouse::getPosition(*getWindow())));
        getWindow()->draw(spr);
    }
    void setMouseVisible(bool isVisible){
        getWindow()->setMouseCursorVisible(isVisible);
    }
};

#endif // GMOUSE_H
