#include <GDebug.h>
#include <GWorld.h>
#include <actorTest.h>

#include <GActorStatic.h>
#include <playertest.h>
class worldTest : public GWorld {
  private:
    /* data */
  public:
    sf::RenderTexture mapTex;
    GDebug deb;
    std::vector<GActorStatic *> treeVec;

    worldTest(std::string path_) {

        Playertest *player = spawnActorAtLocation<Playertest>();
        setPlayerCharactor(player);
        getPlayerCharactor()->setPosInWs({0 * pixSize, 0 * pixSize, 0});
        nlohmann::json info = loadDataFromJson(path_.c_str());
        for (auto ob : info["actors"]) {
            int fileID = ob["fileID"].get<int>();
            int picIndex = ob["picIndex"].get<int>();
            int type = ob["type"].get<int>();
            std::vector<float> posTemp =
                ob["position"].get<std::vector<float>>();
            std::vector<float> sizeTemp = ob["sizeInWs"].get<std::vector<float>>();
            FVector3 pos = {posTemp[0], posTemp[1], posTemp[2]};
            GActor *ac = createActorFromJson(type);
            ac->setPosInWs(pos);
            ac->setSprite(fileID, picIndex);
            ac->setSizeInWs({sizeTemp[0],sizeTemp[1],sizeTemp[2]});
        }
        for (auto ob : info["landBlocks"]) {
            int fileID = ob["fileID"].get<int>();
            int picIndex = ob["picIndex"].get<int>();
            int type = ob["type"].get<int>();
            std::vector<float> posTemp =
                ob["position"].get<std::vector<float>>();
            std::vector<float> sizeTemp = ob["sizeInWs"].get<std::vector<float>>();
            FVector3 pos = {posTemp[0], posTemp[1], posTemp[2]};
            GActor *ac = createActorFromJson(type);
            ac->setPosInWs(pos);
            ac->setSprite(fileID, picIndex);
            ac->setSizeInWs({sizeTemp[0],sizeTemp[1],sizeTemp[2]});
        }
    };
    ~worldTest() { delete getPlayerCharactor(); };
};
