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

class GController : public GControllerInterface {
  private:
    /* data */
  public:
    enum key {
        w = 0,
        wr, //
        a,
        ar,
        s,
        sr,
        d,
        dr,
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
        openUi,
        closeUi,
        b
    };
  public:
   
    virtual void pollKey(sf::RenderWindow&window_,sf::Event&event_);
};

#endif