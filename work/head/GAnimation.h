#if !defined(GANIMATION_H)
#define GANIMATION_H
#include <SFML/Graphics.hpp>
#include <timer.h>
#include<GBase.h>

#define LEFT_ORTN 1     //角色朝向左
#define RIGHT_ORTN -1   //角色朝向右
class GAnimation {
  public:
    int currentFrame;
    float rate = 1;
    FVector scale={1,1};

  public:
    GAnimation() = default;
    GAnimation(const char *path, int sizex_, int sizey_, int row_, int column_)
        : sizex(sizex_), sizey(sizey_), row(row_), column(column_) {

        if (!tex.loadFromFile(path))
            printf("%s load failed", path);
        spr.setTexture(tex);
        spr.setOrigin(sizex_ / 2, sizey_);
    };

    ~GAnimation() {};
    sf::Texture tex;
    sf::Sprite spr;

    void init(const char *path, int sizex, int sizey, int row, int column) {
        if (!tex.loadFromFile(path))
            printf("%s load failed", path);
        spr.setTexture(tex);
    }

    // 设置动画起始帧与结束帧,第一格为0
    void play(int start_, int end_) {
        if (start == start_)
            return;
        start = start_;
        end = end_;
        currentFrame = start;
    }

    void update() {
        if (cr.delay(33 / rate)) {

            spr.setTextureRect({currentFrame % column * sizex,
                                currentFrame / column * sizey, sizex, sizey});

            currentFrame++;
            if (currentFrame > end) {
                currentFrame = start;
            }
        }
    }
     void playSoundAtFrame(sf::Sound&s,int frame,bool &isPlayed);

  private:
    int ptx = 0;
    int pty = 0;
    int sizex;
    int sizey;
    int row;
    int column;
    int start = 0;
    int end = 0;
    canRun cr;
};

#endif // GANIMATION_H
