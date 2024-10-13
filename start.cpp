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
    system("game.exe");
    return 0;
}