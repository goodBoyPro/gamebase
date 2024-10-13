#include <game.h>
#include <xmlRead.hpp>
#include<mylib.h>
#if 1
int main() {
    mylib::ProgramMutex proMut(L"main");
    if(proMut.isOtherProgramOn())
        return 0;

    initTools();
    
    gameIns = new Game;
    delete gameIns;
    xlib::getTimer().brun = false;
    return 0;
}

#else
int main(){
    
    xmlRead xr("res/animData.xml");
    int a = getSource().animations[1].sizex;
    printf("%d",std::stoi(xr[1]["sizex"]));
    getSource();

    return 0; }
#endif
