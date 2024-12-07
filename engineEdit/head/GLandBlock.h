#if !defined(GLANDBLOCK_H)
#define GLANDBLOCK_H
#include <GActor.h>
#include <GSource.h>
class GLandBlock : public GActor
{
private:
    static FVector2 size;
    gameSprite spr;
    int index;
    static int rows;
    static int columns;
    static FVector2 mapBeginPoint;

public:
    GLandBlock(int index_);
    ~GLandBlock();
    static void spawnLandBlock(int index_);
    virtual void eventBegin() {};
    virtual void eventTick() {};
    void setPosForIndex();
};
inline FVector2 GLandBlock::size = {2, 2};
inline int GLandBlock::rows = 100;
inline int GLandBlock::columns = 100;
inline FVector2 GLandBlock::mapBeginPoint = {-10, -10};
inline GLandBlock::GLandBlock(int index_)
{
    index = index_;
    getSource().setSprite(spr, 3, 5);
    setRenderSprite(&spr);
    spr.setOrigin(0, 0);
    setSize(size.x / pixSize, size.y / pixSize);
}

inline GLandBlock::~GLandBlock()
{
}
inline void GLandBlock::spawnLandBlock(int index_)
{
    spawnActorArgsAtLocation(new GLandBlock(index_))->setPosForIndex();
}

inline void GLandBlock::setPosForIndex()
{
    setPosInWs({index % columns * size.x, index / columns * size.y, 0});
}

#endif // GLANDBLOCK_H
