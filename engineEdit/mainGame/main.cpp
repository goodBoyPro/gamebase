#include <gametest.h>
#include <xmlRead.hpp>
#include<mylib.h>
#if 1
int main() {
    xlib::createTimeManager();
    system("chcp 65001");
    // mylib::ProgramMutex proMut(L"main");
    // if(proMut.isOtherProgramOn())
    //     return 0;
    
    // initTools();
    // gametest g;
    std::atomic<int>a=0;
    std::atomic<bool>b=false;
    xlib::getTimer().setPause(false);
    xlib::getTimer().addTaskSafe(getTime(),50,100,&a,&b,[](){printf("testttttttttttttttttttttttttt\n");});
    Sleep(6000);
    
    
    
    xlib::destroyTimeManager();
    printf("\nprogram has ended without error\n");
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
