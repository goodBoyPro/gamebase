#if !defined(GWIDGET_H)
#define GWIDGET_H
#include <GButton.h>
#include <GController.h>
#include <GObject.h>
class GWidget : public GWidgetInterface {
  private:
    /* data */
  public:
    // UI面板中心位置
    IVector position;
    int width;
    int height;
    GWidget(/* args */);
    GWidget(IVector pos, int width__, int height__);
    virtual ~GWidget();
    void draw(sf::RenderWindow &window_);
    virtual void onKeyPressed(sf::Keyboard::Key keycode) override {
        if (keycode == sf::Keyboard::U)
            pop();
    }
    virtual void onEventAny(sf::RenderWindow &window_) override;
    void resizeView();

    sf::Sprite spr;
    sf::Texture tex;
    std::vector<GButton> btns;

    IVector getLeftTopPoint();
    bool isOpened = 0;

  private:
};

#endif // GWIDGET_H
