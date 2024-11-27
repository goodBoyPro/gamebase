#include <GDebug.h>
#include <GWorld.h>
#include <actorTest.h>
#include <house.h>
#include <playertest.h>
#include <trees.h>
class worldTest : public GWorld {
  private:
    /* data */
  public:
    sf::RenderTexture mapTex;
    GDebug deb;
    std::vector<trees *> treeVec;
    std::vector<house *> houseVec;
    worldTest() {
        mapTex.create(3840, 3840);
        tex.loadFromFile("res/grass.png");
        spr.setTexture(tex);
        for (int i = 0; i < 15; i++) {
            for (int j = 0; j < 15; j++) {
                spr.setPosition(i * 256, j * 256);
                spr.setTextureRect(
                    {rand() % 3 * 256, rand() % 3 * 256, 256, 256});
                mapTex.draw(spr);
            }
        }
        mapTex.display();
        spr.setTexture(mapTex.getTexture(), 1);
        Playertest *player = spawnActorAtLocation<Playertest>();
        setPlayerCharactor(player);
        getPlayerCharactor()->setPosInWs({500 * pixSize, 500 * pixSize, 0});

         for (int i = 0; i < 10; i++) {

            trees *a = spawnActorAtLocation<trees>(
                {rand() % 3840 * pixSize, rand() % 3840 * pixSize, 0});
            a->init(rand() % 25);
        }
       

        // 线程安全测试
        for (int i = 0; i < 30000; i++) {
            spawnActorAtLocation<actorTest>({5,6,0});
        }
    };
    ~worldTest() { delete getPlayerCharactor(); };
};
