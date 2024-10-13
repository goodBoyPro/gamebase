#if !defined(GANIMATIONBP_H)
#define GANIMATIONBP_H
#include <GAnimation.h>
#include <GObject.h>
#include <GSource.h>

class GAnimationBp {
  public:
    enum animState {
        idle, //
        walk,
        run,
        attack,
        jump
    };

  public:
    int orientation = LEFT_ORTN;
    animState state = idle;
    // 用来标记声音是否播放,但是数组大小大于8会造成动画不显示
    // bool isPlayed[16];
    std::vector<char> isPlayed;
    sf::Sprite *sprPt = nullptr;
    GAnimation *animPt = nullptr;

  public:
    GAnimationBp();
    virtual ~GAnimationBp() = default;
    virtual void updateAnim();
    virtual void updateSound();
};

#endif // GANIMATIONBP_H
