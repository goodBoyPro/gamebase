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
  
    createWorld(new worldTest("res/datalist/world/wtest.json"));
    gameBegin();    
}
gametest::gametest(std::string path_)
{
     createWorld(new worldTest(path_));
    gameBegin();    
}

gametest::~gametest()
{
    
}

#endif // GAMETEST_H)
