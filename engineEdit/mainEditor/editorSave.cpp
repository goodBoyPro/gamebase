#include"editorSave.h"
#include"gameEditor.h"
namespace ens
{
    void editorSave::saveTofile(std::string path_) {
    std::ofstream ofile("wtest.json", std::ios::binary);
    if(!ofile.is_open())printf("%s open failed",path_.c_str());
    nlohmann::json root;
    nlohmann::json child_actors;
    nlohmann::json child_landBlocks;
    nlohmann::json worldSet;
    for(auto&meo:MovableEditObj::allMEO){
        if(meo->type==MovableEditObj::eactor){
            nlohmann::json info;
            info["fileID"] = meo->info.fileID;
            info["picIndex"] = meo->info.picIndex;
            info["position"] = nlohmann::json({meo->info.position.x,meo->info.position.y,meo->info.position.z});
            child_actors.push_back(info);
            
        }
        root["actors"]=child_actors;
    }
    std::string str = root.dump(4);
    ofile.write(str.data(), str.size());
    ofile.close();
}
    
} // namespace ens
