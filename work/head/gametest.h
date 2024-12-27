#if !defined(GAMETEST_H)
#define GAMETEST_H
#include <worldTest.h>
#include "game.h"
class gametest : public Game
{
private:
    /* data */
public:
    gametest(/* args */);
    gametest(std::string path_);
    ~gametest();
};

gametest::gametest(/* args */)
{
    // world1 = new worldTest("res/datalist/world/wtest.json");
    // setWorld(world1);
    createWorld(new worldTest("res/datalist/world/wtest.json"));
    printf("%p\n%p",GGameInterface::getGameIns(),Game::gameIns);
    gameBegin();    
}
gametest::gametest(std::string path_)
{
     createWorld(new worldTest(path_));
    gameBegin();    
}

gametest::~gametest()
{
    delete world1;
}

#endif // GAMETEST_H)
