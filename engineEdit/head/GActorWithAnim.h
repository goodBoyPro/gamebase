#if !defined(MACRO)
#define MACRO
#include <GActor.h>
#include <GAnimationBp.h>
class GActorWithAnim : public GActor
{
private:
    class GAnimationBpInterface *aniBpPtr__antiRename = nullptr;
    class GComponentAnimation *componentAnimationPtr = nullptr;

public:
    virtual void updateState() = 0;
    GActorWithAnim(/* args */) {};
    void setRenderAnimationBp(GAnimationBpInterface *bp)
    {
        aniBpPtr__antiRename = bp;
        setRenderSprite(aniBpPtr__antiRename->sprPt);
    };
    GAnimationBpInterface *getRenderAnimationBp() { return aniBpPtr__antiRename; }
    template <class T>
    GComponentAnimation *createAnimationComponent()
    {
        componentAnimationPtr = createActorComponent<T>(new T);
        componentAnimationPtr->ownerPtr=this;
        return (T*)componentAnimationPtr;
    }
    GComponentAnimation *getAnimationComponent()
    {
        return componentAnimationPtr;
    }
};

#endif // MACRO
