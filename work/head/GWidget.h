#if !defined(GWIDGET_H)
#define GWIDGET_H
#include <GButton.h>
#include <GController.h>
#include <GObject.h>
#include <GSource.h>
class buttonTest : public GButtonInterface {
  public:
    textureArray *tex = nullptr;
    buttonTest() { tex = &(getSource().getTexArray(5)); }
    virtual void setSprIdle() { tex->setSprite(spr, 0); };
    virtual void setSprHover() { tex->setSprite(spr, 6); };
    virtual void setSprClicked() { tex->setSprite(spr, 12); };
};
class UIMain : public GUiInterface {
  public:
    UIMain() {
        getSource().setSprite(spr, 4, 0);
        setSize({WINW * 0.8, WINH * 0.8});
        for (int i = 0; i < 100; i++) {
            buttonTest *btn = (buttonTest *)createUI<buttonTest>();
            btn->setPosition({i%10*90, i/10*40});
            btn->setSize({80, 30});
            btn->addToViewPort();
            btn->setLayer(1);
            int x = i;
            btn->onMouseLeftClicked = [=]() { printf("%d\n",x); };
        }
       
    }
    std::function<void()> onKeyPressed = []() {};
    virtual void pollKey(sf::RenderWindow &window_,
                         sf::Event &event_) override {
        GUiInterface::pollKey(window_, event_);
        if (event_.type == sf::Event::KeyPressed)
            switch (event_.key.code) {
            case sf::Keyboard::Backspace:
                onKeyPressed();
                break;

            default:
                break;
            }
    }
};
class GWidget : public GWidgetInterface {
  private:
  public:
    GWidget() {
        UIMain *x = (UIMain *)createUI<UIMain>();
        x->setPosition({WINW * 0.1, WINH * 0.1});
        x->onKeyPressed = [&]() { pop(); };
        x->addToViewPort();
    };
};

#endif // GWIDGET_H
