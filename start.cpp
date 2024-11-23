#include<cstdlib>
#include <direct.h>
#include<stdio.h>
#include<mylib.h>
int main(){
    mylib::ProgramMutex proMut(L"start");
    if(proMut.isOtherProgramOn())
        return 0;
    if(_chdir("bin")==0)printf("dir changed");  
    else
        perror("dir bad change");
    //终端编码切换为utf8
    system("chcp 65001");
    system("game.exe");
    return 0;
}