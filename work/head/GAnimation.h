#if !defined(GANIMATION_H)
#define GANIMATION_H
#include <GBase.h>
#include <SFML/Graphics.hpp>
#include <timer.h>

#define LEFT_ORTN 1   // 角色朝向左
#define RIGHT_ORTN -1 // 角色朝向右
class GAnimation {
  public:
    int currentFrame;
    float rate = 1;
    FVector scale = {1, 1};

  public:
    GAnimation() { };
    GAnimation(const char *path, int sizex_, int sizey_, int row_, int column_)
        : sizex(sizex_), sizey(sizey_), row(row_), column(column_) {

        if (!tex.loadFromFile(path))
            printf("%s load failed", path);
        spr.setTexture(tex);
        spr.setOrigin(sizex_ / 2, sizey_);
    };
    GAnimation(std::map<std::string, std::string> &map_) {
        if (!tex.loadFromFile(map_["path"]))
            printf("%s load failed", map_["path"].c_str());
        spr.setTexture(tex);
        sizex = std::stoi(map_["sizex"]);
        sizey = std::stoi(map_["sizey"]);
        row = std::stoi(map_["row"]);
        column = std::stoi(map_["column"]);
        int originX = std::stoi(map_["originX"]);
        int originY = std::stoi(map_["originY"]);
        spr.setOrigin(originX, originY);
       
    }

    ~GAnimation() {};
    sf::Texture tex;
    sf::Sprite spr;
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
    void playSoundAtFrame(sf::Sound &s, int frame, bool &isPlayed);
    void playSoundAtFrameOnce(int frame,nsg::DoOnce&once);

   //private:
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
