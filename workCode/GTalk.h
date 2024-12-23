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
    GTalk(/* args */) { setRenderSprite(&getSprite(11)); };
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
    virtual void drawActor() override { setContent(); }
    void setContent() {
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
        sf::Sprite &sprTemp = *getRenderSprite();
        sprTemp.setScale(1, 0.5);
        const FVector3 &pos = getPosInWs();
        IVector posWin = wsToWin(pos);
        sprTemp.setPosition(posWin.x, posWin.y-playerSpr->getGlobalBounds().height);
        sprTemp.setOrigin(0, 256);
        getWindow()->draw(*getRenderSprite());

        printText(*getWindow(),consStr, posWin.x + leftEdge,
                  posWin.y - sprTemp.getGlobalBounds().height + topEdge-playerSpr->getGlobalBounds().height);
    }
};

#endif // GTALK_H
