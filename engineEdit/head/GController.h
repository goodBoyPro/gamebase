#define KEYW bindKey[0]
#define KEYA bindKey[1]
#define KEYS bindKey[2]
#define KEYD bindKey[3]
#define KEYUP bindKey[4]
#define KEYDOWN bindKey[5]
#define KEYLEFT bindKey[6]
#define KEYRIGHT bindKey[7]
#define AXISX bindAxis[0]
#define AXISY bindAxis[1]
#define KEYIDLE bindKey[8]
#define KEYJUMP bindKey[9]

#ifndef GCONTROLLER_H
#define GCONTROLLER_H
#include <GObject.h>
#include <functional>
#include <string.h>

class GController : public GObject {
  private:
    /* data */
  public:
    enum key {
        w,
        a,
        s,
        d,
        q,
        e,
        r,
        up,
        down,
        space,
        left,
        right,
        mouseRight,
        mouseLeft,
        uiOpenOrClose,
        openUi, closeUi
    };
    GController() {
        auto x = []() {};
        auto y = [](float) {};//超出作用域后仍然起作用是因为这是一个复制操作
        { printf("No Key Bind\n"); };
        for (std::function<void()> &func : bindKey)
            func =x;
        for (std::function<void(float)> &func : bindAxis)
            func = y;
    };
    virtual~GController();
    std::function<void()> emptyFunc;

  public:
    std::function<void()> bindKey[30];
    std::function<void(float)> bindAxis[30];
    virtual void pollKey();
};

#endif