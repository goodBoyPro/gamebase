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
    ~gametest();
};

gametest::gametest(/* args */)
{
    world1 = new worldTest;
    setWorld(world1);
    gameBegin();    
}

gametest::~gametest()
{
    delete world1;
}

#endif // GAMETEST_H)
