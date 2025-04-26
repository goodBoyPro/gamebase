#ifndef GWORLD_H
#define GWORLD_H

#include "GObject.h"
#include<nlohmann_json/json.hpp>
#include<fstream>
#include<GLandBlock.h>
class GWorld : public GIWorld {
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
    //picNum是最大数量，不是编号
    virtual void createLand(int num,int fileId_,int picNum){
      for(int i=0;i<num;i++){
        GLandBlock::spawnLandBlock(i,fileId_,rand()%picNum);
      }
    };
    virtual nlohmann::json loadDataFromJson(const char* path_){
        
        std::ifstream file(path_);
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