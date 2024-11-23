#if !defined(MYLIB_H)
#define MYLIB_H
#include <stdio.h>
#include <windows.h>
namespace mylib {
// 程序单例模式
struct ProgramMutex {
    HANDLE hMutex;
    ProgramMutex(const wchar_t *progName) {
        hMutex = CreateMutex(NULL, FALSE, progName);
    }
    bool isOtherProgramOn() {
        if (GetLastError() == ERROR_ALREADY_EXISTS) {
            CloseHandle(hMutex);
            printf("another program instance is running\n");
            return 1;
        }
        return 0;
    }
};

} // namespace mylib

#endif // MYLIB_H
