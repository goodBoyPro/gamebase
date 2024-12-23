#include<mylib.h>
#include<gametest.h>
inline void gameEmulator(std::string path_) {
    if(path_=="")
        return;
    gametest g(path_);
    printf("\nprogram has ended without error\n");
    
}