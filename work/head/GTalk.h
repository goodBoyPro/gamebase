#if !defined(GTALK_H)
#define GTALK_H
#include<GSource.h>
#include <GBase.h>
class GTalk {
  private:
    /* data */
  public:
    GTalk(/* args */) = default;
    ~GTalk() = default;
    int leftEdge = 10;
    int topEdge = 5;

    std::wstring str[4] = {L"床前看月光，", //
                           L"疑是地上霜。", //
                           L"举头望山月，", //
                           L"低头思故乡。"};
    // void draw(const FVector &pos, const wchar_t *ch) {
    //     GSource::getSource().sprTalk1.setScale(1, 0.5);
    //     GSource::getSource().sprTalk1.setPosition(pos.x, pos.y);
    //     window->draw(GSource::getSource().sprTalk1);
    //     printText(ch, pos.x + leftEdge,
    //               pos.y -
    //                   GSource::getSource().sprTalk1.getGlobalBounds().height
    //                   + topEdge);
    // }

    
    int textFlag = 0;
    int strFlag = 0;
    canRun cr;
    canRun sr;
    void draw(const FVector &pos, const wchar_t *ch) {
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
        // GSource::getSource().sprTalk1.setScale(1, 0.5);
        // GSource::getSource().sprTalk1.setPosition(pos.x, pos.y);
        // window->draw(GSource::getSource().sprTalk1);
        sf::Sprite &spr = getSprite(11);
        spr.setScale(1, 0.5);
        spr.setPosition(pos.x, pos.y);
        spr.setOrigin(0,256);
        window->draw(spr);

        printText(consStr, pos.x + leftEdge,
                  pos.y -
                      spr.getGlobalBounds().height +
                      topEdge);
    }
};

#endif // GTALK_H
