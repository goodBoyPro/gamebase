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
    world1 = new worldTest("res/datalist/world/wtest.json");
    setWorld(world1);
    gameBegin();    
}
gametest::gametest(std::string path_)
{
    world1 = new worldTest(path_);
    setWorld(world1);
    gameBegin();    
}

gametest::~gametest()
{
    delete world1;
}

#endif // GAMETEST_H)
