#if !defined(GWIDGET_H)
#define GWIDGET_H
#include <GObject.h>
#include<GButton.h>
#include<GController.h>
class GWidget : public GObject {
  private:
    /* data */
  public:
  //UI面板中心位置
    IVector position;
    int width;
    int height;
    GWidget(/* args */);
    GWidget(IVector pos,int width__,int height__);
    virtual ~GWidget();
    void draw(sf::RenderWindow&window_);
    void addToViewport();
    void remove();
    void resizeView();
  
    sf::Sprite spr;
    sf::Texture tex;
    std::vector<GButton> btns;
    GController widgetController;
    IVector getLeftTopPoint();
    bool isOpened = 0;
    //切换ui显示
    void UiSwitch();

  private:
    GController *saveController;
};

#endif // GWIDGET_H
