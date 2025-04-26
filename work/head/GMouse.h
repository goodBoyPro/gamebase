#if !defined(GMOUSE_H)
#define GMOUSE_H
#include <GObject.h>
class GMouse:public GIMouse {
  private:
    /* data */
  public:
    GMouse() {     
        tex.loadFromFile("res/mouse.png");
        spr.setTexture(tex);
        spr.setScale(0.3, 0.3);
        setMouseVisible(*(Game::gameIns->gameWindow), 0);
        GIMouse::mousePtr = this;
    };
    ~GMouse() {};
    sf::Texture tex;
    sf::Sprite spr;
    

    virtual void drawMouseCusor(sf::RenderWindow &window_) {
        spr.setPosition((sf::Vector2f)(sf::Mouse::getPosition(window_)));
        window_.draw(spr);
    }
    void setMouseVisible(sf::RenderWindow &window_, bool isVisible) {
        window_.setMouseCursorVisible(isVisible);
    }
};

#endif // GMOUSE_H
