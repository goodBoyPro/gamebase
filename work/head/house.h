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
    gameSprite spr;
    void eventBegin() {}
    void eventTick() {
          }
    house(int _index) { ;
       setRenderSprite(&spr);
       spr.setTexture(houseTex.tex);
       index = _index;
       spr.setTextureRect(
            {index / 3 * 256, index % 3 * 256, 256, 256});
       spr.setOrigin(128,220);
    };
    ~house(){};
    int index;
   
    static houseTexture houseTex;
    
};




#endif // HOUSE_H
