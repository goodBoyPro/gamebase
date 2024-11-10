#include <GDebug.h>
#include <GWorld.h>
#include <actorTest.h>
#include <house.h>
#include <playertest.h>
#include <trees.h>
class actortest2:public GActor{
    public:
    actortest2(){mapNodeId=40080;}
    virtual ~actortest2(){}
    void eventTick(){printf("actortest2");}
    void eventBegin(){}
};
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
        Playertest *player =spawnActorAtLocation<Playertest>() ;
        setPlayerCharactor(player);     
        getPlayerCharactor()->setPosInWs({500 * pixSize, 500 * pixSize,0});

        for (int i = 0; i < 200; i++) {
            trees *a = new trees(rand() % 25);
            a->setPosInWs({rand() % 3840 * pixSize, rand() % 3840 * pixSize,0});
            // treeVec.push_back(a);
        }

        for (int i = 0; i < 20; i++) {
            house *b = new house(rand() % 5);

            b->setPosInWs({rand() % 10000 * pixSize, rand() % 10000 * pixSize,0});
            // houseVec.push_back(b);
        }
    };
    ~worldTest() { delete getPlayerCharactor(); };
};
