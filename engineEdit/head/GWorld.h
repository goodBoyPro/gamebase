#ifndef GWORLD_H
#define GWORLD_H

#include "GObject.h"
#include<nlohmann_json/json.hpp>
#include<fstream>
#include<GLandBlock.h>
class GWorld : public GObject {
  private:
    /* data */
  public:
    GWorld(/* args */);
    virtual ~GWorld();
    FVector3 posWs;
    sf::Sprite *sprPt;
    sf::Sprite spr;
    sf::Texture tex;
    virtual void dataLoop();
    virtual void drawLoop();
    virtual void createLand(){
      for(int i=0;i<10000;i++){
        GLandBlock::spawnLandBlock(i);
      }
    };
    virtual nlohmann::json loadDataFromJson(const char* path_){
        
        std::ifstream file("res/datalist/world/world1.json");
        if(!file.is_open())
           { printf(path_);
            PRINTF("json load failed");}
        return nlohmann::json::parse(file);
    }

  protected:
    // 注册actor类
    enum actorName {
        actorTestClass = 0, //
        
    };
};

#endif