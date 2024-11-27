#if !defined(TREES_H)
#define TREES_H
#include <GActor.h>
#include <GSource.h>
struct treesTexture {
    treesTexture() {
        tex.loadFromFile("res/trees_256x5.png");
        spr.setTexture(tex);
        spr.setOrigin(128, 256);
    }
    sf::Texture tex;
    sf::Sprite spr;
};

class trees : public GActor {

  public:
    gameSprte spr;
    void eventBegin() {}
    void eventTick() {}
    void init(int _index) {
        
        index = _index;
        spr.setTexture(getSource().getTexArray(1).tex);
        spr.setOrigin({128, 256});
        spr.setTextureRect({index / 5 * 256, index % 5 * 256, 256, 256});
    }
    trees(){}
    trees(int fileID, int picIndex) {
        setRenderSprite(&(this->spr));
        textureArray & ta=getSource().getTexArray(fileID);
        spr.setTexture(ta.tex);
        spr.setOrigin({ta.originX,
                       ta.originY});
        spr.setTextureRect({picIndex / ta.column * ta.sizex, picIndex % ta.column * ta.sizey, ta.sizex, ta.sizey});
    };
    ~trees() {};
    int index;

    static treesTexture treeTex;
};

#endif // TREES_H
