#include <GWorld.h>
#include <actorTest.h>
#include<trees.h>
#include <playertest.h>
#include<debug.h>
#include<house.h>
class worldTest : public GWorld {
  private:
    /* data */
  public:
    sf::RenderTexture mapTex;
    debug deb;
    std::vector<trees *> treeVec;
    std::vector<house *> houseVec;
    worldTest() {
        mapTex.create(3840,3840);
        tex.loadFromFile("res/grass.png");        
        spr.setTexture(tex);
        for (int i = 0; i < 15;i++){
            for (int j = 0; j < 15;j++){
                spr.setPosition(i*256, j*256);
                spr.setTextureRect({rand()%3*256,rand()%3*256,256,256});
                mapTex.draw(spr);
            }
        }
        mapTex.display();        
        spr.setTexture(mapTex.getTexture(), 1);
        

        playerCharacter = new Playertest;
        playerCharacter->posInWs = {500*pixSize, 500*pixSize};
       
        
        for (int i = 0; i < 200;i++)
        {
            trees *a = new trees(rand()%25);
            a->posInWs = {rand()%3840*pixSize,rand()%3840*pixSize};
            treeVec.push_back(a);
        }
        new house(1);
         for (int i = 0; i < 20;i++)
        {
             house *b = new house(rand()%5);
            
             b->posInWs = {rand() % 1000 * pixSize, rand() % 1000 * pixSize};
             houseVec.push_back(b);
        }

        // delay(10, 30000, []() {
        //     new actorTest;
        //      new actorTest;
        //       new actorTest;
        //       new actorTest;
        //      new actorTest;
        //       new actorTest;
             
                                      
        // });
       
    };
    ~worldTest() { delete playerCharacter; };
};
