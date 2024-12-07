#if !defined(TREES_H)
#define TREES_H
#include <GActor.h>
#include <GSource.h>
class GActorStatic : public GActor {
  public:
    gameSprite spr;
    void eventBegin() {}
    void eventTick() {}
    void init(int _index) {        
        index = _index;
        spr.setTexture(getSource().getTexArray(1).tex);
        spr.setOrigin({128, 256});
        spr.setTextureRect({index / 5 * 256, index % 5 * 256, 256, 256});
    }
    GActorStatic(){}
    GActorStatic(int fileID, int picIndex) {
        setRenderSprite(&(this->spr));
        getSource().setSprite(spr,fileID,picIndex);
    };
    ~GActorStatic() {};
    int index;

    
};

#endif // TREES_H
