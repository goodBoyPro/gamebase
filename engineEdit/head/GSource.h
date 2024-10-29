#if !defined(GSOURCE_H)
#define GSOURCE_H
#include <GAnimation.h>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <map>
#include <unordered_map>
#include <xmlRead.hpp>
class SourceInteface
{
};
class textureAndSprite
{
public:
    sf::Texture tex;
    sf::Sprite spr;
    textureAndSprite() = default;
    textureAndSprite(std::map<std::string, std::string> &list_)
    {
        if (!tex.loadFromFile(list_["path"]))
            printf("file[%s] load failed\n", list_["path"].c_str());
        spr.setTexture(tex);
        spr.setOrigin(tex.getSize().x / 2,
                      tex.getSize().y / 2);
    }
};
class soundRes
{
public:
    sf::Sound sound;
    sf::SoundBuffer soundBuf;
    soundRes() = default;
    soundRes(std::map<std::string, std::string> &list_)
    {
        if (!soundBuf.loadFromFile(list_["path"]))
            printf("file[%s] load failed\n", list_["path"].c_str());
        sound.setBuffer(soundBuf);
    }
};
class GSource
{
public:
    bool isLoadComplete = false;

public:
    GSource(std::string pathTexture, std::string pathSound, std::string pathAnimation)
    {
        loadTextureRes(pathTexture);
        loadSoundRes(pathSound);
        loadAnimation(pathAnimation);
        isLoadComplete = true;
    }

    sf::Sprite &getSprite(int id) { return texSpr[id].spr; }
    sf::Sound &getSound(int id) { return sounds[id].sound; }
    GAnimation &getAnimation(int id) { return animations[id]; }
    // 全局资源
    static GSource &getSource()
    {
        static GSource gs__("res/picData.xml", "res/soundData.xml", "res/animData.xml");
        return gs__;
    }

private:
    std::unordered_map<int, textureAndSprite> texSpr;
    std::unordered_map<int, soundRes> sounds;
    std::map<int, GAnimation> animations;

private:
    void loadTextureRes(std::string pathTexture)
    {
        xmlRead xr(pathTexture.c_str());
        for (auto &pair : xr.mapData)
        {
            texSpr.emplace(pair.first, pair.second);
        }
    }
    void loadSoundRes(std::string pathSound)
    {
        xmlRead xr(pathSound.c_str());
        for (auto &pair : xr.mapData)
        {
            sounds.emplace(pair.first, pair.second);
        }
    }
    void loadAnimation(std::string pathAnimation);
};

GSource &getSource();
sf::Sprite &getSprite(int id);
#endif // GSOURCE_H
