#include <GAnimationBp.h>

GAnimationBp::GAnimationBp():isPlayed(64,0){
    ;
    animPt = &getSource().getAnimation(1);
    animPt->spr.setScale(2,2);
    animPt->play(40, 59);
    // sprPt = &animPt->spr;
    // animPt->scale = {2, 2,0};
    sprPt = &spr;
    animPt->bindSprite(spr);
    animPt->bindSound(3,getSource().getSound(2));
    animPt->bindSound(13,getSource().getSound(2));
    spr.setTexture(animPt->tex);
    setScale(2,2);
}

void GAnimationBp::updateAnim() {
   

    switch (state) {
    case idle:
        animPt->play(40, 59);
        break;
    case walk:
        sprPt->setScale(getScale().x * orientation, getScale().y);
        
        animPt->play(0, 19);
        break;
    case jump:
        animPt->play(60,70);
        break;

    default:
        break;
    }
    animPt->updateOnSprite(spr);
     
   
    
    
}
