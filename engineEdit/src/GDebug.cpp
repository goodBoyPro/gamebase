#include "GDebug.h"

#include <game.h>
std::vector<GDebug *> debugs__;
void addDebugTask(GDebug *d__) { debugs__.push_back(d__); }
void debugDisplay() {
    //非必要操作，应避免在堆区创建对象；
    // debugs__.erase(std::remove_if(debugs__.begin(), debugs__.end(),
    //                               [](GDebug *a) { return a->shouldDestroy; }),debugs__.end());
      GDebug::logIndex = 0;
    for (auto a : debugs__) {
        a->drawLoop();
    }
    
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
int GDebug::logIndex = 0;
GDebug::GDebug() {
    addDebugTask(this);
}



GDebug::~GDebug() { }

void GDebug::drawLog() {

    // swprintf(wchar_, L"%f,%f", playerCharacter->posInWs.x,
    //          playerCharacter->posInWs.y);

    printText(wchar_, 0, 30 *logIndex);
    logIndex++;
}
void GDebug::drawLoop() {
    // 示例
    // swprintf(wchar_, L"示例：%d", 1);
    // drawLog(0);
    drawLog();
}
