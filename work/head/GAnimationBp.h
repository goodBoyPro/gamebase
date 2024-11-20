#if !defined(GANIMATIONBP_H)
#define GANIMATIONBP_H
#include <GAnimation.h>
#include <GObject.h>
#include <GSource.h>
class GAnimationBpInterface
{
  private:std::vector<nsg::DoOnce*>___AllSounds___;
public:
  virtual ~GAnimationBpInterface() {for(auto ptr:___AllSounds___)delete ptr;}
  virtual void updateAnim() = 0;
  virtual void updateSound() = 0;
  nsg::DoOnce* createSoundEvent(sf::Sound &sound_)
  {
    auto ptr=new nsg::DoOnce([&](){sound_.play();});
    ___AllSounds___.push_back(ptr);
    return ptr;
  };
  sf::Sprite *sprPt = nullptr;
  GAnimation *animPt = nullptr;
};
class GAnimationBp : public GAnimationBpInterface
{
public:
  enum animState
  {
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
  std::vector<sf::Sound *> sounds;
  
  nsg::DoOnce sound1 = []()
  { getSource().getSound(2).play(); };
  nsg::DoOnce sound2 = []()
  { getSource().getSound(2).play(); };

public:
  GAnimationBp();
  virtual ~GAnimationBp() = default;
  virtual void updateAnim();
  virtual void updateSound();
};

#endif // GANIMATIONBP_H
