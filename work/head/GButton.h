#if !defined(GBUTTON_H)
#define GBUTTON_H
#include <GObject.h>

class GButton : public GObject {

  public:
    IVector position={0,0};
    int width=40;
    int height=20;
    IVector sprOriginSize;
    wchar_t wchar[8];
    sf::Color textColorIdle={0,0,0};
    sf::Color textColorHovered={255,255,255};
    enum btnState { idle, hovered, clicked };
    GButton() = default;
    GButton(IVector pos__,int width_,int height_,const wchar_t *wchar_);

    sf::Sprite spr;
    btnState state = idle;
    sf::Color *colorPtr = nullptr;
    ~GButton() = default;
    std::function<void()> onClicked;
    void drawLoop(sf::RenderWindow&window_);
    bool isMouseOn(sf::RenderWindow&window_);
    void pollKey(sf::RenderWindow&window_);
};
struct texButton {
    sf::Texture tex;

    texButton() {
        tex.loadFromFile("res/button_128x3.png");
       
    }
};
#endif // GBUTTON_H
