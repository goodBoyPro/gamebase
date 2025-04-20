#include <gametest.h>
#include <xmlRead.hpp>
#include<mylib.h>
#if 1
int main() {
    
    system("chcp 65001");
    mylib::ProgramMutex proMut(L"main");
    if(proMut.isOtherProgramOn())
        return 0;
    
    initTools();
    gametest g;
    xlib::getTimer().stop();
    thread_pool::getThreadPool().stop();
    printf("\nprogram has ended without error\n");
    system("pause");
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
