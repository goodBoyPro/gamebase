#include "GAnimation.h"


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