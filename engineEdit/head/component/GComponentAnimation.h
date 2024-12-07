#if !defined(GCOMPONENTANIMATION_H)
#define GCOMPONENTANIMATION_H
#include <GActorWithAnim.h>
#include <GAnimationBp.h>
//为了优化性能，迭代时不对指针判空，使用时务必创建动画蓝图并引用有效的动画资产
class GComponentAnimation :public GActorComponent
{
private:
    class GAnimationBpInterface *animationBpPtr = nullptr;

public:
    GActorWithAnim *ownerPtr=nullptr;
    GComponentAnimation(/* args */) {};
    virtual ~GComponentAnimation() { delete animationBpPtr; };
    GAnimationBpInterface *getAnimationBp(){return animationBpPtr;}
    template <class T>
    void createAnimationBp()
    {
        delete animationBpPtr;
        animationBpPtr = new T;
        setRenderSprite(animationBpPtr->sprPt);
    };
    virtual void dataLoop()
    {   
        ownerPtr->updateState();
        animationBpPtr->updateAnim();
        animationBpPtr->animPt->update();
    };
};

#endif // GCOMPONENTANIMATION_H
