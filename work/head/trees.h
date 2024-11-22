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
    gameSprte spr;
    void eventBegin() {}
    void eventTick() {
    }
    void init(int _index){setRenderSprite(&(this->spr));
        index = _index;
        spr.setTexture(treeTex.tex);
        spr.setOrigin({128,256});
        spr.setTextureRect(
            {index / 5 * 256, index % 5 * 256, 256, 256});}
    trees() { ;
        
    };
    ~trees(){};
    int index;
   
    static treesTexture treeTex;
};

#endif // TREES_H
