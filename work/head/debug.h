#if !defined(DEBUG_H)
#define DEBUG_H
#include <GDebug.h>
#include<GCamera.h>
#include<game.h>
class debug:public GDebug
 {
  private:
    /* data */
  public:
    debug(/* args */){};
    ~debug(){};
    virtual void drawLoop() {
        // swprintf(wchar_, L"场景对象数量：%d", actors.size());
        // drawLog();
        // swprintf(wchar_, L"像素尺寸：%d", (int)pixSize);
        // drawLog();
        // swprintf(wchar_, L"玩家位置：[%.3f,%.3f]", gameCamera->posInWs.x,
        //          gameCamera->posInWs.y);
        // drawLog();
        // swprintf(wchar_, L"裁剪距离：%.3f", gameIns->loopDistance);
        // drawLog();
        // swprintf(wchar_, L"deltaTime:%d", deltaTime);
        // drawLog();
        // swprintf(wchar_, L"键鼠位置:%d,%d", sf::Mouse::getPosition(*window).x,sf::Mouse::getPosition(*window).y);
        // drawLog();
        drawLog();
        }
};

#endif // DEBUG_H
