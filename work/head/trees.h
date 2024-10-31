#if !defined(TREES_H)
#define TREES_H
#include <GActor.h>
struct treesTexture {
    treesTexture() {
        tex.loadFromFile("res/trees_256x5.png");
        spr.setTexture(tex);
        spr.setOrigin(128,256);
    }
    sf::Texture tex;
    sf::Sprite spr;
};

class trees : public GActor {

  public:
  void gameBegin(){}
  void eventTick(){}
    trees(int _index) { ;
        setRenderSprite(&treeTex.spr);
        index = _index;
    };
    ~trees(){};
    int index;
    virtual void drawLoop()override{
        getRenderSprite()->setTextureRect({index/5*256,index%5*256,256,256});
        GActor::drawLoop();
    };
    static treesTexture treeTex;
};

#endif // TREES_H
