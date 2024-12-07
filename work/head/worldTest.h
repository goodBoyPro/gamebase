#include <GDebug.h>
#include <GWorld.h>
#include <actorTest.h>
#include <house.h>
#include <playertest.h>
#include <GActorStatic.h>
class worldTest : public GWorld {
  private:
    /* data */
  public:
    sf::RenderTexture mapTex;
    GDebug deb;
    std::vector<GActorStatic *> treeVec;
    std::vector<house *> houseVec;
    worldTest() {
        // mapTex.create(3840, 3840);
        // tex.loadFromFile("res/grass.png");
        // spr.setTexture(tex);
        // for (int i = 0; i < 15; i++) {
        //     for (int j = 0; j < 15; j++) {
        //         spr.setPosition(i * 256, j * 256);
        //         spr.setTextureRect(
        //             {rand() % 3 * 256, rand() % 3 * 256, 256, 256});
        //         mapTex.draw(spr);
        //     }
        // }
        // mapTex.display();
        // spr.setTexture(mapTex.getTexture(), 1);
        Playertest *player = spawnActorAtLocation<Playertest>();
        setPlayerCharactor(player);
        getPlayerCharactor()->setPosInWs({0 * pixSize, 0 * pixSize, 0});
        nlohmann::json info =
            loadDataFromJson("res/datalist/world/world1.json");
        for (auto ob : info) {
            std::string fileIDs = ob["fileID"];
            std::string picIndexs = ob["picIndex"];
            std::string positionS = ob["position"];
            int fileID = std::stoi(fileIDs);
            int picIndex=std::stoi(picIndexs);
            FVector3 pos = nsg::strTo3Float(positionS);
            spawnActorArgsAtLocation(new GActorStatic(fileID,picIndex),
                                     pos);
        }

        // float n = 0;
        // for (int i = 0; i < 10; i++) {

        //     trees *a = spawnActorArgsAtLocation<trees>(new trees(2,1),
        //        {n,n,0});
        //     n++;

        // }

        // 线程安全测试
        //  for (int i = 0; i < 30000; i++) {
        //      spawnActorAtLocation<actorTest>({0,0,0});
        //  }
    };
    ~worldTest() { delete getPlayerCharactor(); };
};
