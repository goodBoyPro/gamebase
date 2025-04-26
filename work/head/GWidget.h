#if !defined(GWIDGET_H)
#define GWIDGET_H
#include <GButton.h>
#include <GController.h>
#include <GObject.h>
#include <GSource.h>
class buttonTest : public GIButton {
  public:
    textureArray *tex = nullptr;
    buttonTest() { tex = &(getSource().getTexArray(5)); }
    virtual void setSprIdle() {
        tex->setSprite(spr, 0);
        textState.textColor = {255, 255, 255};
    };
    virtual void setSprHover() {
        tex->setSprite(spr, 6);
        textState.textColor = {255, 255, 255};
    };
    virtual void setSprClicked() {
        tex->setSprite(spr, 12);
        textState.textColor = {255, 0, 0};
    };
};
class UIMain : public GIUi {
  public:
    UIMain() {
        getSource().setSprite(spr, 4, 0);
        setSize({WINW * 0.8, WINH * 0.8});
        int edge = 30;
        int height = getSize().y - edge * 2;
        int width = getSize().x - edge * 2;
        int row = 10;
        int column = 10;
        int w = width / column - 4;
        int h = height / row - 4;
        for (int i = 0; i < 100; i++) {
            buttonTest *btn = (buttonTest *)createUI<buttonTest>();
            btn->setPosition(
                {i % 10 * (w + 4) + edge + 2, i / 10 * (h + 4) + edge + 2});
            btn->setSize({w, h});
            btn->addToViewPort();
            btn->setLayer(1);
            int x = i;
            btn->text = L"测试";
            btn->onMouseLeftClicked = [=]() { printf("%d\n", x); };
        }
    }
    std::function<void()> onKeyPressed = []() {};
    virtual void pollKey(sf::RenderWindow &window_,
                         sf::Event &event_) override {
        GIUi::pollKey(window_, event_);
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
class GWidget : public GIWidget {
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
