#if !defined(GTALK_H)
#define GTALK_H
#include <GActor.h>
#include <GSource.h>
class GTalk : public GActorComponent {
  public:
    void eventBegin() {}
    void eventTick() {}
    sf::Sprite *playerSpr=nullptr;

  private:
    /* data */
  public:
    GTalk(/* args */) { setRenderSprite(&getSprite(11));
        getRenderSprite()->setOrigin(0,getRenderSprite()->getTextureRect().getSize().y);
    };
    ~GTalk() = default;
    int leftEdge = 10;
    int topEdge = 5;

    std::wstring str[4] = {L"床前看月光，", //
                           L"疑是地上霜。", //
                           L"举头望山月，", //
                           L"低头思故乡。"};

    int textFlag = 0;
    int strFlag = 0;
    canRun cr;
    canRun sr;
    virtual void drawActor(sf::RenderWindow&window_,const FVector3&cameraPos_) override {
        GActor::drawActor(window_,cameraPos_);
        setContent(window_);
    }
    void setContent(sf::RenderWindow&window_) {
        const wchar_t *consStr;
        if (cr.delay(200)) {
            if (textFlag < 50)
                textFlag++;
        }
        if (sr.delay(4000)) {
            if (strFlag < 3) {
                textFlag = 0;
                strFlag++;
            }
        }

        consStr = str[strFlag].substr(0, textFlag).c_str();
        
        
        const FVector3 &pos = getPosInWs();
        

        sf::FloatRect bound = getRenderSprite()->getGlobalBounds();
        printText(window_,consStr, bound.left + leftEdge,
                  bound.top + topEdge,0.3/pixSize);
    }
};

#endif // GTALK_H
