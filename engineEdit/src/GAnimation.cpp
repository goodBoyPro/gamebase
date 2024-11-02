#include "GAnimation.h"

//动画跳至下一帧前窗口可能会在同一帧刷新很多次，会造成重复播放
void GAnimation::playSoundAtFrame(sf::Sound & s, int frame,bool &isPlayed)
{
    if(currentFrame==frame&&!isPlayed){
        s.play();
        isPlayed = 1;
    }
    if (currentFrame!=frame)
    {
        isPlayed = 0;
    }
    
    
}

void GAnimation::playSoundAtFrameOnce(int frame, nsg::DoOnce &once)
{
    if(currentFrame==frame){
        once();
    }
    if (currentFrame-frame==1)
    {
       once.reset();
    }
}
