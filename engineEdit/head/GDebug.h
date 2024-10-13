#if !defined(GDBUG)
#define GDBUG
#include <GObject.h>

// 示例： static GDebug gddd;
// swprintf(gddd.wchar_, L"场景对象数量：%d", actors.size());
class GDebug : public GObject {
  private:
    /* data */
  public:
    GDebug();

    virtual ~GDebug();
    int yPosition = 0;
    int texHight = 10;
    wchar_t wchar_[64] = {0};
    void drawLog();
    virtual void drawLoop();
    bool shouldDestroy = 0;
    static int logIndex;
};
extern std::vector<GDebug *> debugs__;
void addDebugTask(GDebug *d__);
void debugDisplay();

#endif // GDBUG
