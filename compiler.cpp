#include <windows.h>
#include<filesystem>
// #include<direct.h>
int main()
{
    // _chdir("build");
    std::string folderPath = "./build";

    // 检查文件夹是否存在
    if (!std::filesystem::exists(folderPath) && std::filesystem::create_directory(folderPath))
    {
        // 文件夹不存在，创建文件夹      
                printf("build folder created\n");
    }
    

    SetCurrentDirectory("build");
    system("cmake -G \"MinGW Makefiles\" ..");
    system("cmake --build .");
    return 0;
}
