#if !defined(GSOURCE_H)
#define GSOURCE_H
#include<GAnimation.h>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <map>
#include <unordered_map>
#include <xmlRead.hpp>

struct textureAndSprite {
    sf::Texture tex;
    sf::Sprite spr;
    void loadFile(const char *path) {
        if (!tex.loadFromFile(path))
            printf("file[%s] load failed\n", path);
        spr.setTexture(tex);
    }
};
struct soundRes {
    sf::Sound sound;
    sf::SoundBuffer soundBuf;
    void loadFile(const char *path) {
        if (!soundBuf.loadFromFile(path))
            printf("file[%s] load failed\n", path);
        sound.setBuffer(soundBuf);
    }
};
class GSource {
  public:
    bool isLoadComplete = false;
    std::unordered_map<int, textureAndSprite> texSpr;
    std::unordered_map<int, soundRes> sounds;
    std::map<int, GAnimation> animations;

    GSource() {
        loadTextureRes();
        loadSoundRes();
        loadAnimation();
        isLoadComplete = true;
        std::string x;
    }
    void loadTextureRes() {
        xmlRead xr("res/picData.xml");
        for (auto &pair : xr.mapData) {

            texSpr[pair.first] = textureAndSprite();
            textureAndSprite &tempTs = texSpr[pair.first];
            tempTs.loadFile(pair.second["path"].c_str());
            tempTs.spr.setOrigin(tempTs.tex.getSize().x / 2,
                                 tempTs.tex.getSize().y / 2);
        }
    }
    void loadSoundRes() {
        xmlRead xr("res/soundData.xml");
        for (auto &pair : xr.mapData) {
            sounds[pair.first] = soundRes();
            soundRes &sr = sounds[pair.first];
            sr.loadFile(pair.second["path"].c_str());
        }
    }
    void loadAnimation();
    static GSource &getSource() {
        static GSource gs__;
        return gs__;
    }
};

GSource &getSource();
sf::Sprite &getSprite(int id);
#endif // GSOURCE_H
