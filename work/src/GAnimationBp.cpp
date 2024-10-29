#include <GAnimationBp.h>

GAnimationBp::GAnimationBp():isPlayed(64,0){
    ;
    animPt = &getSource().getAnimation(1);
    animPt->play(40, 59);
    sprPt = &animPt->spr;
    animPt->scale = {2, 2};
}

void GAnimationBp::updateAnim() {
   

    switch (state) {
    case idle:
        animPt->play(40, 59);
        break;
    case walk:
        sprPt->setScale(animPt->scale.x * orientation, animPt->scale.y);
        animPt->play(0, 19);
        break;
    case jump:
        animPt->play(60,70);
        break;

    default:
        break;
    }
    animPt->update();
     
    //getWindow()->draw(*sprPt);
     //播放声音
    updateSound();
    
}

void GAnimationBp::updateSound() {
    
    // animPt->playSoundAtFrame(getSource().getSound(2), 3, (bool&)isPlayed[0]);    
    // animPt->playSoundAtFrame(getSource().getSound(2), 13, (bool&)isPlayed[1]);
    animPt->playSoundAtFrameOnce(3,sound1);
    animPt->playSoundAtFrameOnce(13,sound2);
   
}
