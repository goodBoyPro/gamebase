#include "GDebug.h"

#include <game.h>
std::set<GDebug *> debugs__;

void GDebug::debugDisplay()
{
    // 非必要操作，应避免在堆区创建对象；
    //  debugs__.erase(std::remove_if(debugs__.begin(), debugs__.end(),
    //                                [](GDebug *a) { return a->shouldDestroy; }),debugs__.end());
    int i = 0;
    for (auto a : debugs__)
    {   
        if(a==nullptr)continue;
        a->drawLog(i);
        i++;
    }
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

GDebug::GDebug()
{
    debugs__.emplace(this);
}

GDebug::~GDebug() {  
   debugs__.erase(this);//可能存在线程安全问题，遇到时解决
}

void GDebug::drawLog(int index)
{

    printText(wchar_, 0, 30 * index);
}

void GDebug::clearDebugs()
{
    for(auto d:debugs__)delete d;
}
