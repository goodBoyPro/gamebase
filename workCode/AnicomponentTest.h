#if !defined(ANICOMPONENTTEST_H)
#define ANICOMPONENTTEST_H
#include<GComponentAnimation.h>
#include<GAnimationBp.h>
class anicomponenttest:public GComponentAnimation
{
private:
    /* data */
public:
    anicomponenttest(/* args */){
        createAnimationBp<GAnimationBp>();
    };
    ~anicomponenttest(){};
    void gameBegin(){}
    void eventTick(){}
};




#endif // ANICOMPONENTTEST_H
