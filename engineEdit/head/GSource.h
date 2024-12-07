#if !defined(GSOURCE_H)
#define GSOURCE_H
#include <GAnimation.h>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <map>
#include <unordered_map>
#include <xmlRead.hpp>
class SourceInteface {};
class textureArray {
  private:
    std::vector<sf::Sprite> sprs;
    int sizex, sizey, row, column, originX, originY;

  public:
    sf::Texture tex;
    sf::Sprite spr;

    void setSprite(gameSprite &spr_, int index) {
        if (index >= column * row)
            return;
        spr_.setTexture(tex);
        spr_.setTextureRect(
            {index % column * sizex, index /column * sizey, sizex, sizey});
        spr_.setOrigin(originX, originY);
    }
    textureArray() = default;
    textureArray(std::map<std::string, std::string> &list_) {
        if (!tex.loadFromFile(list_["path"]))
            printf("%s load failed", list_["path"].c_str());
        spr.setTexture(tex);
        sizex = std::stoi(list_["sizex"]);
        sizey = std::stoi(list_["sizey"]);
        row = std::stoi(list_["row"]);
        column = std::stoi(list_["column"]);
        originX = std::stoi(list_["originX"]);
        originY = std::stoi(list_["originY"]);
        spr.setOrigin(originX, originY);
        createSprs();
    }

    sf::Sprite &getSprite(int index) { return sprs[index]; }

  private:
    // 第一格编号为0
    void selectSprite(int index) {
        spr.setTextureRect(
            {index % column * sizex, index / column * sizey, sizex, sizey});
    }
    void createSprs() {
        int num = row * column;
        for (int i = 0; i < num; i++) {
            ;
            sf::Sprite &sprLoc = sprs.emplace_back(tex);
            sprLoc.setTextureRect(
                {i % column * sizex, i / column * sizey, sizex, sizey});
            sprLoc.setOrigin(originX, originY);
        }
    }
};
class textureAndSprite {
  public:
    sf::Texture tex;
    sf::Sprite spr;
    textureAndSprite() = default;
    textureAndSprite(std::map<std::string, std::string> &list_) {
        if (!tex.loadFromFile(list_["path"]))
            printf("file[%s] load failed\n", list_["path"].c_str());
        spr.setTexture(tex);
        spr.setOrigin(tex.getSize().x / 2, tex.getSize().y / 2);
    }
};
class soundRes {
  public:
    sf::Sound sound;
    sf::SoundBuffer soundBuf;
    soundRes() = default;
    soundRes(std::map<std::string, std::string> &list_) {
        if (!soundBuf.loadFromFile(list_["path"]))
            printf("file[%s] load failed\n", list_["path"].c_str());
        sound.setBuffer(soundBuf);
    }
};
/// @brief
/// ///////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// ///////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// ///////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// ///////////////////////////////////////////////////////////////////////////////////////////////////////////
class GSource {
  public:
    bool isLoadComplete = false;

  public:
    GSource(std::string pathTexArray, std::string pathTexture,
            std::string pathSound, std::string pathAnimation) {
        loadAllTextureArray(pathTexArray);
        loadTextureRes(pathTexture);
        loadSoundRes(pathSound);
        loadAnimation(pathAnimation);
        isLoadComplete = true;
    }

    textureArray &getTexArray(int id) { return allTextureArrays[id]; };
    void setSprite(gameSprite &spr_, int fileId, int picIndex) {
        getTexArray(fileId).setSprite(spr_, picIndex);
    }
    sf::Sprite &getSprite(int id) { return texSpr[id].spr; }
    sf::Sound &getSound(int id) { return sounds[id].sound; }
    GAnimation &getAnimation(int id) { return animations[id]; }
    // 全局资源
    static GSource &getSource() {
        static GSource gs__("res/textureArray.xml", "res/picData.xml",
                            "res/soundData.xml", "res/animData.xml");
        return gs__;
    }

  private:
    std::map<int, textureAndSprite> texSpr;
    std::map<int, soundRes> sounds;
    std::map<int, GAnimation> animations;
    std::map<int, textureArray> allTextureArrays;

  private:
    void loadAllTextureArray(const std::string &pathTexArrays) {
        xmlRead xr(pathTexArrays.c_str());
        for (auto &pair : xr.mapData) {
            allTextureArrays.emplace(pair.first, pair.second);
        }
    }
    void loadTextureRes(const std::string &pathTexture) {
        xmlRead xr(pathTexture.c_str());
        for (auto &pair : xr.mapData) {
            texSpr.emplace(pair.first, pair.second);
        }
    }
    void loadSoundRes(const std::string &pathSound) {
        xmlRead xr(pathSound.c_str());
        for (auto &pair : xr.mapData) {
            sounds.emplace(pair.first, pair.second);
        }
    }
    void loadAnimation(const std::string &pathAnimation) {
        xmlRead xr(pathAnimation.c_str());
        for (auto &pair : xr.mapData) {
            //  animations[pair.first] = {pair.second};不是原地构造，发生了拷贝
            //   animations.emplace(std::piecewise_construct,
            //                      std::forward_as_tuple(pair.first),
            //                      std::forward_as_tuple(pair.second));//原地构造
            animations.emplace(pair.first, pair.second); // 原地构造
        }
    };
};
inline GSource &getSource() { return GSource::getSource(); }

inline sf::Sprite &getSprite(int id) { return getSource().getSprite(id); }

#endif // GSOURCE_H
