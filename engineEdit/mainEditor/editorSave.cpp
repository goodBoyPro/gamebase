#include "editorSave.h"
#include "gameEditor.h"
#include<stdio.h>
namespace ens
{
    void editorSave::saveTofile(std::string path_)
    {
        std::ofstream ofile(path_, std::ios::binary);
        if (!ofile.is_open())
            printf("%s open failed", path_.c_str());
        nlohmann::json root;
        nlohmann::json child_actors;
        nlohmann::json child_landBlocks;
        nlohmann::json worldSet;
        for (auto &meo : MovableEditObj::allMEO)
        {
            nlohmann::json info;
            info["fileID"] = meo->info.fileID;
            info["picIndex"] = meo->info.picIndex;
            info["position"] = nlohmann::json({meo->info.position.x, meo->info.position.y, meo->info.position.z});
            if (meo->type == MovableEditObj::eactor)
            {
                child_actors.push_back(info);
            }
            else if (meo->type == MovableEditObj::elandBlock)
            {
                child_landBlocks.push_back(info);
            }
            root["actors"] = child_actors;
            root["landBlocks"] = child_landBlocks;
        }
        std::string str = root.dump(4);
        ofile.write(str.data(), str.size());
        ofile.close();
    }

    void editorSave::loadFromFile(std::string path_)
    {
        
        std::ifstream ifile(path_, std::ios::binary);
        if (!ifile.is_open())
            printf("json open failed");
            std::stringstream temp;
            temp<<ifile.rdbuf();
            std::string text=temp.str();
        nlohmann::json jsObj=nlohmann::json::parse(text);
        
        ifile.close();
        printf("%s",text.c_str());
        for(auto it=jsObj["actors"].begin();it!=jsObj["actors"].end();it++){
            int fileId=it.value()["fileID"];
            int picIndex=it.value()["picIndex"];
            std::vector<float> position=it.value()["position"].get<std::vector<float>>();
             createAtLocation(new Actor(fileId,picIndex),
                             {position[0],position[1],position[2]});
        }
        
       
        
    }

}; // namespace ens
