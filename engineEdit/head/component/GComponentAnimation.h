#if !defined(GCOMPONENTANIMATION_H)
#define GCOMPONENTANIMATION_H
#include <GActor.h>
#include <GAnimationBp.h>
class GComponentAnimation :public GActorComponent
{
private:
    class GAnimationBpInterface *animationBpPtr = nullptr;

public:
    GComponentAnimation(/* args */) {};
    virtual ~GComponentAnimation() { delete animationBpPtr; };
    GAnimationBpInterface *getAnimationBp(){return animationBpPtr;}
    template <class T>
    void createAnimationBp()
    {
        animationBpPtr = new T;
        setRenderSprite(animationBpPtr->sprPt);
    };
    virtual void dataLoop()
    {
        animationBpPtr->updateAnim();
        animationBpPtr->updateSound();
    };
};

#endif // GCOMPONENTANIMATION_H
