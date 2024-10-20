#ifndef GAME_H
#define GAME_H
#include "GObject.h"

class Game : public GObject {
  private:
    /* data */
  public:
    Game(/* args */);
    virtual ~Game();
    virtual void gameBegin();
    virtual void dataLoop();    
    void renderLoop2D();   
    sf::RenderWindow *gameWindow;
    sf::Event event;
    // 响应按键
    virtual void pollKey();     
    virtual void setWinIcon();
    // 由于控制器的全局指针随时会变，每个对象的控制器需要单独标记
    class GController *gameController;
    class GMouse *mouseGame;
    class GWorld *world1;
    bool bGameContinue = 1;
    // 超过此距离的对象不执行逻辑
    float loopDistance=5;
};
#endif