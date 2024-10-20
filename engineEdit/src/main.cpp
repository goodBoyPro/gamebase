#include <gametest.h>
#include <xmlRead.hpp>
#include<mylib.h>
#if 1
int main() {
    mylib::ProgramMutex proMut(L"main");
    if(proMut.isOtherProgramOn())
        return 0;

    initTools();
    gametest g;
    xlib::getTimer().brun = false;
    return 0;
}
#endif

#if 0
int main(){
    
    xmlRead xr("res/animData.xml");
    int a = getSource().animations[1].sizex;
    printf("%d",std::stoi(xr[1]["sizex"]));
    getSource();

    return 0; }
#endif
