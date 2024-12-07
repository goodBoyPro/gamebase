#if !defined(GANIMATION_H)
#define GANIMATION_H
#include <GBase.h>
#include <SFML/Graphics.hpp>

#define LEFT_ORTN 1   // 角色朝向左
#define RIGHT_ORTN -1 // 角色朝向右
class GAnimation {
  public:
    int currentFrame;
    float rate = 1;
    FVector3 scale = {1, 1, 1};

  public:
    GAnimation() {};
    ~GAnimation() {
        for (auto o : once) {
            delete o.task;
        }
    };
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
        originX = std::stoi(map_["originX"]);
        originY = std::stoi(map_["originY"]);
        spr.setOrigin(originX, originY);
    }

    sf::Texture tex;
    sf::Sprite spr;
    sf::IntRect rect;
    // 设置动画起始帧与结束帧,第一格为0
    void play(int start_, int end_) {
        if (start == start_)
            return;
        start = start_;
        end = end_;
        currentFrame = start;
    }
    void bindSprite(gameSprite &spr_) {
        spr_.setTexture(tex);
        spr_.setOrigin(originX, originY);
    }
    void updateOnSprite(gameSprite &spr_) {
        if (cr.delay(33 / rate)) {

            spr_.setTextureRect({currentFrame % column * sizex,
                                 currentFrame / column * sizey, sizex, sizey});

            currentFrame++;
            if (currentFrame > end) {
                currentFrame = start;
            }
            updateSound();
        }
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
    void playSoundAtFrameOnce(int frame, nsg::DoOnce &once);
    void bindSound(int frame_, sf::Sound &sound) {
        once.emplace_back(frame_, [&]() { sound.play(); });
    }

    private:
    int ptx = 0;
    int pty = 0;
    int sizex;
    int sizey;
    int row;
    int column;
    int originX;
    int originY;
    int start = 0;
    int end = 0;
    canRun cr;

  private:
    struct SoundAtFrame {

        SoundAtFrame(int frame_, std::function<void()> func)
            : frame(frame_), task(new nsg::DoOnce(std::move(func))) {}
        // std::vector扩容时会调用导致异常，所已不能在这里删除
        //~SoundAtFrame() { delete task; }
        int frame;
        nsg::DoOnce *task = nullptr;
    };
    std::vector<SoundAtFrame> once;
    void updateSound() {
        for (auto o : once) {
            if (currentFrame == o.frame) {
                (*(o.task))();
            }
            if (currentFrame - o.frame == 1) {
                (*(o.task)).reset();
            }
        }
    }
};

#endif // GANIMATION_H
