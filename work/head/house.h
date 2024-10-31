#if !defined(HOUSE_H)
#define HOUSE_H
#include<GActor.h>
struct houseTexture {
    houseTexture() {
        tex.loadFromFile("res/house_256x3.png");
        spr.setTexture(tex);
        spr.setOrigin(128,220);
    }
    sf::Texture tex;
    sf::Sprite spr;
};

class house : public GActor {

  public:
  void gameBegin(){}
  void eventTick(){}
    house(int _index) { ;
       setRenderSprite(&houseTex.spr);
        index = _index;
    };
    ~house(){};
    int index;
    virtual void drawLoop()override{
        getRenderSprite()->setTextureRect({index/3*256,index%3*256,256,256});
        GActor::drawLoop();
    };
    static houseTexture houseTex;
    
};




#endif // HOUSE_H
