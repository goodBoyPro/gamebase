#if !defined(MACRO)
#define MACRO
#include <GActor.h>
#include <GAnimationBp.h>
class GActorWithAnim : public GActor
{
private:
    class GAnimationBpInterface *aniBpPtr__antiRename = nullptr;

public:
    virtual void updateState() = 0;
    GActorWithAnim(/* args */) {};
    void setRenderAnimationBp(GAnimationBpInterface *bp)
    {
        aniBpPtr__antiRename = bp;
        setRenderSprite(aniBpPtr__antiRename->sprPt);
    };
    GAnimationBpInterface *getRenderAnimationBp() { return aniBpPtr__antiRename; }
    void drawAnimation()
    {
        if (aniBpPtr__antiRename)
        {
            aniBpPtr__antiRename->updateAnim();
            updateState();
        }
       
    };
};

#endif // MACRO
